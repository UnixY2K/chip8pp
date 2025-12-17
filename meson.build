project('chip8pp',
        'cpp',
        default_options : [
            'cpp_std=c++20',
            'warning_level=3',
        ],
)

cc = meson.get_compiler('cpp')

sdl3_dep = dependency('sdl3', version: '>=3.2.10', fallback: ['sdl3', 'sdl3_dep'])
cli11_dep = dependency('cli11', version: '>=2.3.2', fallback: ['cli11', 'CLI11_dep'])

subdir('libcanvas')
subdir('emulator')
