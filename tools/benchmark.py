import logging
import json
import zlib
from pathlib import Path
from typing import Any
from subprocess import check_call
from collections import defaultdict

import jmespath

from palgen import Extension, Sources, Model
from palgen.ingest import Name, Toml


def fix_value(value):
    if isinstance(value, bool):
        return "ON" if value else "OFF"
    return value


def generate_trace(file, output, defines: dict[str, Any]):
    defines = [
        f'-D{name}={fix_value(value)}' for name, value in defines.items()]
    includes = ['-I../../include', '-I/home/che/src/magic_enum/include']
    standard = ['-std=c++20']
    profile_flags = ['-ftime-trace', '-c']

    call = ["clang++", str(file), '-o', str(output), *
            defines, *includes, *standard, *profile_flags]

    output.parent.mkdir(exist_ok=True, parents=True)
    check_call(call, cwd=file.parent)
    output.unlink()

    trace_file = output.with_suffix('.json')
    logging.info("Generated %s", trace_file)
    return trace_file


def analyze_trace(file: Path, queries):
    trace_times = defaultdict(list)
    trace = json.loads(file.read_text('utf-8'))
    for query_name, entry in queries.items():
        search_result = jmespath.search(entry, trace)

        if len(search_result) > 1:
            logging.warning("Found more than one result")
            continue

        trace_times[query_name].append(search_result[0])
    return trace_times


class Config(Model):
    source: Path
    profile: dict[str, Any]
    query: dict[str, Any]


class Analyze:
    def __init__(self, config: Config, repeat_each: int, file: Path, output: Path):
        self.config = config
        self.output = output
        self.repeat_each = repeat_each
        self.queries = {name: jmespath.compile(query) for name, query in config.query.items()}

        self.runs: dict[str, list[Path]] = {
            name: [generate_trace(file, self.output / file.parent.stem / f'{name}.{idx}.obj', features)
                   for idx in range(self.repeat_each)]
            for name, features in self.config.profile.items()}

    def _search(self, trace_file: Path):
        trace = json.loads(trace_file.read_text('utf-8'))
        for name, query in self.queries.items():
            result = query.search(trace)
            if len(result) > 1:
                logging.warning("More than one result found")
                continue

            yield name, result[0]

    def run_queries(self):
        feature_sets = {name: [dict(self._search(file))
                               for file in files]
                        for name, files in self.runs.items()}
        return {query: {name: [run[query]
                               for run in runs]
                        for name, runs in feature_sets.items()}
                for query in self.queries}

    def worst_offenders(self):
        expensive_classes = jmespath.compile("reverse(sort_by(traceEvents[?name=='ParseClass' "
                                             "&& !starts_with(@.args.detail, 'std')], &dur)[])[:10]")
        expensive_functions = jmespath.compile("reverse(sort_by(traceEvents[?name=='InstantiateFunction' "
                                               "&& !starts_with(@.args.detail, 'std')], &dur)[])[:10]")


class Benchmark(Extension):
    ingest = Sources / 'benchmark' >> Name("profiles.toml") >> Toml

    class Settings(Model):
        output: Path
        repeat_each: int = 1

    Schema = Config

    def run_benchmark(self, data):
        if not self.settings.output.is_absolute():
            self.settings.output = self.root_path / self.settings.output

        for path, config in data:
            if not (file := Path(config.source)).is_absolute():
                file = path.parent / file
            analyzer = Analyze(config, self.settings.repeat_each, file, self.settings.output)
            queries = analyzer.run_queries()
            colors = [f"#{hex(zlib.crc32(name.encode('utf-8')))[4:]}"
                      for name in config.profile]
            summary = {}
            for idx, (query, runs) in enumerate(queries.items()):
                datasets = []
                yMarkers = []
                chart_data = {"labels": [str(idx) for idx in range(self.settings.repeat_each)],
                              "datasets": datasets,
                              "yMarkers": yMarkers}
                logging.info("Query %s", query)
                summary[query] = {}
                for name, run in runs.items():
                    summary[query][name] = min(run)
                    datasets.append({"name": name,
                                     "values": run,
                                     "chartType": 'line'})
                    yMarkers.append({"label": name, "value": min(run)})
                    logging.info("  Feature set %s", name)
                    logging.info("    Results: %s", run)
                    logging.info("    Min: %sms", min(run) / 1000)
                chart = self.settings.output / path.parent.name / f"{query}.json"

                """
                import { Chart } from "frappe-charts/dist/frappe-charts.esm.js";
                import "frappe-charts/dist/frappe-charts.min.css";
                new Chart("#chart", 
                JSON.parse({query}.json)
                );
                """
                chart.write_text(json.dumps({'title': query,
                                             'data': chart_data,
                                             'type': "line",
                                             'height': 400,
                                             'colors': colors,
                                             'lineOptions': {'regionFill': 1, 'spline': 1}}))
            summary_file = self.settings.output / path.parent.name / 'summary.json'
            summary_file.write_text(json.dumps(summary))
    pipeline = ingest >> Extension.validate >> run_benchmark
