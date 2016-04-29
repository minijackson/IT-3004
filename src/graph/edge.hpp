#pragma once

#include <string>
#include <tuple>

template <typename EdgeProperty>
class Edge : public std::tuple<std::string, std::string, EdgeProperty> {
public:
	constexpr Edge(char const* startName, char const* endName)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           EdgeProperty()) {}

	Edge(std::string startName, std::string endName)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           EdgeProperty()) {}

	constexpr Edge(char const* startName, char const* endName, EdgeProperty property)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           std::move(property)) {}

	Edge(std::string startName, std::string endName, EdgeProperty property)
	      : std::tuple<std::string, std::string, EdgeProperty>(std::move(startName),
	                                                           std::move(endName),
	                                                           std::move(property)) {}
};
