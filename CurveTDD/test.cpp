#include "pch.h"
#include "../Splines/Curve.cpp"
#include <cmath>
#include <stdexcept>


// This file contains unit tests for the Curve class using Google Test framework

// Test to verify that the Curve constructor initializes the correct number of knots
TEST(CurveTest, ConstructorTest) {
  std::vector<std::tuple<double, double>> knots = { {0, 1}, {1, 2}, {2, 3}, {2, 4} };
  Curve curve(knots);
  EXPECT_EQ(curve.get_number_of_knots(), 3);  // Expected: { {0, 1}, {1, 2}, {2, 3.5} }
}

// Test to verify that the get_knots method returns the correct copy of the knots vector
TEST(CurveTest, GetKnotsTest) {
  std::vector<std::tuple<double, double>> knots = { {0, 1}, {1, 2}, {2, 3}, {2, 4} };
  Curve curve(knots);

  // Expected knots after processing: { {0, 1}, {1, 2}, {2, 3.5} }
  auto copied_knots = curve.get_knots();
  std::vector<std::tuple<double, double>> expected_knots = { {0, 1}, {1, 2}, {2, 3.5} };
  EXPECT_EQ(copied_knots, expected_knots);  // Compare the copied knots with the expected values
}

// Test for the value method with known input
TEST(CurveTest, ValueMethodTest) {
  // Define your knots and control points as vectors of tuples
  std::vector<std::vector<std::tuple<double, double>>> lst_knots = {
      {{1, 1}, {1.5, 1}, {2, 4}, {2.5, 3}, {3, 2}, {3.5, 4}, {4, 3}},
      {{1, 1}, {2, 4}, {3, 2}, {4, 3}},
      {{-1, 0}, {10, 3}, {4, 1.5}, {-1, 4}, {1.5, 1}, {3, 2}},
      {{-5, 0}, {6, -1}, {0, 2}, {2, 4}, {8, 3}, {2, 2}, {3, 2}},
      {{4, 4}, {-4, 3}, {2, 6}, {-2, 6}, {0, 6}},
      {{-2, 0}, {0, -1}, {2, 4}, {2.5, 3}, {3, 2}, {4, 3}}
  };

  // Expected x and y values
  std::vector<double> lst_x = { 2.2, 4.6, -3, -2.5, 4, -2 };
  std::vector<double> lst_y = { 4.068430769230769, 4.0352, 3.1431412103746394, 0.5996164259927798, 4.0, 0.0 };

  for (size_t i = 0; i < lst_knots.size(); ++i) {
    Curve curve(lst_knots[i]);
    double x_val = lst_x[i];
    double expected_y = lst_y[i];
    double actual_y = curve.value(x_val);

    ASSERT_NEAR(expected_y, actual_y, 1e-6);
  }
}

// Test for the min and max values for the curve inside the given knots
TEST(CurveTest, MinMaxMethodTest) {
  // Define your knots and control points as vectors of tuples
  std::vector<std::vector<std::tuple<double, double>>> lst_knots = {
      {{1, 1}, {1.5, 1}, {2, 4}, {2.5, 3}, {3, 2}, {3.5, 4}, {4, 3}},
      {{1, 1}, {2, 4}, {3, 2}, {4, 3}},
      {{-1, 0}, {10, 3}, {4, 1.5}, {-1, 4}, {1.5, 1}, {3, 2}},
      {{-5, 0}, {6, -1}, {0, 2}, {2, 4}, {8, 3}, {2, 2}, {3, 2}},
      {{4, 4}, {-4, 3}, {2, 6}, {-2, 6}, {0, 6}},
      {{-2, 0}, {0, -1}, {2, 4}, {2.5, 3}, {3, 2}, {4, 3}}
  };

  // Expected min and max values
  std::vector<std::pair<std::tuple<double, double>, std::tuple<double, double>>> lst_min_max = {
      {{1.2886751345948129, 0.5874659615021136}, {2.1096984493010407, 4.157063477690783}},
      {{1, 1.0}, {1.9927271762054324, 4.000242132531974}},
      {{0.9575200141235161, 0.8560448601421319}, {10, 3.0}},
      {{5.617704864531077, -1.1279812580580901}, {1.6986545353359912, 3.0598403375432843}},
      {{-4, 3.0}, {-1.3010961023018706, 6.205359754330297}},
      {{-0.6463374738743516, -1.6567213006313906}, {1.87261117785007, 4.053237076850908}}
  };

  for (size_t i = 0; i < lst_knots.size(); ++i) {
    Curve curve(lst_knots[i]);
    auto [min_val, max_val] = curve.min_max();

    ASSERT_NEAR(std::get<0>(lst_min_max[i].first), std::get<0>(min_val), 1e-9);
    ASSERT_NEAR(std::get<1>(lst_min_max[i].first), std::get<1>(min_val), 1e-9);
    ASSERT_NEAR(std::get<0>(lst_min_max[i].second), std::get<0>(max_val), 1e-9);
    ASSERT_NEAR(std::get<1>(lst_min_max[i].second), std::get<1>(max_val), 1e-9);
  }
}

// Test for the add_knot method
TEST(CurveTest, AddKnotTest) {
  std::vector<std::tuple<double, double>> knots = { {0, 1}, {1, 2}, {2, 3} };
  Curve curve(knots);

  auto new_curve = curve.add_knot({ 3, 4 });

  EXPECT_EQ(new_curve.get_number_of_knots(), 4);

  auto new_knots = new_curve.get_knots();
  std::vector<std::tuple<double, double>> expected_knots = { {0, 1}, {1, 2}, {2, 3}, {3, 4} };
  EXPECT_EQ(new_knots, expected_knots);
}

// Test for the remove_knot method
TEST(CurveTest, RemoveKnotTest) {
  std::vector<std::tuple<double, double>> knots = { {0, 1}, {1, 2}, {2, 3}, {3, 4} };
  Curve curve(knots);

  auto new_curve = curve.remove_knot({ 2, 3 });

  EXPECT_EQ(new_curve.get_number_of_knots(), 3);

  auto new_knots = new_curve.get_knots();
  std::vector<std::tuple<double, double>> expected_knots = { {0, 1}, {1, 2}, {3, 4} };
  EXPECT_EQ(new_knots, expected_knots);
}

// Test for the + operator with valid curves
TEST(CurveTest, AdditionOperatorTestValid) {
  std::vector<std::tuple<double, double>> knots1 = { {0, 1}, {1, 2}, {2, 3} };
  std::vector<std::tuple<double, double>> knots2 = { {3, 4}, {4, 5}, {5, 6} };

  Curve curve1(knots1);
  Curve curve2(knots2);

  auto new_curve = curve1 + curve2;

  EXPECT_EQ(new_curve.get_number_of_knots(), 6);

  auto new_knots = new_curve.get_knots();
  std::vector<std::tuple<double, double>> expected_knots = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5}, {5, 6} };
  EXPECT_EQ(new_knots, expected_knots);
}

// Test for the + operator ensuring combined curves meet the minimum knot requirement
TEST(CurveTest, AdditionOperatorTestInvalid) {
  std::vector<std::tuple<double, double>> knots1 = { {0, 1}, {1, 2}, {2, 3} };
  std::vector<std::tuple<double, double>> knots2 = { {3, 4} };

  Curve curve1(knots1);

  EXPECT_THROW({
      Curve curve2(knots2);
      auto new_curve = curve1 + curve2;
    }, std::invalid_argument);
}

// Test for the - operator with valid curves
TEST(CurveTest, SubtractionOperatorTestValid) {
  std::vector<std::tuple<double, double>> knots1 = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 5} };
  std::vector<std::tuple<double, double>> knots2 = { {2, 3}, {3, 4}, {6, 7} };

  Curve curve1(knots1);
  Curve curve2(knots2);

  auto new_curve = curve1 - curve2;

  EXPECT_EQ(new_curve.get_number_of_knots(), 3);

  auto new_knots = new_curve.get_knots();
  std::vector<std::tuple<double, double>> expected_knots = { {0, 1}, {1, 2}, {4, 5} };
  EXPECT_EQ(new_knots, expected_knots);
}

// Test for the - operator ensuring combined curves meet the minimum knot requirement
TEST(CurveTest, SubtractionOperatorTestInvalid) {
  std::vector<std::tuple<double, double>> knots1 = { {0, 1}, {1, 2}, {2, 3}, {3, 4} };
  std::vector<std::tuple<double, double>> knots2 = { {0, 1}, {1, 2}, {2, 3}, {3, 4} };

  Curve curve1(knots1);
  Curve curve2(knots2);

  EXPECT_THROW({
      auto new_curve = curve1 - curve2;
    }, std::runtime_error);
}

