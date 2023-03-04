project('chip8pp',
        'cpp',
        default_options : [
            'cpp_std=c++20',
            'warning_level=3',
        ],
)

cc = meson.get_compiler('cpp')

sdl2_dep = dependency('sdl2', version: '>=2.0.0', fallback: ['sdl2', 'sdl2_dep'])
cli11_dep = dependency('cli11', version: '>=2.3.2', fallback: ['cli11', 'CLI11_dep'])
fmt_dep = dependency('fmt', version: '>=8.0.1', fallback: ['fmt', 'fmt_dep'])

subdir('libcanvas')
subdir('emulator')
