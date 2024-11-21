#pragma once
#include "Point2D.h"

constexpr double CIRCLE_CENTER_EPSILON = 1.0e-7;
bool findCircleCenter(const Point2D &p1, const Point2D &p2, const Point2D &p3, Point2D &center);
