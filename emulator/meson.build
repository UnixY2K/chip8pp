
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
    sdl2_dep,
    libcanvas_dep,
    cli11_dep,
]

# enable fmt dep if <format> is not available
if not cc.has_header('format')
    message('<format> not available, using fmt instead')
    emulator_deps += fmt_dep
endif

emulator = executable(
    'emulator',
    emulator_srcs,
    include_directories: emulator_incl,
    dependencies: emulator_deps,
)
