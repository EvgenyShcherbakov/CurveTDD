#include "Curve.h"

#include "Curve.h"
#include <algorithm>
#include <cassert>
#include <unordered_map>

/** 
 * @brief Constructor for the Curve class. 
 * @param knots A vector of tuples representing knots. 
 */
Curve::Curve(const std::vector<std::tuple<double, double>>& knots) {
  // Initialize the dictionary of knots
  std::unordered_map <double, std::pair<double, int>> dict_knots;

  for (const auto& tpl : knots) {
    if (dict_knots.find(std::get<0>(tpl)) != dict_knots.end()) {
      // Add value to y-coordinate
      dict_knots[std::get<0>(tpl)].first += std::get<1>(tpl);
      // Increase count of additions
      dict_knots[std::get<0>(tpl)].second += 1;
    }
    else {
      // Add a node with unique x coordinate
      dict_knots[std::get<0>(tpl)] = { std::get<1>(tpl), 1 };
    }
  }

  for (const auto& pair : dict_knots) {
    // Add knots to the vector from the dictionary
    this->knots.emplace_back(pair.first, pair.second.first / pair.second.second);
  }

  std::sort(this->knots.begin(), this->knots.end());

  assert(this->knots.size() >= 3);  // Cubic spline requires minimum 3 knots
  this->n_knots = this->knots.size();
}

size_t Curve::get_number_of_knots() const {
  return n_knots;
}
