import json
from pathlib import Path
from palgen.interface import Extension, Sources, Model
from palgen.ingest import Name
from palgen.machinery.filesystem import walk


class Graphs(Extension):
    ingest = Sources >> Name("summary.json")

    class Settings(Model):
        benchmark_output: Path
        summary_path: Path

    def render(self, data: list):
        for summary in data:
            result = json.loads(summary.read_text())
            summary_file = self.out_path / f'{summary.parent.name}.json'
            if summary_file.exists():
                results = json.loads(summary_file.read_text())
                for query, item in result.items():
                    if query not in results:
                        results[query] = {}
                    for name, value in item.items():
                        if name not in results[query]:
                            results[query][name] = []
                        results[query][name].append(value)

                yield summary_file, json.dumps(results)
            else:
                yield summary_file, json.dumps({query:
                                                {name: [value] for name, value in item.items()}
                                                for query, item in result.items()})
        '''summary = {
            'enum_types' : {
                'title': 'enum_types'
            }
        }'''
    def run(self, _, jobs):
        files = walk(self.settings.benchmark_output)
        self.out_path = self.root_path / self.settings.summary_path
        super().run(files, jobs)
