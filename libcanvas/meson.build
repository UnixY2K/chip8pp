
libcanvas_incl = include_directories(
    'include',
)
libcanvas_srcs = files(
    'src/screen.cpp',
    'src/grid.cpp',
)
libcanvas_deps = [
    sdl2_dep,
]

# make a static library
libcanvas = static_library(
    'libcanvas',
    libcanvas_srcs,
    dependencies: libcanvas_deps,
    include_directories: libcanvas_incl,
)

# make a dependency object
libcanvas_dep = declare_dependency(
    link_with: libcanvas,
    include_directories: libcanvas_incl,
)

# make an example executable
example_srcs = files(
    'example/source.cpp',
)
example_deps = [
    libcanvas_dep,
    sdl2_dep,
]
example = executable(
    'example',
    example_srcs,
    dependencies: example_deps,
)
