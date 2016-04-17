project('Graphe et Algorithmes', 'cpp', default_options: ['cpp_std=c++14'], license: 'MIT')

subdir('./src/graph')

subdir('./tests')

celero_dep = dependency('celero', fallback: ['celero', 'celero_dep'])

graph_dep = declare_dependency(include_directories: graph_inc, link_with: libgraph)

# vim: ft=cmake
