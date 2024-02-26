from tempfile import TemporaryDirectory
from diagtest import Runner
from diagtest.printer import Printer
from diagtest.output.terminal import TerminalPrinter
from diagtest.default import load_compilers, load_languages

from palgen import Extension, Model, Sources
from palgen.ingest import Suffix


class Diagnostics(Extension):
    ingest = Sources / "test" / "diagnostics" >> Suffix(".cpp")

    class Settings(Model):
        language: str = 'c++'
        brief: bool = False
        assertions_first: bool = False


    def render(self, files: list):
        printers: list[Printer] = [TerminalPrinter(self.settings.brief, self.settings.assertions_first)]
        load_compilers()
        load_languages()

        with TemporaryDirectory() as temp_path:
            runner = Runner(files, temp_path, self.settings.language)
            results = list(runner.run())

        for printer in printers:
            printer.print(results)
