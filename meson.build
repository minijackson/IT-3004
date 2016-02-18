project('Graphe et Algorithmes', 'cpp', default_options: ['cpp_std=c++14'], license: 'MIT')

add_global_arguments(['-fdiagnostics-color=always', '-Wno-c++98-compat', '-Wno-c++98-compat-pedantic', '-Wno-padded', '-Wno-shadow'], language: 'cpp')

subdir('./src/graph')

subdir('./tests')

celero_dep = dependency('celero', fallback: ['celero', 'celero_dep'])

# vim: ft=cmake
