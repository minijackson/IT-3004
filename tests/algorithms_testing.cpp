#include "graph.hpp"
#include "algorithms.hpp"
#include "printing.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace graph;

BOOST_AUTO_TEST_CASE(algorithms_symmetric) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}}, expected{{"2", "1"}, {"4", "3"}, {"6", "5"}};

	BOOST_CHECK(graph::symmetric(myGraph) == expected);
}

BOOST_AUTO_TEST_CASE(algorithms_undirected) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"1", "2"}, {"3", "4"}, {"5", "6"}},
	        expected{{"1", "2"}, {"2", "1"}, {"3", "4"}, {"4", "3"}, {"5", "6"}, {"6", "5"}};

	BOOST_CHECK(graph::undirected(myGraph) == expected);
}

BOOST_AUTO_TEST_CASE(algorithms_strongly_connected_component) {
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

BOOST_AUTO_TEST_CASE(algorithms_connected_component) {
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

BOOST_AUTO_TEST_CASE(algorithms_minimum_spanning_tree) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph;
	for(std::string i : {"0", "1", "2", "3", "4"}) {
		myGraph.addNode(i);
	}

	myGraph.connect(myGraph["0"], myGraph["1"], {9});
	myGraph.connect(myGraph["0"], myGraph["2"], {75});

	myGraph.connect(myGraph["1"], myGraph["2"], {95});
	myGraph.connect(myGraph["1"], myGraph["3"], {19});
	myGraph.connect(myGraph["1"], myGraph["4"], {42});

	myGraph.connect(myGraph["2"], myGraph["3"], {51});

	myGraph.connect(myGraph["3"], myGraph["4"], {31});

	Graph myUndirectedGraph = graph::undirected(myGraph);

	Graph directedExpected;
	for(std::string i : {"0", "1", "2", "3", "4"}) {
		myGraph.addNode(i);
	}

	directedExpected.connect(directedExpected["0"], directedExpected["1"], {9});
	directedExpected.connect(directedExpected["1"], directedExpected["3"], {19});
	directedExpected.connect(directedExpected["3"], directedExpected["2"], {51});
	directedExpected.connect(directedExpected["3"], directedExpected["4"], {31});

	Graph expected = graph::undirected(directedExpected);

	BOOST_CHECK(graph::minimumSpanningTree(myUndirectedGraph, myUndirectedGraph["0"]) == expected);

	Graph result = graph::minimumSpanningTree(myUndirectedGraph, myUndirectedGraph["0"]);
}
