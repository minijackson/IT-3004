#include "graph.hpp"

#include <iostream>
#include <set>
#include <vector>

#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(empty_matrix_graph_creation) {
	using matrix::Graph;
	Graph myGraph;
	BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 0);
}

BOOST_AUTO_TEST_CASE(matrix_graph_with_vertices_creation) {
	using matrix::Graph;

	{
		Graph myGraph(1);
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 0);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], false);
	}

	{
		Graph myGraph(10);
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 10);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 0);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 10);
		for(size_t i = 0; i < 10; ++i) {
			BOOST_CHECK_EQUAL(myGraph.getConnections()[i].size(), 10);
			for(size_t j = 0; j < 10; ++j) {
				BOOST_CHECK_EQUAL(myGraph.getConnections()[i][j], false);
			}
		}
	}

	{
		Graph myGraph(100);
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 100);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 0);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 100);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[42].size(), 100);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[42][69], false);
	}
}

BOOST_AUTO_TEST_CASE(matrix_graph_with_vertices_and_edges_creation) {
	using matrix::Graph;

	{
		Graph myGraph(1, std::pair<int,int>{0, 0});
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], true);
	}

	{
		std::set<std::pair<int, int>> arcs{{1, 3}, {1, 4}, {2, 7}, {8, 3}};
		Graph myGraph(10,
		              std::pair<int, int>{1, 3},
		              std::pair<int, int>{1, 4},
		              std::pair<int, int>{2, 7},
		              std::pair<int, int>{8, 3});

		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 10);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 4);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 10);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 10);

		for(size_t i = 0; i < 10; ++i) {
			for(size_t j = 0; j < 10; ++j) {
				if(arcs.find(std::pair<int, int>{i, j}) != arcs.end()) {
					BOOST_CHECK_EQUAL(myGraph.getConnections()[i][j], true);
				} else {
					BOOST_CHECK_EQUAL(myGraph.getConnections()[i][j], false);
				}
			}
		}

	}
}

BOOST_AUTO_TEST_CASE(matrix_graph_with_initializer_list) {
	using matrix::Graph;

	{
		Graph myGraph{{0,0}};
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 1);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], true);
	}

	{
		std::initializer_list<std::pair<size_t, size_t>> arcs{{1, 3}, {1, 4}, {2, 7}, {8, 3}};
		Graph myGraph(arcs);
		BOOST_CHECK_EQUAL(myGraph.getVerticesCount(), 9);
		BOOST_CHECK_EQUAL(myGraph.getArcsCount(), 4);
		BOOST_CHECK_EQUAL(myGraph.getConnections().size(), 9);
		BOOST_CHECK_EQUAL(myGraph.getConnections()[0].size(), 9);

		BOOST_CHECK_EQUAL(myGraph.getConnections()[0][0], false);

		for(const auto& arc : arcs) {
			BOOST_CHECK_EQUAL(myGraph.getConnections()[arc.first][arc.second], true);
		}
	}
}

BOOST_AUTO_TEST_CASE(matrix_graph_symmetric) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{1, 2}, {3, 4}, {5, 6}}, expected{{2, 1}, {4, 3}, {6, 5}};

	BOOST_CHECK(myGraph.symmetric() == expected);
}

BOOST_AUTO_TEST_CASE(matrix_graph_strongly_connected_component) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{0, 1}, {0, 5}, {1, 2}, {2, 5}, {3, 1}, {3, 2}, {4, 2}, {5, 1}, {5, 3}, {5, 4}};
	std::set<size_t> expectedFor0{0}, expectedForOthers{1, 2, 3, 4, 5};

	BOOST_CHECK(myGraph.stronglyConnectedComponent(0) == expectedFor0);
	BOOST_CHECK(myGraph.stronglyConnectedComponent(1) == expectedForOthers);
	BOOST_CHECK(myGraph.stronglyConnectedComponent(2) == expectedForOthers);
	BOOST_CHECK(myGraph.stronglyConnectedComponent(5) == expectedForOthers);
}

BOOST_AUTO_TEST_CASE(matrix_graph_connected_component) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{0, 1}, {1, 2}, {2, 0}, {3, 4}, {4, 3}, {5, 6}, {7, 7}};
	std::set<size_t> expectedFor0{0, 1, 2}, expectedFor3{3, 4}, expectedFor5{5, 6}, expectedFor7{7};

	BOOST_CHECK(myGraph.connectedComponent(0) == expectedFor0);
	BOOST_CHECK(myGraph.connectedComponent(1) == expectedFor0);
	BOOST_CHECK(myGraph.connectedComponent(2) == expectedFor0);
	BOOST_CHECK(myGraph.connectedComponent(3) == expectedFor3);
	BOOST_CHECK(myGraph.connectedComponent(4) == expectedFor3);
	BOOST_CHECK(myGraph.connectedComponent(5) == expectedFor5);
	BOOST_CHECK(myGraph.connectedComponent(6) == expectedFor5);
	BOOST_CHECK(myGraph.connectedComponent(7) == expectedFor7);
}

BOOST_AUTO_TEST_CASE(matrix_graph_subscript_operator) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{1, 2}, {3, 4}, {5, 6}};
	Node firstNode = myGraph[0];

	BOOST_CHECK(myGraph.getConnections() == firstNode.getConnections());
}

BOOST_AUTO_TEST_CASE(matrix_graph_equal_to_operator) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{6, 5}, {4, 3}, {2, 1}}, myOtherGraph{{6, 5}, {4, 3}, {2, 1}};

	BOOST_CHECK(myGraph == myOtherGraph);
}

BOOST_AUTO_TEST_CASE(matrix_graph_not_equal_to_operator) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{6, 5}, {4, 3}, {2, 1}}, myOtherGraph{{7, 6}, {4, 3}, {2, 1}};

	BOOST_CHECK(myGraph != myOtherGraph);
}

BOOST_AUTO_TEST_CASE(matrix_node_get_id) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{0, 0}, {1, 1}, {2, 2}, {3, 3}};

	BOOST_CHECK_EQUAL(myGraph[0].getId(), 0);
	BOOST_CHECK_EQUAL(myGraph[1].getId(), 1);
	BOOST_CHECK_EQUAL(myGraph[2].getId(), 2);
	BOOST_CHECK_EQUAL(myGraph[3].getId(), 3);
}

BOOST_AUTO_TEST_CASE(matrix_node_is_connected_to) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{0, 0}, {1, 3}, {5, 7}, {2, 0}};

	BOOST_CHECK(myGraph[0].isConnectedTo(0));
	BOOST_CHECK(!myGraph[0].isConnectedTo(1));
	BOOST_CHECK(myGraph[1].isConnectedTo(3));
}

BOOST_AUTO_TEST_CASE(matrix_node_subscript_operator) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{1, 1}, {4, 5}, {3, 6}, {1, 2}};

	BOOST_CHECK_EQUAL(myGraph.getConnections()[2][2], false);
	BOOST_CHECK_EQUAL(myGraph[2][2], false);
	myGraph[2][2] = true;
	BOOST_CHECK_EQUAL(myGraph.getConnections()[2][2], true);
	BOOST_CHECK_EQUAL(myGraph[2][2], true);
}

BOOST_AUTO_TEST_CASE(matrix_node_connect_disconnect) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{6, 2}, {3, 1}, {4, 2}};

	BOOST_CHECK(!myGraph[2].isConnectedTo(5));
	myGraph[2].connectTo(5);
	BOOST_CHECK(myGraph[2].isConnectedTo(5));

	BOOST_CHECK(myGraph[3].isConnectedTo(1));
	myGraph[3].disconnectFrom(1);
	BOOST_CHECK(!myGraph[3].isConnectedTo(1));

}

BOOST_AUTO_TEST_CASE(matrix_node_get_arcs) {
	using matrix::Graph;
	using matrix::Node;

	Graph myGraph{{4, 5}, {6, 3}, {2, 4}, {5, 2}, {6, 4}, {3, 3}};

	BOOST_CHECK((myGraph[0].getArcs() == std::vector<size_t>{}));
	BOOST_CHECK((myGraph[2].getArcs() == std::vector<size_t>{4}));
	BOOST_CHECK((myGraph[6].getArcs() == std::vector<size_t>{3, 4}));
}
