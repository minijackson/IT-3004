boost_testing_dep = dependency('boost', modules: 'test')

matrix_graph_testing = executable('matrix_graph_testing',
                                  'matrix_graph_testing.cpp',
                                  include_directories: graph_inc,
                                  link_with: libgraph,
                                  dependencies: boost_testing_dep)

test('Matrix Graph testing', matrix_graph_testing, args: ['-l', 'test_suite'])

list_graph_testing = executable('list_graph_testing',
                                'list_graph_testing.cpp',
                                include_directories: graph_inc,
                                link_with: libgraph,
                                dependencies: boost_testing_dep)

test('List Graph testing', list_graph_testing, args: ['-l', 'test_suite'])

# vim: ft=cmake
