#ifndef GRAPH_LIB_PROPERTIES_HPP
#define GRAPH_LIB_PROPERTIES_HPP

namespace graph {
		struct WeightedProperty {
			int weight;

		    bool operator==(WeightedProperty other) const {
			    return weight == other.weight;
		    }
		};

		struct AstarNodeProperty {
			int gScore;
			int fScore;

		    bool operator==(AstarNodeProperty other) const {
			    return (gScore == other.fScore) && (fScore == other.fScore);
		    }
		};

	    struct NoProperty {
		    bool operator==(NoProperty) const {
			    return true;
		    }
	    };
}

#endif
