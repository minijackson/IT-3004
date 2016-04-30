node_insertion_benchmark = executable('node_insertion_benchmark', 'node_insertion_benchmark.cpp',
	dependencies: [graph_dep,celero_dep])

edge_insertion_benchmark = executable('edge_insertion_benchmark', 'edge_insertion_benchmark.cpp',
	dependencies: [graph_dep,celero_dep])

benchmark('Node insertion', node_insertion_benchmark, args: ['-t', 'node_insertion_benchmark.csv'])
benchmark('Edge insertion', edge_insertion_benchmark, args: ['-t', 'edge_insertion_benchmark.csv'])
