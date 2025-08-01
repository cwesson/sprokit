/**
 * @file Dimensions.h
 * @author Conlan Wesson
 * @copyright (c) 2025, Conlan Wesson, GNU General Public License v3
 */

#pragma once

#include <map>
#include <string>

/**
 * Unit dimensions for dimensional analysis.
 * @ingroup dim
 */
class Dimensions {
	public:
		/**
		 * Constructor.
		 */
		Dimensions() = default;

		/**
		 * Multiply a dimension.
		 * @param id Name of the dimension to multiply in.
		 * @return Reference to the updated Dimension.
		 */
		Dimensions& operator*=(const std::string& id);

		/**
		 * Divide a dimension.
		 * @param id Name of the dimension to divide by.
		 * @return Reference to the updated Dimension.
		 */
		Dimensions& operator/=(const std::string& id);

		/**
		 * Multiply a dimension.
		 * @param other Dimensions to multiply in.
		 * @return Reference to the updated Dimension.
		 */
		Dimensions& operator*=(const Dimensions& other);

		/**
		 * Divide a dimension.
		 * @param other Dimensions to divide by.
		 * @return Reference to the updated Dimension.
		 */
		Dimensions& operator/=(const Dimensions& other);

		/**
		 * Check if two Dimensions are equivalent.
		 * @param other Dimensions to compare to.
		 * @return True if the two are equivalent.
		 */
		bool operator==(const Dimensions& other) const;

		/**
		 * Check if two Dimensions are not equivalent.
		 * @param other Dimensions to compare to.
		 * @return True if the two are not equivalent.
		 */
		bool operator!=(const Dimensions& other) const;

		/**
		 * Get the degree of a dimension.
		 * @param i Dimension to get.
		 * @return Degree of the given dimension.
		 */
		int& operator[](const std::string& i);

		/**
		 * Get the degree of a dimension.
		 * @param i Dimension to get.
		 * @return Degree of the given dimension.
		 */
		int operator[](const std::string& i) const;

		/**
		 * Convert to a string.
		 * @return String representation of the units.
		 */
		operator std::string() const;

		std::map<std::string, int>::iterator begin() {
			return dim.begin();
		}

		std::map<std::string, int>::iterator end() {
			return dim.end();
		}

		std::map<std::string, int>::const_iterator begin() const {
			return dim.cbegin();
		}

		std::map<std::string, int>::const_iterator end() const {
			return dim.cend();
		}

	private:
		std::map<std::string, int> dim; ///< Map of unit dimensions.
};
