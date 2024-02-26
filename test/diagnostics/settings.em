@include_path(".")
@include_path("../../include")

@# finalize - load the compilers
@load_defaults('c++')

@{
gcc = GCC(dialect='>=20')
clang = Clang(dialect='>=20')
clang16 = Clang(dialect='>=20', version='>=16')
}@