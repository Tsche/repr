from collections import defaultdict
from itertools import starmap
import logging
import json
from pathlib import Path
from tempfile import TemporaryDirectory
from typing import Any, Optional
from subprocess import check_call
from dataclasses import dataclass

import jmespath
from git import Repo
from palgen import Extension, Sources, Model
from palgen.ingest import Name, Toml

total_time_query = jmespath.compile("traceEvents[?name=='ExecuteCompiler'].dur")


def fix_value(value):
    if isinstance(value, bool):
        return "ON" if value else "OFF"
    return value


def generate_trace(file, output, defines: dict[str, Any], includes: list[Path], cpu: Optional[int] = None):
    defines = [f"-D{name}={fix_value(value)}" for name, value in defines.items()]
    includes = [f"-I{include}" for include in includes]
    standard = ["-std=c++20"]

    profile_flags = ["-ftime-trace", "-c"]
    taskset_prefix = [] if cpu is None else ["taskset", "--cpu-list", str(cpu)]  # cpu pinning
    call = [*taskset_prefix, "clang++", str(file), "-o", str(output), *defines, *includes, *standard, *profile_flags]

    output.parent.mkdir(exist_ok=True, parents=True)
    check_call(call, cwd=file.parent)
    output.unlink()

    trace_file = output.with_suffix(".json")
    time, *_ = total_time_query.search(json.loads(trace_file.read_text("utf-8")))

    logging.debug("ExecuteCompiler: %dms Output: %s", time // 1000, trace_file)
    return trace_file, time


@dataclass
class Run:
    trace: Path
    total_time: int

    # query name -> time
    results: dict[str, int]
    expensive_classes: list[Any]
    expensive_functions: list[Any]


class Benchmark(Extension):
    ingest = Sources / "benchmark" >> Name("profiles.toml") >> Toml

    total_time_query = jmespath.compile("traceEvents[?name=='ExecuteCompiler'].dur")
    expensive_classes = jmespath.compile(
        "reverse(sort_by(traceEvents[?name=='ParseClass' && !starts_with(@.args.detail, 'std')], &dur)[])[:10]"
    )
    expensive_functions = jmespath.compile(
        "reverse(sort_by(traceEvents[?name=='InstantiateFunction' && !starts_with(@.args.detail, 'std')], &dur)[])[:10]"
    )

    class Settings(Model):
        output: Path
        repeat_each: int = 1
        cpu: Optional[int] = None

    class Schema(Model):
        source: Path
        profile: dict[str, Any]
        query: dict[str, Any]
        includes: list[Path] = []

        class Git(Model):
            url: str
            tag: Optional[str] = None
            include: Optional[str] = None

        third_party: list[Git] = []

    def clone_dependency(self, dependencies: list[Schema.Git]):
        out_path = self.out_path / "third_party"
        for dependency in dependencies:
            repo_name = dependency.url.split("/")[-1].removesuffix(".git")
            target = out_path / repo_name
            if not target.exists():
                logging.info("Cloning %s into %s", dependency.url, target)
                repo = Repo.clone_from(dependency.url, target)
                if dependency.tag:
                    repo.git.checkout(dependency.tag)

            yield target if dependency.include is None else target / dependency.include

    def prepare(self, data: list[tuple[Path, Schema]]):
        self.settings.output = self._expand_path(self.settings.output)

        for config_path, config in data:
            config.source = self._expand_path(config.source, config_path.parent)
            config.query = {name: jmespath.compile(query) for name, query in config.query.items()}

            config.includes = [self._expand_path(include) for include in config.includes]
            if (include_dir := self.root_path / "include").exists():
                config.includes.append(include_dir)

            config.includes.extend(self.clone_dependency(config.third_party))

            yield config_path, config

    def compile(self, data: list[tuple[Path, Schema]]):
        for config_path, config in data:
            group_name = config_path.parent.name

            out_path = self.settings.output / f"{group_name}"
            out_path.mkdir(exist_ok=True, parents=True)

            group_results: dict[str, dict[str, Any]] = defaultdict(lambda: {})
            with TemporaryDirectory() as temp_path:
                for name, features in config.profile.items():
                    args = [
                        (
                            config.source,
                            Path(temp_path) / f"{name}.{idx}.obj",
                            features,
                            config.includes,
                            self.settings.cpu,
                        )
                        for idx in range(self.settings.repeat_each)
                    ]

                    if self.settings.cpu is None:
                        # local import to prevent dependency when ran with pinning options
                        from multiprocessing import Pool

                        with Pool(self.settings.cpu) as pool:
                            results = pool.starmap_async(generate_trace, args).get()
                    else:
                        results = starmap(generate_trace, args)

                    # select best run
                    trace_path, time = min(results, key=lambda x: x[1])
                    logging.info("=> ExecuteCompiler: %dms Output: %s", time // 1000, trace_path)

                    trace_text = trace_path.read_text("utf-8")
                    trace = json.loads(trace_text)
                    query_results = dict(self._search(trace, config.query))
                    for query, result in query_results.items():
                        group_results[query][name] = result

                    yield out_path / f"{name}_trace.json", trace_text
                    yield (
                        out_path / f"{name}.json",
                        json.dumps(
                            {
                                "name": name,
                                "total_time": time,
                                "features": features,
                                "results": query_results,
                                "worst_classes": self._search_list(trace, Benchmark.expensive_classes),
                                "worst_functions": self._search_list(trace, Benchmark.expensive_functions),
                            }
                        ),
                    )

            for query, results in group_results.items():
                logging.info("%s", query)
                for feature_set, time in results.items():
                    logging.info("  %s: %dus (%dms)", feature_set, time, time // 1000)

            yield out_path / "summary.json", json.dumps(group_results)

    pipeline = ingest >> Extension.validate >> prepare >> compile >> Extension.write

    def _search(self, trace, queries):
        for name, query in queries.items():
            result = query.search(trace)
            if len(result) > 1:
                logging.warning("More than one result found")
                continue

            yield name, result[0]

    def _search_list(self, trace, query, reverse=True):
        items = {result["args"]["detail"]: result["dur"] for result in query.search(trace)}
        return sorted(items.items(), key=lambda x: x[1], reverse=reverse)

    def _expand_path(self, path: Path, parent: Optional[Path] = None) -> Path:
        return path if path.is_absolute() else (parent or self.root_path) / path
