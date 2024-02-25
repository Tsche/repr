import logging
import re
import shutil
import subprocess
from pathlib import Path
from typing import Iterable

from palgen import Extension, Model, Sources
from palgen.ingest import Suffix, Absolute

clang_format_path = shutil.which("clang-format")
gperf_path = shutil.which("gperf")


class Gperf(Extension):
    ingest = Sources >> Suffix(".gperf") >> Absolute

    class Settings(Model):
        clang_format: bool = False

    def gperf(self, data: Iterable[Path]):
        for input_file in data:
            result = subprocess.run(
                [gperf_path, str(input_file), "-j", "1", "-m", "100", "-D"],
                text=True,
                capture_output=True,
                check=False,
            )
            assert result.returncode == 0, f"gperf returned {result.returncode}"

            yield input_file.with_suffix(".h"), result.stdout

    def cleanup(self, data: Iterable[tuple[Path, str]]):
        for path, source in data:
            name = path.stem
            source = source.replace(f"{path.parent}/", "") \
                           .replace("inline ", "constexpr ")\
                           .replace("struct ", "constexpr const ")\
                           .replace("const char *", "std::string_view ")\
                           .replace(", size_t len", "") \
                           .replace(", len", "") \
                           .replace("len", "str.length()")\
                           .replace("return 0;", "return nullptr;")

            source = re.sub("static ([a-zA-Z]+) lookup", "constexpr static \\1 lookup", source)
            source = source.replace("lookup", f"{name}_lookup")
            source = re.sub(r"\*str.*!strcmp \(.*?\)", "str == s", source)
            asso_def = re.search(r"static (?P<type>[a-zA-Z ]+) asso_values\[\].*", source)
            print()
            asso_start = asso_def.end()
            asso_end = source.find("};", asso_start) + 2
            insertion_point = source.rfind("};\n", 0, asso_start) + 3
            source = ''.join([
                source[:insertion_point],
                f"\nstatic constexpr {asso_def['type']} asso_values[] =",
                source[asso_start:asso_end],
                '\n',
                source[insertion_point:asso_def.start()],
                source[asso_end:]
                ])
            source = source.replace("asso_values", f"{name}_asso_values")
            yield path, source

    def render(self, data: Iterable[tuple[Path, str]]):
        for path, preprocessed in data:
            if self.settings.clang_format:
                logging.debug("Running clang-format")
                assert (
                    clang_format_path is not None
                ), "Configured to run clang-format but couldn't find clang-format"

                result = subprocess.run(
                    [clang_format_path, "--Wno-error=unknown"],
                    input=preprocessed,
                    text=True,
                    capture_output=True,
                    check=False,
                )
                assert result.returncode == 0, f"clang-format returned {result.returncode}"

                preprocessed = result.stdout

            yield path, preprocessed

    pipeline = ingest >> gperf >> cleanup >> render >> Extension.write