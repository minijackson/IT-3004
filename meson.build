project('Graphe et Algorithmes', 'cpp', default_options: ['cpp_std=c++14'], license: 'MIT')

celero_dep = dependency('celero', fallback: ['celero', 'celero_dep'])

subdir('./src/graph')

graph_dep_inc = include_directories('./src/')
graph_dep = declare_dependency(include_directories: graph_dep_inc, link_with: libgraph)

subdir('./tests')
subdir('./benchmarks')

# vim: ft=cmake
