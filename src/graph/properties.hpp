#pragma once

#include <string>

namespace graph {
		struct WeightedProperty {
			int weight;

		    bool operator==(WeightedProperty const& other) const {
			    return weight == other.weight;
		    }
		};

		template <typename State>
		struct AstarNodeProperty {
			int gScore;
			int hScore;

			std::string parent;

			State state;

		    bool operator==(AstarNodeProperty const& other) const {
			    return (gScore == other.gScore) && (hScore == other.hScore) &&
			           (parent == other.parent) && (state == other.state);
		    }
		};

		template <>
		struct AstarNodeProperty<void> {
			int gScore;
			int hScore;

			std::string parent;

		    bool operator==(AstarNodeProperty const& other) const {
			    return (gScore == other.gScore) && (hScore == other.hScore) &&
			           (parent == other.parent);
		    }
		};

	    struct NoProperty {
		    bool operator==(NoProperty const&) const {
			    return true;
		    }
	    };
}
