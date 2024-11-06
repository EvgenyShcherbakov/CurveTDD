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
    this->knots_.emplace_back(pair.first, pair.second.first / pair.second.second);
  }

  std::sort(this->knots_.begin(), this->knots_.end());

  // assert(this->knots_.size() >= 3);  // Cubic spline requires minimum 3 knots
  if (knots_.size() < 3) {
    throw std::invalid_argument("Curve must have at least 3 knots");
  }
  this->n_knots_ = this->knots_.size();
  this->sp_ = this->make_splines();
}

size_t Curve::get_number_of_knots() const {
  return n_knots_;
}

const std::vector<std::tuple<double, double>>& Curve::get_knots() const {
  return knots_;
}

// Method to add a knot and return a new Curve
Curve Curve::add_knot(const std::tuple<double, double>& knot) const {
  std::vector<std::tuple<double, double>> new_knots = knots_;
  new_knots.push_back(knot);
  return Curve(new_knots);
}

Curve Curve::remove_knot(const std::tuple<double, double>& knot) const {
  if (knots_.size() > 3) {
    std::vector<std::tuple<double, double>> new_knots;
    for (const auto& k : knots_) {
      if (k != knot) {
        new_knots.push_back(k);
      }
    }
    if (new_knots.size() == knots_.size() - 1) {
      return Curve(new_knots);
    }
  }
  return *this;  // Return the same object if the condition is not met
}

// Overload the + operator
Curve Curve::operator+(const Curve& other) const {
  std::vector<std::tuple<double, double>> new_knots = knots_;
  new_knots.insert(new_knots.end(), other.knots_.begin(), other.knots_.end());

  if (new_knots.size() < 3) {
    throw std::runtime_error("Resulting Curve object must have at least 3 knots");
  }

  return Curve(new_knots);
}

// Overload the - operator
Curve Curve::operator-(const Curve& other) const {
  std::vector<std::tuple<double, double>> new_knots;

  // Use a set to quickly check if a knot exists in the other curve
  std::set<std::tuple<double, double>> other_knots(other.knots_.begin(), other.knots_.end());

  // Only add knots to new_knots if they are not in other_knots
  for (const auto& knot : knots_) {
    if (other_knots.find(knot) == other_knots.end()) {
      new_knots.push_back(knot);
    }
  }

  // Ensure the new curve meets the minimum knot requirement
  if (new_knots.size() < 3) {
    throw std::runtime_error("Resulting Curve object must have at least 3 knots");
  }

  return Curve(new_knots);
}

// Implementation of min_max
std::pair<std::tuple<double, double>, std::tuple<double, double>> Curve::min_max() const {
  std::vector<std::tuple<double, double>> lst_potential = knots_;

  for (size_t j = 0; j < sp_.at("n")[0]; ++j) {
    double a = 3 * sp_.at("d")[j];
    double b = 2 * sp_.at("c")[j] - 6 * sp_.at("d")[j] * sp_.at("x")[j];
    double c = 3 * sp_.at("d")[j] * std::pow(sp_.at("x")[j], 2) + sp_.at("b")[j] - 2 * sp_.at("c")[j] * sp_.at("x")[j];
    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0 && std::abs(a) > 0) {
      double sqrt_discriminant = std::sqrt(discriminant);
      double x1 = (-b - sqrt_discriminant) / (2 * a);
      double x2 = (-b + sqrt_discriminant) / (2 * a);

      if (sp_.at("x")[j] <= x1 && x1 < sp_.at("x")[j + 1]) {
        lst_potential.push_back({ x1, value(x1) });
      }
      if (sp_.at("x")[j] <= x2 && x2 < sp_.at("x")[j + 1]) {
        lst_potential.push_back({ x2, value(x2) });
      }
    }
  }

  auto min_max_x_y = [](const auto& lhs, const auto& rhs) { return std::get<1>(lhs) < std::get<1>(rhs); };
  auto min_y_tuple = *std::min_element(lst_potential.begin(), lst_potential.end(), min_max_x_y);
  auto max_y_tuple = *std::max_element(lst_potential.begin(), lst_potential.end(), min_max_x_y);

  return { min_y_tuple, max_y_tuple };
}

// Implementation of make_splines
std::map<std::string, std::vector<double>> Curve::make_splines() const {
  std::vector<std::tuple<double, double>> points = knots_;
  size_t n = points.size() - 1;

  std::vector<double> x(n + 1), a(n + 1), h(n), alpha(n), l(n + 1, 1), mu(n, 0), z(n + 1, 0), c(n + 1, 0), b(n + 1, 0), d(n + 1, 0);

  for (size_t i = 0; i <= n; ++i) {
    x[i] = std::get<0>(points[i]);
    a[i] = std::get<1>(points[i]);
  }

  for (size_t i = 0; i < n; ++i) {
    h[i] = x[i + 1] - x[i];
  }

  for (size_t i = 1; i < n; ++i) {
    alpha[i] = 3 * ((a[i + 1] - a[i]) / h[i] - (a[i] - a[i - 1]) / h[i - 1]);
  }

  for (size_t i = 1; i < n; ++i) {
    l[i] = 2 * (x[i + 1] - x[i - 1]) - h[i - 1] * mu[i - 1];
    mu[i] = h[i] / l[i];
    z[i] = (alpha[i] - h[i - 1] * z[i - 1]) / l[i];
  }

  for (size_t j = n - 1; j != SIZE_MAX; --j) {
    c[j] = z[j] - mu[j] * c[j + 1];
    b[j] = (a[j + 1] - a[j]) / h[j] - h[j] * (c[j + 1] + 2 * c[j]) / 3;
    d[j] = (c[j + 1] - c[j]) / (3 * h[j]);
  }

  return { {"n", std::vector<double>{static_cast<double>(n)}},
           {"a", a},
           {"b", b},
           {"c", c},
           {"d", d},
           {"x", x} };
}


double Curve::value(double x_val) const {
  const std::vector<double>& x = sp_.at("x");
  const std::vector<double>& a = sp_.at("a");
  const std::vector<double>& b = sp_.at("b");
  const std::vector<double>& c = sp_.at("c");
  const std::vector<double>& d = sp_.at("d");

  size_t j = 0;
  for (size_t i = 0; i < sp_.at("n").front(); ++i) {
    if (x_val < x[i + 1]) {
      j = i;
      break;
    }
    else {
      j = sp_.at("n").front() - 1;
    }
  }

  double dx = x_val - x[j];
  return a[j] + b[j] * dx + c[j] * std::pow(dx, 2) + d[j] * std::pow(dx, 3);
}

