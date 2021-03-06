#include "graph.hpp"
#include "algorithms.hpp"

#include <set>
#include <sstream>
#include <tuple>
#include <vector>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace graph;

BOOST_AUTO_TEST_CASE(empty_matrix_graph_creation) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;
	Graph myGraph;
	BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 0);
}

BOOST_AUTO_TEST_CASE(matrix_graph_with_vertices_and_edges_creation) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;
	using Edge  = Graph::Edge_t;

	{
		Graph myGraph(Edge{"0", "0"});
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], true);
	}

	{
		std::set<std::pair<std::string, std::string>> arcs{
		        {"1", "3"}, {"1", "4"}, {"2", "7"}, {"8", "3"}};
		Graph myGraph(Edge{"1", "3"}, Edge{"1", "4"}, Edge{"2", "7"}, Edge{"8", "3"});

		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 6);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 6);

		for(auto const i : {"1", "2", "3", "4", "7", "8"}) {
			for(auto const j : {"1", "2", "3", "4", "7", "8"}) {
				size_t beginId = myGraph.getId(i), endId = myGraph.getId(j);
				if(arcs.find(std::pair<std::string, std::string>{i, j}) != arcs.end()) {
					BOOST_CHECK_EQUAL(myGraph.getConnections()[beginId][endId], true);
				} else {
					BOOST_CHECK_EQUAL(myGraph.getConnections()[beginId][endId], false);
				}
			}
		}

	}
}

BOOST_AUTO_TEST_CASE(matrix_graph_with_initializer_list) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	{
		Graph myGraph{{"0", "0"}};
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], true);
	}

	{
		std::initializer_list<Graph::Edge_t> arcs{{"1", "3"}, {"1", "4"}, {"2", "7"}, {"8", "3"}};
		Graph myGraph(arcs);
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 6);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 6);

		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], false);

		for(const auto& arc : arcs) {
			size_t beginId = myGraph.getId(std::get<0>(arc)),
			       endId = myGraph.getId(std::get<1>(arc));
			BOOST_CHECK_EQUAL(myGraph.getConnections()[beginId][endId], true);
		}
	}
}

BOOST_AUTO_TEST_CASE(matrix_graph_get_id) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getId("1"), 5);
	BOOST_CHECK_EQUAL(myGraph.getId("2"), 4);
	BOOST_CHECK_EQUAL(myGraph.getId("3"), 3);
	BOOST_CHECK_EQUAL(myGraph.getId("4"), 2);
	BOOST_CHECK_EQUAL(myGraph.getId("5"), 1);
	BOOST_CHECK_EQUAL(myGraph.getId("6"), 0);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
	BOOST_CHECK_EQUAL(myGraph.getId("7"), 6);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 7);
	BOOST_CHECK_EQUAL(myGraph.getId("8"), 7);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
}

BOOST_AUTO_TEST_CASE(matrix_graph_const_get_id) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	const Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getId("1"), 5);
	BOOST_CHECK_EQUAL(myGraph.getId("2"), 4);
	BOOST_CHECK_EQUAL(myGraph.getId("3"), 3);
	BOOST_CHECK_EQUAL(myGraph.getId("4"), 2);
	BOOST_CHECK_EQUAL(myGraph.getId("5"), 1);
	BOOST_CHECK_EQUAL(myGraph.getId("6"), 0);
	BOOST_CHECK_THROW(myGraph.getId("7"), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getId("8"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(matrix_graph_subscript_operator) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;
	using Node = Graph::Node_t;

	Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}};
	Node firstNode = myGraph["1"];

	BOOST_CHECK(myGraph.getConnections()[0] == firstNode.getConnections());
}

BOOST_AUTO_TEST_CASE(matrix_graph_const_subscript_operator) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;
	using Node = Graph::ConstNode_t;

	const Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}};
	Node firstNode = myGraph["1"];

	BOOST_CHECK(myGraph.getConnections()[0] == firstNode.getConnections());
}

BOOST_AUTO_TEST_CASE(matrix_graph_begin) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;
	using Node = Graph::Node_t;

	Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}};
	Node firstNode = myGraph["1"];

	BOOST_CHECK(myGraph.begin() == firstNode);
}

BOOST_AUTO_TEST_CASE(matrix_graph_const_begin) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;
	using Node = Graph::ConstNode_t;

	const Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}};
	Node firstNode = myGraph["1"];

	BOOST_CHECK(myGraph.begin() == firstNode);
}

BOOST_AUTO_TEST_CASE(matrix_graph_equal_to_operator) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}},
		    sameGraph{{"4", "3"}, {"6", "5"}, {"2", "1"}},
		    differentEdges{{"4", "3"}, {"6", "1"}, {"2", "5"}},
		    differentNodeCount{{"5", "3"}, {"6", "5"}, {"2", "0"}},
		    differentNodeNames{{"a", "b"}, {"c", "d"}, {"e", "f"}};

	BOOST_CHECK(myGraph == sameGraph);
	BOOST_CHECK(!(myGraph == differentEdges));
	BOOST_CHECK(!(myGraph == differentNodeCount));
	BOOST_CHECK(!(myGraph == differentNodeNames));
}

BOOST_AUTO_TEST_CASE(matrix_graph_not_equal_to_operator) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}},
		    sameGraph{{"4", "3"}, {"6", "5"}, {"2", "1"}},
		    differentEdges{{"4", "3"}, {"6", "1"}, {"2", "5"}},
		    differentNodeCount{{"5", "3"}, {"6", "5"}, {"2", "0"}},
		    differentNodeNames{{"a", "b"}, {"c", "d"}, {"e", "f"}};

	BOOST_CHECK(!(myGraph != sameGraph));
	BOOST_CHECK(myGraph != differentEdges);
	BOOST_CHECK(myGraph != differentNodeCount);
	BOOST_CHECK(myGraph != differentNodeNames);
}

BOOST_AUTO_TEST_CASE(matrix_graph_has_node) {
	using Graph = matrix::Graph<WeightedProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK(myGraph.hasNode("6"));
	BOOST_CHECK(myGraph.hasNode("5"));
	BOOST_CHECK(myGraph.hasNode("4"));
	BOOST_CHECK(myGraph.hasNode("3"));
	BOOST_CHECK(myGraph.hasNode("2"));
	BOOST_CHECK(myGraph.hasNode("1"));

	BOOST_CHECK(!myGraph.hasNode("7"));
	BOOST_CHECK(!myGraph.hasNode("42"));
	BOOST_CHECK(!myGraph.hasNode("1337"));
}

BOOST_AUTO_TEST_CASE(matrix_graph_add_node) {
	using Graph = matrix::Graph<WeightedProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	myGraph.addNode("Hello");
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 7);
	BOOST_CHECK_EQUAL(myGraph["Hello"].getProperty().weight, 0);
	myGraph.addNode("World", {5});
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
	BOOST_CHECK_EQUAL(myGraph["World"].getProperty().weight, 5);
}

BOOST_AUTO_TEST_CASE(matrix_graph_remove_node) {
	using Graph = matrix::Graph<WeightedProperty, NoProperty>;
	using ConstNode = Graph::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}, {"5", "6"}};

	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
	myGraph.removeNode(myGraph["6"]);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 5);
	BOOST_CHECK(!myGraph.hasNode("6"));

	std::ostringstream result;
	std::string expected = "2->1, 4->3, ";
	myGraph.eachEdges([&result](ConstNode begin, ConstNode end) {
		result << begin.getName() << "->" << end.getName() << ", ";
	});

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(matrix_graph_has_edge) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK(myGraph.hasEdge(myGraph["6"], myGraph["5"]));
	BOOST_CHECK(myGraph.hasEdge(myGraph["4"], myGraph["3"]));
	BOOST_CHECK(myGraph.hasEdge(myGraph["2"], myGraph["1"]));

	BOOST_CHECK(!myGraph.hasEdge(myGraph["1"], myGraph["2"]));
	BOOST_CHECK(!myGraph.hasEdge(myGraph["1"], myGraph["3"]));
	BOOST_CHECK(!myGraph.hasEdge(myGraph["6"], myGraph["4"]));
}

BOOST_AUTO_TEST_CASE(matrix_graph_add_edges) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;
	using Edge  = Graph::Edge_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	myGraph.addEdges({"Hello", "World"});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["Hello"], myGraph["World"]).weight, 0);

	myGraph.addEdges({"World", "Hello", WeightedProperty{5}});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 5);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["World"], myGraph["Hello"]).weight, 5);

	myGraph.addEdges(Edge{"Goodbye", "World", WeightedProperty{5}}, Edge{"World", "Goodbye"});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 7);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 9);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["Goodbye"], myGraph["World"]).weight, 5);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["World"], myGraph["Goodbye"]).weight, 0);

	myGraph.addEdges({{"foo", "bar", {5}}, {"bar", "foo"}});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 9);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 11);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["foo"], myGraph["bar"]).weight, 5);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["bar"], myGraph["foo"]).weight, 0);
}

BOOST_AUTO_TEST_CASE(matrix_graph_connect) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	myGraph.connect(myGraph["3"], myGraph["2"]);
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["3"], myGraph["2"]).weight, 0);
	myGraph.connect(myGraph["5"], myGraph["4"], {42});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 5);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["5"], myGraph["4"]).weight, 42);
}

BOOST_AUTO_TEST_CASE(matrix_graph_remove_edge) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 3);
	myGraph.removeEdge(myGraph["4"], myGraph["3"]);
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 2);
	BOOST_CHECK(!myGraph.hasEdge(myGraph["4"], myGraph["3"]));
	BOOST_CHECK_THROW(myGraph.removeEdge(myGraph["5"], myGraph["4"]), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(matrix_graph_set_edge_property) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["6"], myGraph["5"]).weight, 0);
	myGraph.setEdgeProperty(myGraph["6"], myGraph["5"], {1337});
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["6"], myGraph["5"]).weight, 1337);
}

BOOST_AUTO_TEST_CASE(matrix_graph_each_vertices) {
	using Graph     = matrix::Graph<NoProperty, WeightedProperty>;
	using ConstNode = matrix::Graph<NoProperty, WeightedProperty>::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	std::string expected = "123456";
	std::ostringstream result;

	myGraph.eachVertices([&result](ConstNode node) { result << node.getName(); });

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(matrix_graph_each_edges) {
	using Graph     = matrix::Graph<NoProperty, WeightedProperty>;
	using ConstNode = matrix::Graph<NoProperty, WeightedProperty>::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	std::string expected = "2->1, 4->3, 6->5, ";
	std::ostringstream result;

	myGraph.eachEdges([&result](ConstNode begin, ConstNode end) {
		result << begin.getName() << "->" << end.getName() << ", ";
	});

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(matrix_graph_each_adjacents) {
	using Graph     = matrix::Graph<NoProperty, WeightedProperty>;
	using ConstNode = matrix::Graph<NoProperty, WeightedProperty>::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}, {"4", "2"}};

	std::string expected = "4->2, 4->3, ";
	std::ostringstream result;

	myGraph.eachAdjacents(myGraph["4"],
	                      [&result](ConstNode end) {
		                      result << "4->" << end.getName() << ", ";
		                  });

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(matrix_node_get_id) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}};

	BOOST_CHECK_EQUAL(myGraph["0"].getId(), 0);
	BOOST_CHECK_EQUAL(myGraph["1"].getId(), 1);
	BOOST_CHECK_EQUAL(myGraph["2"].getId(), 2);
	BOOST_CHECK_EQUAL(myGraph["3"].getId(), 3);
}

BOOST_AUTO_TEST_CASE(matrix_node_const_get_id) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	const Graph myGraph{{"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}};

	BOOST_CHECK_EQUAL(myGraph["0"].getId(), 0);
	BOOST_CHECK_EQUAL(myGraph["1"].getId(), 1);
	BOOST_CHECK_EQUAL(myGraph["2"].getId(), 2);
	BOOST_CHECK_EQUAL(myGraph["3"].getId(), 3);
}

BOOST_AUTO_TEST_CASE(matrix_node_is_connected_to) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"0", "0"}, {"1", "3"}, {"5", "7"}, {"2", "0"}};

	BOOST_CHECK(myGraph["0"].isConnectedTo(myGraph["0"]));
	BOOST_CHECK(!myGraph["0"].isConnectedTo(myGraph["1"]));
	BOOST_CHECK(myGraph["1"].isConnectedTo(myGraph["3"]));
}

BOOST_AUTO_TEST_CASE(matrix_weighted_graph) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	myGraph.setEdgeProperty(myGraph["2"], myGraph["4"], {5});

	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["0"], myGraph["1"]), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["0"], myGraph["2"]), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["3"], myGraph["2"]), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["5"], myGraph["1"]), std::out_of_range);

	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["2"], myGraph["4"]).weight, 5);
}

BOOST_AUTO_TEST_CASE(matrix_weighted_graph_default_values) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	myGraph.setEdgeProperty(myGraph["2"], myGraph["4"], {5});

	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["4"], myGraph["5"]).weight, 0);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["6"], myGraph["3"]).weight, 0);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["5"], myGraph["2"]).weight, 0);
}

BOOST_AUTO_TEST_CASE(matrix_weighted_graph_nonexistent_edge) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	BOOST_CHECK_THROW(myGraph.setEdgeProperty(myGraph["2"], myGraph["3"], {5}), std::out_of_range);
}
