#include "graph.hpp"

#include <sstream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_ALTERNATIVE_INIT_API
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

using namespace graph;

BOOST_AUTO_TEST_CASE(printing_stream_operator_list_graph) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	std::string expected = "\"4\" -> \"5\"\n\"5\" -> \"2\"\n\"6\" -> \"3\"\n\"6\" -> \"4\"\n\"3\" "
	                       "-> \"3\"\n\"2\" -> \"4\"\n";
	std::ostringstream result;

	result << myGraph;
	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(printing_stream_operator_matrix_graph) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	std::string expected = "\"2\" -> \"4\"\n\"3\" -> \"3\"\n\"4\" -> \"5\"\n\"5\" -> \"2\"\n\"6\" "
	                       "-> \"3\"\n\"6\" -> \"4\"\n";
	std::ostringstream result;

	result << myGraph;
	BOOST_CHECK_EQUAL(result.str(), expected);
}

BOOST_AUTO_TEST_CASE(printing_make_digraph_list_graph) {
	using Graph = list::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	std::string expected = "digraph myGraph {\n\"4\" -> \"5\"\n\"5\" -> \"2\"\n\"6\" -> "
	                       "\"3\"\n\"6\" -> \"4\"\n\"3\" -> \"3\"\n\"2\" -> \"4\"\n}\n";

	BOOST_CHECK_EQUAL(makeDigraph("myGraph", myGraph), expected);
}

BOOST_AUTO_TEST_CASE(printing_make_digraph_matrix_graph) {
	using Graph = matrix::Graph<NoProperty, NoProperty>;

	Graph myGraph{{"4", "5"}, {"6", "3"}, {"2", "4"}, {"5", "2"}, {"6", "4"}, {"3", "3"}};

	std::string expected = "digraph myGraph {\n\"2\" -> \"4\"\n\"3\" -> \"3\"\n\"4\" -> "
	                       "\"5\"\n\"5\" -> \"2\"\n\"6\" -> \"3\"\n\"6\" -> \"4\"\n}\n";

	BOOST_CHECK_EQUAL(makeDigraph("myGraph", myGraph), expected);
}

BOOST_AUTO_TEST_CASE(printing_weighted_edge_list_graph) {
	using Graph = list::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{std::make_tuple("4", "5", WeightedProperty{1}),
	              std::make_tuple("6", "3", WeightedProperty{2}),
	              std::make_tuple("2", "4", WeightedProperty{3}),
	              std::make_tuple("5", "2", WeightedProperty{4}),
	              std::make_tuple("6", "4", WeightedProperty{5}),
	              std::make_tuple("3", "3", WeightedProperty{6})};

	std::string expected = "digraph myGraph {\n"
	                       "\"4\" -> \"5\" [label=\"1\"]\n"
	                       "\"5\" -> \"2\" [label=\"4\"]\n"
	                       "\"6\" -> \"3\" [label=\"2\"]\n"
	                       "\"6\" -> \"4\" [label=\"5\"]\n"
	                       "\"3\" -> \"3\" [label=\"6\"]\n"
	                       "\"2\" -> \"4\" [label=\"3\"]\n"
	                       "}\n";

	BOOST_CHECK_EQUAL(makeDigraph("myGraph", myGraph), expected);
}

BOOST_AUTO_TEST_CASE(printing_weighted_edge_matrix_graph) {
	using Graph = matrix::Graph<NoProperty, WeightedProperty>;

	Graph myGraph{std::make_tuple("4", "5", WeightedProperty{1}),
	              std::make_tuple("6", "3", WeightedProperty{2}),
	              std::make_tuple("2", "4", WeightedProperty{3}),
	              std::make_tuple("5", "2", WeightedProperty{4}),
	              std::make_tuple("6", "4", WeightedProperty{5}),
	              std::make_tuple("3", "3", WeightedProperty{6})};

	std::string expected = "digraph myGraph {\n"
	                       "\"2\" -> \"4\" [label=\"3\"]\n"
	                       "\"3\" -> \"3\" [label=\"6\"]\n"
	                       "\"4\" -> \"5\" [label=\"1\"]\n"
	                       "\"5\" -> \"2\" [label=\"4\"]\n"
	                       "\"6\" -> \"3\" [label=\"2\"]\n"
	                       "\"6\" -> \"4\" [label=\"5\"]\n"
	                       "}\n";

	BOOST_CHECK_EQUAL(makeDigraph("myGraph", myGraph), expected);
}
