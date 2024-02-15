import logging
import shutil  # required to find doxygen
import subprocess  # required to run doxygen

from pathlib import Path
from typing import Optional

from git import Repo # required to clone stylesheets
from palgen import Extension, Model


class Docs(Extension):
    class Settings(Model):
        doxygen_path: Path = Path(shutil.which("doxygen"))
        doxyfile: Path = Path("docs") / "Doxyfile"

        style_output: Path = Path("build") / "style"
        style_url: str = "https://github.com/jothepro/doxygen-awesome-css.git"
        style_tag: str = "v2.3.1"

        extensions_output: Path = Path("build") / "extensions"
        extensions_url: str = "https://github.com/Tsche/doxygen-utils.git"
        extensions_tag: str = "master"

    def expand_path(self, path: Path):
        return path if path.is_absolute() else self.root_path / path

    def clone_repo(self, output: Path, url: str, tag: str):
        output = self.expand_path(output)
        if not output.exists():
            logging.info(f"Cloning {url} into {output}")
            repo = Repo.clone_from(url, output)
        else:
            repo = Repo(output)
            # logging.info(f"Fetching {output}")
            # repo.git.fetch()

        logging.info(f"Checking out {tag}")
        repo.git.checkout(tag)

    def clone_style(self):
        self.clone_repo(self.settings.style_output, self.settings.style_url, self.settings.style_tag)

    def clone_extensions(self):
        self.clone_repo(self.settings.extensions_output, self.settings.extensions_url, self.settings.extensions_tag)

    def run(self, files: list, jobs: Optional[int] = None) -> list:
        self.clone_style()
        self.clone_extensions()
        self.settings.doxyfile = self.expand_path(self.settings.doxyfile)

        subprocess.check_call([self.settings.doxygen_path, self.settings.doxyfile], cwd=self.root_path)
