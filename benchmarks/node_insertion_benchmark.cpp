#include <celero/Celero.h>

#include "graph/graph.hpp"

#include <algorithm>

CELERO_MAIN

class NodeInsertionFixture : public celero::TestFixture {
public:
	using ListGraph           = graph::list::Graph<graph::NoProperty, graph::NoProperty>;
	using WeightedListGraph   = graph::list::Graph<graph::WeightedProperty, graph::NoProperty>;
	using MatrixGraph         = graph::matrix::Graph<graph::NoProperty, graph::NoProperty>;
	using WeightedMatrixGraph = graph::matrix::Graph<graph::WeightedProperty, graph::NoProperty>;

	NodeInsertionFixture() {}

	std::vector<std::pair<int64_t, uint64_t>> getExperimentValues() const override {
		std::vector<std::pair<int64_t, uint64_t>> nodeCounts;

		nodeCounts.push_back(std::pair<int64_t, uint64_t>(4, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(8, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(16, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(32, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(64, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(128, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(256, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(512, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(1024, 0));
		nodeCounts.push_back(std::pair<int64_t, uint64_t>(2048, 0));

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
	WeightedListGraph weightedListGraph;
	MatrixGraph matrixGraph;
	WeightedMatrixGraph weightedMatrixGraph;

	std::list<std::string> nodes;
};

BASELINE_F(NodeInsertion, ListGraph, NodeInsertionFixture, 1'000, 1) {
	for(std::string const& nodeName : nodes) {
		listGraph.addNode(nodeName);
	}
}

BENCHMARK_F(NodeInsertion, WeightedListGraph, NodeInsertionFixture, 1'000, 1) {
	for(std::string const& nodeName : nodes) {
		weightedListGraph.addNode(nodeName);
	}
}

BENCHMARK_F(NodeInsertion, MatrixGraph, NodeInsertionFixture, 1'000, 1) {
	for(std::string const& nodeName : nodes) {
		matrixGraph.addNode(nodeName);
	}
}

BENCHMARK_F(NodeInsertion, WeightedMatrixGraph, NodeInsertionFixture, 1'000, 1) {
	for(std::string const& nodeName : nodes) {
		weightedMatrixGraph.addNode(nodeName);
	}
}
