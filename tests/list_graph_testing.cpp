#include "graph.hpp"
#include "algorithms.hpp"

#include <set>
#include <sstream>
#include <vector>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace graph;

BOOST_AUTO_TEST_CASE(empty_list_graph_creation) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph;
	BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 0);
}

BOOST_AUTO_TEST_CASE(list_graph_with_vertices_and_edges_creation) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	{
		Graph myGraph(std::pair<std::string, std::string>{"0", "0"});
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].front(), 0);
	}

	{
		std::set<std::pair<std::string, std::string>> arcs{
		        {"1", "3"}, {"1", "4"}, {"2", "7"}, {"8", "3"}};
		Graph myGraph(std::pair<std::string, std::string>{"1", "3"},
		              std::pair<std::string, std::string>{"1", "4"},
		              std::pair<std::string, std::string>{"2", "7"},
		              std::pair<std::string, std::string>{"8", "3"});

		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 6);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 2);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[1].size(), 0);

		for(auto const i : {"1", "2", "3", "4", "7", "8"}) {
			for(auto const j : {"1", "2", "3", "4", "7", "8"}) {
				size_t beginId = myGraph.getId(i), endId = myGraph.getId(j);
				auto nodeConnections = myGraph.getConnections()[beginId];
				if(arcs.find(std::pair<std::string, std::string>{i, j}) != arcs.end()) {
					BOOST_CHECK(std::find(nodeConnections.begin(), nodeConnections.end(), endId) !=
					            nodeConnections.end());
				} else {
					BOOST_CHECK(std::find(nodeConnections.begin(), nodeConnections.end(), endId) ==
					            nodeConnections.end());
				}
			}
		}
	}
}

BOOST_AUTO_TEST_CASE(list_graph_with_initializer_list) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	{
		Graph myGraph{{"0", "0"}};
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].front(), 0);
	}

	{
		std::initializer_list<std::pair<std::string, std::string>> arcs{
			    {"1", "3"}, {"1", "4"}, {"2", "7"}, {"8", "3"}};
		Graph myGraph(arcs);
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
		BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 6);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 2);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[1].size(), 0);

		BOOST_CHECK(myGraph.getConnections()[1].empty());

		for(const auto& arc : arcs) {
			auto endVertices = myGraph.getConnections()[myGraph.getId(arc.first)];
			BOOST_CHECK(std::find(endVertices.begin(),
			                      endVertices.end(),
			                      myGraph.getId(arc.second)) != endVertices.end());
		}
	}
}

BOOST_AUTO_TEST_CASE(list_graph_symmetric) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}}, expected{{"2", "1"}, {"4", "3"}, {"6", "5"}};

	BOOST_CHECK(graph::symmetric(myGraph) == expected);
}

BOOST_AUTO_TEST_CASE(list_graph_strongly_connected_component) {
	using Graph = list::Graph<NoProperty, NoProperty>;
	using ConstNode = Graph::ConstNode_t;

	const Graph myGraph{{"0", "1"},
	                    {"0", "5"},
	                    {"1", "2"},
	                    {"2", "5"},
	                    {"3", "1"},
	                    {"3", "2"},
	                    {"4", "2"},
	                    {"5", "1"},
	                    {"5", "3"},
	                    {"5", "4"}};
	std::set<ConstNode> expectedFor0{myGraph["0"]},
	        expectedForOthers{myGraph["1"], myGraph["2"], myGraph["3"], myGraph["4"], myGraph["5"]};

	BOOST_CHECK(graph::stronglyConnectedComponent(myGraph, myGraph["0"]) == expectedFor0);
	BOOST_CHECK(graph::stronglyConnectedComponent(myGraph, myGraph["1"]) == expectedForOthers);
	BOOST_CHECK(graph::stronglyConnectedComponent(myGraph, myGraph["2"]) == expectedForOthers);
	BOOST_CHECK(graph::stronglyConnectedComponent(myGraph, myGraph["5"]) == expectedForOthers);
}

BOOST_AUTO_TEST_CASE(list_graph_connected_component) {
	using Graph = list::Graph<NoProperty, NoProperty>;
	using ConstNode = Graph::ConstNode_t;

	const Graph myGraph{
		    {"0", "1"}, {"1", "2"}, {"2", "0"}, {"3", "4"}, {"4", "3"}, {"5", "6"}, {"7", "7"}};
	std::set<ConstNode> expectedFor0{myGraph["0"], myGraph["1"], myGraph["2"]},
	        expectedFor3{myGraph["3"], myGraph["4"]}, expectedFor5{myGraph["5"], myGraph["6"]},
	        expectedFor7{myGraph["7"]};

	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["0"]) == expectedFor0);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["1"]) == expectedFor0);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["2"]) == expectedFor0);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["3"]) == expectedFor3);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["4"]) == expectedFor3);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["5"]) == expectedFor5);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["6"]) == expectedFor5);
	BOOST_CHECK(graph::connectedComponent(myGraph, myGraph["7"]) == expectedFor7);
}

BOOST_AUTO_TEST_CASE(list_graph_get_id) {
	using Graph = list::Graph<NoProperty, NoProperty>;

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

BOOST_AUTO_TEST_CASE(list_graph_const_get_id) {
	using Graph = list::Graph<NoProperty, NoProperty>;

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

BOOST_AUTO_TEST_CASE(list_graph_subscript_operator) {
	using Graph = list::Graph<NoProperty, NoProperty>;
	using Node = Graph::Node_t;

	Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}};
	Node firstNode = myGraph["1"];

	BOOST_CHECK(myGraph.getConnections()[0] == firstNode.getConnections());
}

BOOST_AUTO_TEST_CASE(list_graph_equal_to_operator) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}},
		    myOtherGraph{{"4", "3"}, {"6", "5"}, {"2", "1"}},
		    myOtherOtherGraph{{"4", "3"}, {"6", "5"}, {"2", "0"}};

	BOOST_CHECK(myGraph == myOtherGraph);
	BOOST_CHECK(!(myGraph == myOtherOtherGraph));
}

BOOST_AUTO_TEST_CASE(list_graph_not_equal_to_operator) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}},
		    myOtherGraph{{"7", "6"}, {"4", "3"}, {"2", "1"}},
			myOtherOtherGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK(myGraph != myOtherGraph);
	BOOST_CHECK(!(myGraph != myOtherOtherGraph));
}

BOOST_AUTO_TEST_CASE(list_graph_add_node) {
	using Graph = list::Graph<WeightedProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	myGraph.addNode("Hello");
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 7);
	BOOST_CHECK_EQUAL(myGraph["Hello"].getProperty().weight, 0);
	myGraph.addNode("World", {5});
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
	BOOST_CHECK_EQUAL(myGraph["World"].getProperty().weight, 5);
}

BOOST_AUTO_TEST_CASE(list_graph_remove_node) {
	using Graph = list::Graph<WeightedProperty, NoProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 6);
	myGraph.removeNode(myGraph["6"]);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 5);
	// TODO
}

BOOST_AUTO_TEST_CASE(list_graph_add_edges) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	myGraph.addEdges({"Hello", "World"});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 4);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["Hello"], myGraph["World"]).weight, 0);
	myGraph.addEdges({"World", "Hello"});
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 5);
	BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 8);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["World"], myGraph["Hello"]).weight, 0);
}

BOOST_AUTO_TEST_CASE(list_graph_connect) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

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

BOOST_AUTO_TEST_CASE(list_graph_remove_edge) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 3);
	myGraph.removeEdge(myGraph["4"], myGraph["3"]);
	BOOST_CHECK_EQUAL(myGraph.getEdgesCount(), 2);
	// TODO
}

BOOST_AUTO_TEST_CASE(list_graph_set_edge_property) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["6"], myGraph["5"]).weight, 0);
	myGraph.setEdgeProperty(myGraph["6"], myGraph["5"], {1337});
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["6"], myGraph["5"]).weight, 1337);
}

BOOST_AUTO_TEST_CASE(list_graph_each_vertices) {
	using Graph     = list::Graph<NoProperty, WeightedProperty>;
	using ConstNode = list::Graph<NoProperty, WeightedProperty>::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	std::string expected = "654321";
	std::ostringstream result;

	myGraph.eachVertices([&result](ConstNode node) { result << node.getName(); });

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(list_graph_each_edges) {
	using Graph     = list::Graph<NoProperty, WeightedProperty>;
	using ConstNode = list::Graph<NoProperty, WeightedProperty>::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}};

	std::string expected = "6->5, 4->3, 2->1, ";
	std::ostringstream result;

	myGraph.eachEdges([&result](ConstNode begin, ConstNode end) {
		result << begin.getName() << "->" << end.getName() << ", ";
	});

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(list_graph_each_adjacents) {
	using Graph     = list::Graph<NoProperty, WeightedProperty>;
	using ConstNode = list::Graph<NoProperty, WeightedProperty>::ConstNode_t;

	Graph myGraph{{"6", "5"}, {"4", "3"}, {"2", "1"}, {"4", "2"}};

	std::string expected = "4->3, 4->2, ";
	std::ostringstream result;

	myGraph.eachAdjacents(myGraph["4"],
	                      [&result](ConstNode end) {
		                      result << "4->" << end.getName() << ", ";
		                  });

	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(list_node_get_id) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}};

	BOOST_CHECK_EQUAL(myGraph["0"].getId(), 0);
	BOOST_CHECK_EQUAL(myGraph["1"].getId(), 1);
	BOOST_CHECK_EQUAL(myGraph["2"].getId(), 2);
	BOOST_CHECK_EQUAL(myGraph["3"].getId(), 3);
}

BOOST_AUTO_TEST_CASE(list_node_const_get_id) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	const Graph myGraph{{"0", "0"}, {"1", "1"}, {"2", "2"}, {"3", "3"}};

	BOOST_CHECK_EQUAL(myGraph["0"].getId(), 0);
	BOOST_CHECK_EQUAL(myGraph["1"].getId(), 1);
	BOOST_CHECK_EQUAL(myGraph["2"].getId(), 2);
	BOOST_CHECK_EQUAL(myGraph["3"].getId(), 3);
}

BOOST_AUTO_TEST_CASE(list_node_is_connected_to) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"0", "0"}, {"1", "3"}, {"5", "7"}, {"2", "0"}};

	BOOST_CHECK(myGraph["0"].isConnectedTo(myGraph["0"]));
	BOOST_CHECK(!myGraph["0"].isConnectedTo(myGraph["1"]));
	BOOST_CHECK(myGraph["1"].isConnectedTo(myGraph["3"]));
}

BOOST_AUTO_TEST_CASE(list_graph_printing) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	std::string expected = "4 -> 5\n5 -> 2\n6 -> 3\n6 -> 4\n3 -> 3\n2 -> 4\n";
	std::ostringstream result;

	result << myGraph;
	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(list_graph_graphviz) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	std::string expected = "digraph myGraph {\n4 -> 5\n5 -> 2\n6 -> 3\n6 -> 4\n3 -> 3\n2 -> 4\n}\n";

	BOOST_CHECK_EQUAL(makeDigraph("myGraph", myGraph), expected);
}

BOOST_AUTO_TEST_CASE(list_weighted_graph) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	myGraph.setEdgeProperty(myGraph["2"], myGraph["4"], {5});

	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["0"], myGraph["1"]), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["0"], myGraph["2"]), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["3"], myGraph["2"]), std::out_of_range);
	BOOST_CHECK_THROW(myGraph.getEdgeProperty(myGraph["5"], myGraph["1"]), std::out_of_range);

	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["2"], myGraph["4"]).weight, 5);
}

BOOST_AUTO_TEST_CASE(list_weighted_graph_default_values) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	myGraph.setEdgeProperty(myGraph["2"], myGraph["4"], {5});

	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["4"], myGraph["5"]).weight, 0);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["6"], myGraph["3"]).weight, 0);
	BOOST_CHECK_EQUAL(myGraph.getEdgeProperty(myGraph["5"], myGraph["2"]).weight, 0);
}

BOOST_AUTO_TEST_CASE(list_weighted_graph_nonexistent_edge) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	BOOST_CHECK_THROW(myGraph.setEdgeProperty(myGraph["2"], myGraph["3"], {5}), std::out_of_range);
}
