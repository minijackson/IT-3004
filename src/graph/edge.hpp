#pragma once

#include <string>
#include <tuple>

/*! \brief Represents an Edge.
 *
 * Used to bybass the explicit tuple constructor using an initializer_list
 */
template <typename EdgeProperty>
class Edge : public std::tuple<std::string, std::string, EdgeProperty> {
public:
	/*! \brief Construct an edge.
	 *
	 * \param startName the name of the node at the start of the edge.
	 * \param endName the name of the node at the end of the edge.
	 */
	constexpr Edge(char const* startName, char const* endName)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           EdgeProperty()) {}

	/*! \brief Construct an edge.
	 *
	 * \param startName the name of the node at the start of the edge.
	 * \param endName the name of the node at the end of the edge.
	 */
	Edge(std::string startName, std::string endName)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           EdgeProperty()) {}

	/*! \brief Construct an edge.
	 *
	 * \param startName the name of the node at the start of the edge.
	 * \param endName the name of the node at the end of the edge.
	 * \param property the property of the edge.
	 */
	constexpr Edge(char const* startName, char const* endName, EdgeProperty property)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           std::move(property)) {}

	/*! \brief Construct an edge.
	 *
	 * \param startName the name of the node at the start of the edge.
	 * \param endName the name of the node at the end of the edge.
	 * \param property the property of the edge.
	 */
	Edge(std::string startName, std::string endName, EdgeProperty property)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           std::move(property)) {}
};
