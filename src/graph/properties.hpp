#pragma once

#include <string>

namespace graph {

		/*! \brief A weight property.
		 *
		 * This can be used to have weighted arcs or weighted nodes.
		 */
		struct WeightedProperty {
			/*! \brief The weight.
			 */
			int weight;

			/*! \brief Compare two weights
			 *
			 * \param other the other property to which compare.
			 */
		    bool operator==(WeightedProperty const& other) const {
			    return weight == other.weight;
		    }
		};

		/*! \brief Add typical A* node properties with a intermediate state.
		 */
		template <typename State>
		struct AstarNodeProperty {

			/*! \brief the \f$g(n)\f$ of the node.
			 */
			int gScore;

			/*! \brief the \f$h(n)\f$ of the node.
			 */
			int hScore;

			/*! \brief The intermediate state
			 */
			State state;

			/*! \brief Compare two A* properties
			 *
			 * \param other the other property to which compare.
			 */
		    bool operator==(AstarNodeProperty const& other) const {
			    return (gScore == other.gScore) && (hScore == other.hScore) &&
			           (state == other.state);
		    }
		};

		/*! \brief Add typical A* node properties without a intermediate state.
		 */
		template <>
		struct AstarNodeProperty<void> {

			/*! \brief the \f$g(n)\f$ of the node.
			 */
			int gScore;

			/*! \brief the \f$h(n)\f$ of the node.
			 */
			int hScore;

			/*! \brief Compare two A* properties
			 *
			 * \param other the other property to which compare.
			 */
		    bool operator==(AstarNodeProperty const& other) const {
			    return (gScore == other.gScore) && (hScore == other.hScore);
		    }
		};

		/*! \brief Placeholder to not have any properties.
		 */
	    struct NoProperty {
			/*! \brief Compare nothing
			 *
			 * Always return true.
			 */
		    bool operator==(NoProperty const&) const {
			    return true;
		    }
	    };
}
