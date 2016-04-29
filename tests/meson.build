boost_testing_dep = dependency('boost', modules: 'test')

matrix_graph_testing = executable('matrix_graph_testing',
                                  'matrix_graph_testing.cpp',
                                  include_directories: graph_inc,
                                  link_with: libgraph,
                                  dependencies: boost_testing_dep)

list_graph_testing = executable('list_graph_testing',
                                'list_graph_testing.cpp',
                                include_directories: graph_inc,
                                link_with: libgraph,
                                dependencies: boost_testing_dep)

algorithms_testing = executable('algorithms_testing',
                                'algorithms_testing.cpp',
                                include_directories: graph_inc,
                                link_with: libgraph,
                                dependencies: boost_testing_dep)

printing_testing = executable('printing_testing',
                              'printing_testing.cpp',
                              include_directories: graph_inc,
                              link_with: libgraph,
                              dependencies: boost_testing_dep)

test('Matrix Graph testing', matrix_graph_testing, args: ['-l', 'test_suite'])
test('List Graph testing', list_graph_testing, args: ['-l', 'test_suite'])
test('Algorithms testing', algorithms_testing, args: ['-l', 'test_suite'])
test('Printing testing', printing_testing, args: ['-l', 'test_suite'])

graphviz = executable('graphviz',
                      'graphviz.cpp',
                      include_directories: graph_inc,
                      link_with: libgraph)
# vim: ft=cmake
