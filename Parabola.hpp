#pragma once
#include "Point2D.h"

/**
 
 Calculate number of intersection points between two parabolas with foci `f1` and `f2` and with given `directrix`
 
 */
int intersectionPointsNum(const Point2D &f1, const Point2D &f2, double directrix);

/**

 Find intersection points of two parabolas with foci `f1` and `f2` and with given `directrix`
 
 */
std::vector<Point2D> findIntersectionPoints(const Point2D &f1, const Point2D &f2, double directrix);

