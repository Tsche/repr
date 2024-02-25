import logging
import shutil  # required to find doxygen
import subprocess  # required to run doxygen

from pathlib import Path
from typing import Optional

from git import Repo  # required to clone stylesheets
from palgen import Extension, Model

doxygen_raw_path = shutil.which("doxygen")


class Docs(Extension):
    class Settings(Model):
        style_output: Path
        style_url: str
        style_tag: str

        extensions_output: Path
        extensions_url: str
        extensions_tag: str

        doxygen_path: Path = Path(doxygen_raw_path) if doxygen_raw_path is not None else Path()
        doxyfile: Path = Path("docs") / "Doxyfile"

    def expand_path(self, path: Path):
        return path if path.is_absolute() else self.root_path / path

    def clone_repo(self, output: Path, url: str, tag: str):
        output = self.expand_path(output)
        if not output.exists():
            logging.info("Cloning %s into %s", url, output)
            repo = Repo.clone_from(url, output)
        else:
            repo = Repo(output)
            if not repo.head.is_detached:
                logging.info("Updating %s", output)
                repo.git.pull()

        logging.info("Checking out %s", tag)
        repo.git.checkout(tag)

    def clone_style(self):
        self.clone_repo(self.settings.style_output,
                        self.settings.style_url, self.settings.style_tag)

    def clone_extensions(self):
        self.clone_repo(self.settings.extensions_output,
                        self.settings.extensions_url, self.settings.extensions_tag)

    def run(self, files: list, jobs: Optional[int] = None) -> list:
        self.clone_style()
        self.clone_extensions()
        self.settings.doxyfile = self.expand_path(self.settings.doxyfile)

        subprocess.check_call(
            [self.settings.doxygen_path, self.settings.doxyfile], cwd=self.root_path)
