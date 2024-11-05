#pragma once

#include <vector>
#include <tuple>
#include <map>

/** 
 * @brief Curve class representing a mathematical curve.
 */
class Curve {
public:
  /** 
   * @brief Constructor for the Curve class. 
   * @param knots A vector of tuples representing knots. 
   */
  Curve(const std::vector<std::tuple<double, double>>& knots);
  /**
   * Returns the number of unique knots.
   */
  size_t get_number_of_knots() const;

private:
  std::vector<std::tuple<double, double>> knots;
  size_t n_knots;
};


