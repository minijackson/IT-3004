#include <celero/Celero.h>

#include "graph/graph.hpp"

CELERO_MAIN

class EdgeInsertionFixture : public celero::TestFixture {
public:
	using ListGraph           = graph::list::Graph<graph::NoProperty, graph::NoProperty>;
	using WeightedListGraph   = graph::list::Graph<graph::NoProperty, graph::WeightedProperty>;
	using MatrixGraph         = graph::matrix::Graph<graph::NoProperty, graph::NoProperty>;
	using WeightedMatrixGraph = graph::matrix::Graph<graph::NoProperty, graph::WeightedProperty>;

	EdgeInsertionFixture() {}

	std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
		std::vector<std::pair<int64_t, uint64_t>> nodeCounts;

		nodeCounts.push_back(std::pair<int64_t, uint64_t>(4, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(8, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(16, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(32, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(64, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(128, 0));

		return nodeCounts;
	}

	void setUp(int64_t experimentValue) override {
		listGraph           = ListGraph();
		weightedListGraph   = WeightedListGraph();
		matrixGraph         = MatrixGraph();
		weightedMatrixGraph = WeightedMatrixGraph();

		size_t name;
		std::generate_n(std::back_inserter(nodes),
		                experimentValue,
		                [&name]() { return std::to_string(name++); });

	}

	ListGraph listGraph;
	ListGraph listGraph1;
	WeightedListGraph weightedListGraph;
	WeightedListGraph weightedListGraph1;
	MatrixGraph matrixGraph;
	MatrixGraph matrixGraph1;
	WeightedMatrixGraph weightedMatrixGraph;
	WeightedMatrixGraph weightedMatrixGraph1;

	std::list<std::string> nodes;
};

BASELINE_F(EdgeInsertion, ListGraphAddEdge, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			listGraph.addEdges({startName, endName});
		}
	}
}

BENCHMARK_F(EdgeInsertion, ListGraphConnect, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			listGraph1.connect(listGraph1[startName], listGraph1[endName]);
		}
	}
}

BENCHMARK_F(EdgeInsertion, WeightedListGraphAddEdge, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			weightedListGraph.addEdges({startName, endName});
		}
	}
}

BENCHMARK_F(EdgeInsertion, WeightedListGraphConnect, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			weightedListGraph1.connect(weightedListGraph1[startName], weightedListGraph1[endName]);
		}
	}
}

BENCHMARK_F(EdgeInsertion, MatrixGraphAddEdge, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			matrixGraph.addEdges({startName, endName});
		}
	}
}

BENCHMARK_F(EdgeInsertion, MatrixGraphConnect, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			matrixGraph1.connect(matrixGraph1[startName], matrixGraph1[endName]);
		}
	}
}

BENCHMARK_F(EdgeInsertion, WeightedMatrixGraphAddEdge, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			weightedMatrixGraph.addEdges({startName, endName});
		}
	}
}

BENCHMARK_F(EdgeInsertion, WeightedMatrixGraphConnect, EdgeInsertionFixture, 30, 100) {
	for(std::string const& startName : nodes) {
		for(std::string const& endName : nodes) {
			weightedMatrixGraph1.connect(weightedMatrixGraph1[startName],
			                             weightedMatrixGraph1[endName]);
		}
	}
}
