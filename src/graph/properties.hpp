#ifndef GRAPH_LIB_PROPERTIES_HPP
#define GRAPH_LIB_PROPERTIES_HPP

namespace graph {
		struct WeightedProperty {
			int weight;
		};

		struct AstarNodeProperty {
			int gScore;
			int fScore;
		};

	    struct NoProperty {
		    bool operator==(NoProperty) const {
			    return true;
		    }
	    };
}

#endif
