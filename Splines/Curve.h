#pragma once

#include <vector>
#include <tuple>
#include <map>
#include <string>
#include <stdexcept>
#include <set>
#include <algorithm> // For std::min_element and std::max_element

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
  /**
   * Returns a const reference to knots.
   */
  const std::vector<std::tuple<double, double>>& get_knots() const;
  /**
   * Returns a const reference to knots.
   */
  double value(double x_val) const;
  /**
   * Returns updated Curve object with increased number of knots.
   */
  Curve add_knot(const std::tuple<double, double>& knot) const;
  /**
   * Returns updated Curve object with decreased number of knots if knot found.
   */
  Curve remove_knot(const std::tuple<double, double>& knot) const;
  /**
   * Overload the + operator.
   */
  Curve operator+(const Curve& other) const;
  /**
   * Overload the - operator.
   */
  Curve operator-(const Curve& other) const;
  /**
   * Returns tuple of 2 tuples (x_min, y_min), (x_max, y_max)
        y_min, y_max can be reached at knots or within a spline on the curve.
   */
  std::pair<std::tuple<double, double>, std::tuple<double, double>> min_max() const;


private:
  std::vector<std::tuple<double, double>> knots_;
  size_t n_knots_;
  std::map<std::string, std::vector<double>> sp_;

  /**
   * Returns a map of vectors of spline coeff's for each interval.
   */
  std::map<std::string, std::vector<double>> make_splines() const;
};


