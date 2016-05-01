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

			State state;

		    bool operator==(AstarNodeProperty const& other) const {
			    return (gScore == other.gScore) && (hScore == other.hScore) &&
			           (state == other.state);
		    }
		};

		template <>
		struct AstarNodeProperty<void> {
			int gScore;
			int hScore;

		    bool operator==(AstarNodeProperty const& other) const {
			    return (gScore == other.gScore) && (hScore == other.hScore);
		    }
		};

	    struct NoProperty {
		    bool operator==(NoProperty const&) const {
			    return true;
		    }
	    };
}
