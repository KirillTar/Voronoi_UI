#include "Parabola.hpp"


int intersectionPointsNum(const Point2D &f1, const Point2D &f2, double directrix) {
    if (fabs(f1.x - f2.x) < POINT_EPSILON && fabs(f1.y - f2.y) < POINT_EPSILON) {
        return -1;
    }
    if (fabs(f1.y - f2.y) < POINT_EPSILON)
        return 1;
    return 2;
}


std::vector<Point2D> findIntersectionPoints(const Point2D &f1, const Point2D &f2, double d) {
    std::vector<Point2D> result;
    if (fabs(f1.x - f2.x) < POINT_EPSILON) {
        double y = 0.5 * (f1.y + f2.y), D = sqrt(d * d - d * (f1.y + f2.y) + f1.y * f2.y);
        result.push_back(Point2D(f1.x - D, y));
        result.push_back(Point2D(f1.x + D, y));
    } else if (fabs(f1.y - f2.y) < POINT_EPSILON) { 
        double x = 0.5 * (f1.x + f2.x);
        result.push_back(Point2D(x, 0.5 * ((x - f1.x) * (x - f1.x) + f1.y * f1.y  - d * d) / (f1.y - d)));
    } else {
        double D = 2. * sqrt(pow(f1.x - f2.x, 2) * (d - f1.y) * (d - f2.y) * (pow(f1.x - f2.x, 2) + pow(f1.y - f2.y, 2)));
        double T = -2. * d * pow(f1.x - f2.x, 2) + (f1.y + f2.y) * (pow(f2.x - f1.x, 2) + pow(f2.y - f1.y, 2));
        double Q = 2. * pow(f1.y - f2.y, 2);
        
        double y1 = (T - D) / Q, y2 = (T + D) / Q;
        double x1 = 0.5 * (f1.x * f1.x - f2.x * f2.x + (2 * y1 - f2.y - f1.y) * (f2.y - f1.y)) / (f1.x - f2.x);
        double x2 = 0.5 * (f1.x * f1.x - f2.x * f2.x + (2 * y2 - f2.y - f1.y) * (f2.y - f1.y)) / (f1.x - f2.x);
        
        if (x1 > x2) {
            std::swap(x1, x2);
            std::swap(y1, y2);
        }
        result.push_back(Point2D(x1, y1));
        result.push_back(Point2D(x2, y2));
    }
    return result;
}
