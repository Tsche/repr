import logging
import shutil  # required to find clang-format
import subprocess  # required to run clang-format
from pathlib import Path
from typing import Iterable

import em  # requires empy
from palgen import Extension, Model, Sources
from palgen.ingest import Suffixes

clang_format_path = shutil.which("clang-format")


class Codegen(Extension):
    ingest = Sources >> Suffixes(".h.in")

    class Settings(Model):
        clang_format: bool = False

    def render(self, data: Iterable[Path]):
        for path in data:
            content = path.read_text(encoding="utf-8")
            preprocessed = em.expand(content)
            if self.settings.clang_format:
                logging.debug("Running clang-format")
                assert clang_format_path is not None, "Configured to run clang-format but couldn't find clang-format"

                result = subprocess.run(
                    [clang_format_path, "--Wno-error=unknown"],
                    input=preprocessed,
                    text=True,
                    capture_output=True,
                    check=False
                )
                assert result.returncode == 0, f"clang-format returned {result.returncode}"

                preprocessed = result.stdout

            name = path.stem.split(".h", 1)[0]  # strip .h
            yield path.with_name(name).with_suffix(".h"), preprocessed
