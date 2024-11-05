#include "pch.h"
#include "../Splines/Curve.cpp"


TEST(CurveTest, ConstructorTest) {
  std::vector<std::tuple<double, double>> knots = { {0, 1}, {1, 2}, {2, 3}, {2, 4} };
  Curve curve(knots);
  EXPECT_EQ(curve.get_number_of_knots(), 3);  // { {0, 1}, {1, 2}, {2, 3.5} }
}
