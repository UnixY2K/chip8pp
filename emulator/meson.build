
emulator_incl = [
    include_directories('include'),
]
emulator_srcs = files(
    'src/cpu.cpp',
    'src/InstructionDecoder.cpp',
    'src/InstructionsImpl.cpp',
    'src/keypad.cpp',
    'src/memory.cpp',
    'src/source.cpp',
    'src/utils.cpp',
)
emulator_deps = [
    sdl3_dep,
    libcanvas_dep,
    cli11_dep,
]


executable(
    'emulator',
    emulator_srcs,
    include_directories: emulator_incl,
    dependencies: emulator_deps,
)
