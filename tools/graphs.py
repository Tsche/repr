import json
from pathlib import Path
from palgen.interface import Extension, Sources, Model
from palgen.ingest import Name
from palgen.machinery.filesystem import walk


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
        benchmark_output: Path
        summary_path: Path

    def generate_charts(self, group: str, summary: dict[str, dict[str, list[int]]]):
        for query, data in summary.items():
            colors = [color_from_name(name) for name in data]
            num_runs = max([len(runs) for runs in data.values()])
            datasets = []
            yMarkers = []
            chart_data = {"labels": [str(idx) for idx in range(num_runs)],
                          "datasets": datasets,
                          "yMarkers": yMarkers}

            for name, runs in data.items():
                datasets.append({"name": name,
                                 "values": runs,
                                 "chartType": 'line'})
                yMarkers.append({"label": name, "value": min(runs)})

            chart = self.out_path / group / f"{query}.json"
            yield chart, json.dumps({'title': query,
                                     'colors': colors,
                                     'data': chart_data,
                                     'type': "line",
                                     'height': 400,
                                     'lineOptions': {'regionFill': 1, 'spline': 1}})

    def render(self, data: list):
        for report_path in data:
            report = json.loads(report_path.read_text())
            summary_file = self.settings.summary_path / \
                f'{report_path.parent.name}.json'
            summary: dict[str, dict[str, list[int]]] = {}

            if summary_file.exists():
                results = json.loads(summary_file.read_text())
                for query, item in report.items():
                    if query not in results:
                        results[query] = {}
                    for name, value in item.items():
                        if name not in results[query]:
                            results[query][name] = []
                        results[query][name].append(value)

                summary = results
            else:
                summary = {query: {name: [value] for name, value in item.items()}
                           for query, item in report.items()}

            yield summary_file, json.dumps(summary)
            yield from self.generate_charts(report_path.parent.name, summary)

    def run(self, _, jobs):
        files = walk(self.settings.benchmark_output)
        if not Path(self.settings.summary_path).is_absolute():
            self.settings.summary_path = self.root_path / self.settings.summary_path
        super().run(files, jobs)
