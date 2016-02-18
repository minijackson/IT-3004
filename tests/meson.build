boost_testing_dep = dependency('boost', modules: 'test')

graph_testing = executable('graph_testing', 'graph_testing.cpp',
                           include_directories: graph_inc,
                           link_with: libgraph,
                           dependencies: boost_testing_dep)

test('Graph testing', graph_testing, args: ['-l', 'test_suite'])

# vim: ft=cmake
