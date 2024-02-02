import sys
from pathlib import Path
import subprocess
import shutil

import pydot

DOT_BINARY = shutil.which("dot")
STD_HEADERS = ["concepts", "coroutine", "any", "bitset", "chrono", "compare", "csetjmp", "csignal", "cstdarg", 
               "cstddef", "cstdlib", "ctime", "debugging", "expected", "functional", "initializer_list", "optional", 
               "source_location", "tuple", "type_traits", "typeindex", "typeinfo", "utility", "variant", "version", 
               "memory", "memory_resource", "new", "scoped_allocator", "cfloat", "cinttypes", "climits", "cstdint", 
               "limits", "stdfloat", "cassert", "cerrno", "exception", "stacktrace", "stdexcept", "system_error", 
               "cctype", "charconv", "cstring", "cuchar", "cwchar", "cwctype", "format", "string", "string_view", 
               "array", "deque", "flat_map", "flat_set", "forward_list", "list", "map", "mdspan", "queue", "set", 
               "span", "stack", "unordered_map", "unordered_set", "vector", "iterator", "generator", "ranges", 
               "algorithm", "execution", "bit", "cfenv", "cmath", "complex", "linalg", "numbers", "numeric", 
               "random", "ratio", "valarray", "clocale", "codecvt", "locale", "text_encoding", "cstdio", "fstream", 
               "iomanip", "ios", "iosfwd", "iostream", "istream", "ostream", "print", "spanstream", "sstream", 
               "streambuf", "strstream", "syncstream", "filesystem", "regex", "atomic", "barrier", "condition_variable", 
               "future", "hazard_pointer", "latch", "mutex", "rcu", "semaphore", "shared_mutex", "stop_token", "thread"]

def patch(file: Path):
    assert file.exists(), "Dot file not found"
    graphs = pydot.graph_from_dot_file(file, 'utf-8')
    if not graphs or len(graphs) != 1:
        # nothing to do, invalid graph
        # file dependency graphs only have one graph per file
        return

    graph: pydot.Graph = graphs[0]
    name: str = graph.get_name()
    if not name.endswith('.h"') and name != '"repr"':
        # only care about header files
        # special case <repr>
        return

    print(f"Patching {file}")
    nodes: list[pydot.Node] = graph.get_node_list()
    edges: list[pydot.Edge] = graph.get_edge_list()

    def filter_edges(name: str):
        nonlocal edges
        nonlocal graph
        for edge in edges:
            points = [edge.get_source(), edge.get_destination()]
            if name not in points:
                continue
            graph.del_edge(*points)

    for node in nodes:
        attributes: dict[str, str] = node.get_attributes()
        if (url := attributes.get('URL')):
            if "cppreference" not in url:
                continue
        elif (label := attributes.get('label')):
            if label in STD_HEADERS:
                continue
        else:
            continue

        graph.del_node(node.get_name())
        filter_edges(node.get_name())

    graph.write_raw(file)


def main(file, *args):
    patch(Path(file))
    subprocess.check_call(' '.join([DOT_BINARY, file, *args]),
                          shell=True, stdout=sys.stdout, stderr=subprocess.STDOUT)


if __name__ == "__main__":
    main(*sys.argv[1:])
