import re
import logging
from pathlib import Path
from enum import Enum, auto

from palgen.ingest import Suffix, Absolute
from palgen import Extension, Sources, Model

INCLUDE_MARKER = "#include "


class IncludeKind(Enum):
    SYSTEM = auto()
    RELATIVE = auto()
    LOCAL = auto()


class Header:
    def __init__(self, root: Path, project_name: str):
        self.root = root
        self.project_name = project_name
        self.system_includes: set[str] = set()

        # insertion order might matter, in Python lists keep insertion order but sets do not
        self.local_includes: list[Path] = []
        self.seen: set[Path] = set()

        self.amalgamated: str = ""
        # recursively search include paths
        self.get_includes(self.root)

    def classify_include(self, include: str):
        filename = include[1:-1]

        if include.startswith('"'):
            return IncludeKind.RELATIVE
        elif filename.startswith(self.project_name):
            return IncludeKind.LOCAL

        return IncludeKind.SYSTEM

    def resolve_include(self, include: str, origin: Path):
        filename = include[1:-1]

        kind = self.classify_include(include)
        assert kind != IncludeKind.SYSTEM
        return (self.root.parent if kind == IncludeKind.LOCAL else origin.parent) / filename

    def get_includes(self, path: Path, pp_depth = 0):
        if path in self.seen:
            return
        self.seen.add(path)
        logging.debug("Analyzing %s", path)
        self.local_includes.append(path)  # guaranteed to be unique

        text = path.read_text(encoding='utf-8')

        for line in text.split('\n'):
            stripped = line.strip()
            if pp_depth != 0:
                if stripped.startswith(INCLUDE_MARKER):
                    include = stripped[len(INCLUDE_MARKER):]
                    if self.classify_include(include) != IncludeKind.SYSTEM:
                        # dump relative imports
                        self.get_includes(self.resolve_include(include, path))
                        continue
                elif re.match(r"#\s*endif", stripped):
                    pp_depth -= 1

            if re.match(r"#\s*if", stripped):
                pp_depth += 1
            elif stripped.startswith("#pragma once"):
                continue
            elif pp_depth == 0 and stripped.startswith(INCLUDE_MARKER):
                include = stripped[len(INCLUDE_MARKER):]
                if include.startswith("<fmt"):
                    print(pp_depth)
                    print(f"found include {include}")
                    print(path)
                if self.classify_include(include) != IncludeKind.SYSTEM:
                    self.get_includes(self.resolve_include(include, path))
                else:
                    self.system_includes.add(stripped)
                continue
            self.amalgamated += line + '\n'


class Amalgamate(Extension):
    ingest = Sources / 'include' >> Suffix(".hpp", ".h") >> Absolute

    class Settings(Model):
        root: Path
        filename: str

    def find_root(self, *_):
        if self.settings.root.is_absolute():
            return self.settings.root

        for sourcedir in self.project.sources:
            if (file := sourcedir / self.settings.root).exists():
                return file

    def render(self, data: list[tuple[Path]]):
        header = Header(self.find_root(), self.project.name)
        system_includes = '\n'.join(header.system_includes)
        amalgamated = f"""\
#ifndef REPR_INCLUDED
#define REPR_INCLUDED
{system_includes}
{header.amalgamated}
#endif
"""
        for file in data:
            if file not in header.seen:
                logging.warning("File %s was not included anywhere.", file)

        yield self.settings.filename, amalgamated
