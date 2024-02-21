from collections import defaultdict
import logging
import json
import tempfile
from datetime import datetime, timezone
from pathlib import Path
from typing import Optional
from palgen.interface import Extension, Sources, Model
from palgen.ingest import Name
from palgen.machinery.filesystem import walk
from git import Repo  # required to clone summary repo


def fnv1a(text: str) -> int:
    result_hash = 2166136261
    for character in text:
        result_hash ^= ord(character)
        result_hash *= 16777619
        result_hash %= 2**32
    return result_hash


def color_from_name(name: str):
    return f"#{fnv1a(name) % 2**24 :06x}"


class Graphs(Extension):
    ingest = Sources >> Name("summary.json")

    class Settings(Model):
        input: Path
        output: Path
        summary_url: str
        summary_path: Optional[Path] = None

    def expand_path(self, path: Path):
        return path if path.is_absolute() else self.root_path / path

    def generate_charts(self, group: str, summary):
        data: dict[str, dict[str, list[tuple[int, str]]]] = defaultdict(lambda: defaultdict(list))
        for query, results in summary.items():
            for result in results:
                commit = result["commit"]
                for feature_set, time in result["results"].items():
                    data[query][feature_set].append((time, commit))

        for query, results in summary.items():
            data: dict[str, list[int]] = defaultdict(list)
            labels: list[str] = []
            for result in results:
                labels.append(result["commit"])
                for feature_set, time in result["results"].items():
                    data[feature_set].append(time)

            colors = [color_from_name(name) for name in data]
            datasets = []
            markers = []
            chart_data = {"labels": labels, "datasets": datasets, "yMarkers": markers}

            for name, runs in data.items():
                datasets.append({"name": name, "values": runs, "chartType": "line"})
                markers.append({"label": name, "value": min(runs)})

            chart = self.settings.output / group / f"{fnv1a(query):x}_chart.json"
            yield (
                chart,
                json.dumps(
                    {
                        "title": query,
                        "colors": colors,
                        "data": chart_data,
                        "type": "line",
                        "height": 400,
                        "lineOptions": {"regionFill": 1, "spline": 1},
                        "isNavigable": True,
                        "valuesOverPoints": True,
                    }
                ) + "\n\n"
            )

    def generate_tables(self, group: str, summary):
        for query, data in summary.items():
            table = self.settings.output / group / f"{fnv1a(query):x}.json"
            for entry in data:
                for key in entry["results"]:
                    entry["results"][key] //= 1000
            yield table, json.dumps(data)

    def render(self, data: list):
        for report_path in data:
            report = json.loads(report_path.read_text())
            summary_file = self.settings.summary_path / f"{report_path.parent.name}.json"
            summary: dict[str, list[dict[str, str | int]]] = {}

            if summary_file.exists():
                summary = json.loads(summary_file.read_text())

            for query, item in report.items():
                if query not in summary:
                    summary[query] = []

                summary[query].append(
                    {
                        "commit": self.repo.git.rev_parse(self.repo.head, short=True),
                        "timestamp": str(datetime.now(timezone.utc).strftime("%Y-%m-%d %H:%M:%S")),
                        "results": item,
                    }
                )

            yield self.settings.output / f"{report_path.parent.name}.json", json.dumps(summary) + "\n\n"
            yield from self.generate_charts(report_path.parent.name, summary)
            yield from self.generate_tables(report_path.parent.name, summary)

        # self.summary_repo.git.add(all=True)
        # yield self.settings.output / "summary.patch", self.summary_repo.git.diff(self.summary_repo.head.commit.tree)

    def run(self, _, jobs):
        tmp_dir = None
        if self.settings.summary_path is None:
            tmp_dir = tempfile.TemporaryDirectory()
            self.settings.summary_path = Path(tmp_dir.name)
        else:
            self.settings.summary_path = self.expand_path(self.settings.summary_path)

        try:
            self.settings.input = self.expand_path(self.settings.input)
            self.settings.output = self.expand_path(self.settings.output)

            self.settings.output.mkdir(exist_ok=True, parents=True)

            files = walk(self.settings.input)
            summary_path = self.settings.summary_path
            if not (summary_path / ".git").exists():
                logging.info("Cloning %s into %s", self.settings.summary_url, summary_path)
                repo = Repo.clone_from(self.settings.summary_url, summary_path)
            else:
                repo = Repo(summary_path)
                if not repo.head.is_detached:
                    logging.info("Updating %s", summary_path)
                    repo.git.pull()

            self.summary_repo = repo
            self.repo = Repo(self.root_path)
            super().run(files, jobs)
        finally:
            if tmp_dir:
                tmp_dir.cleanup()
