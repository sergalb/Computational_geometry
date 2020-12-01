#include "tests.h"
#include <iostream>
#include <cstddef>
#include <vector>
#include <assert.h>
#include <gmpxx.h>

using namespace std;

double EPS = 10e-8;
class point{
public:
    double x;
    double y;
    point(double x, double y) : x(x), y(y) {}
};

bool bounding_box(double a, double b, double c, double d) {
    if (a > b) swap(a, b);
    if (c > d) swap( c, d);
    return max(a,c) <= min(b,d);
}

int points_orientation(point a, point b, point c) {
    double square = (b.x - a.x) * (c.y - a.y) - (b.y -a.y) * (c.x - a.x);
    if (square > EPS) return 1;
    if (square < -EPS) return -1;
    mpq_class ax = a.x;
    mpq_class ay = a.y;
    mpq_class bx = b.x;
    mpq_class by = b.y;
    mpq_class cx = c.x;
    mpq_class cy = c.y;
    return sgn(((bx - ax) * (cy - ay)) - ((by - ay) * (cx - ax)));
}

int main() {
    int test_id;
    cin >> test_id;
    vector<double> segments = genTest(test_id);
    //vector<double> segments {0, 0, 0, 2, 1, 0, -1, 2};
    for (int i = 0; i < segments.size(); i += 8) {
        point a(segments[i], segments[i + 1]);
        point b(segments[i + 2], segments[i + 3]);
        point c(segments[i + 4], segments[i + 5]);
        point d(segments[i + 6], segments[i + 7]);
        if (bounding_box(a.x, b.x, c.x, d.x) && bounding_box(a.y, b.y, c.y, d.y) &&
            points_orientation(a, b, c) * points_orientation(a, b, d) <= 0 &&
            points_orientation(c, d, a) * points_orientation(c, d, b) <= 0) {
            cout << "Y";
        } else {
            cout << "N";
        }
    }
}