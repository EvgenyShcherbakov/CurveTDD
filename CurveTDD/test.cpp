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

// Test for the value method
TEST(CurveTest, ValueMethodTest) {
  std::vector<std::tuple<double, double>> knots = { {0, 1}, {1, 2}, {2, 3}, {3, 4} };
  Curve curve(knots);

  // Test points and expected values (use your own input-output pairs)
  std::vector<std::tuple<double, double>> test_points = { {0.5, 1.5}, {1.5, 2.5}, {2.5, 3.5}, {4.0, 4.0} };

  for (const auto& [x, expected_y] : test_points) {
    double y = curve.value(x);
    EXPECT_NEAR(y, expected_y, 1e-6);  // Use EXPECT_NEAR for floating-point comparison
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

