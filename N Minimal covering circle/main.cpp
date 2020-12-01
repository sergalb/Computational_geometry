#include <iostream>
#include <cmath>
#include <algorithm>
#include <list>
#include <random>
#include <vector>
#include <iomanip>


using namespace std;
struct point;

const int INF = 2147483647 / 2;
const double EPS = 1e-8;
int n;
vector<point> vertexes;

double distance(const point &a, const point &b);

struct point {
    double x;
    double y;

    point() = default;

    point(double x, double y) : x(x), y(y) {};

    bool operator==(const point &other) {
        return x == other.x && y == other.y;
    }
};

inline double sqr(double x) {
    return x * x;
}

struct Circle {
    point center;
    double radius;

    Circle(const point &center, double radius) : center(center), radius(radius) {}

    Circle(point const &a, point const &b) {
        center = point((a.x + b.x) / 2,
                       (a.y + b.y) / 2);
        radius = distance(a, center);
    }

    Circle(point const &a, point const &b, point const &c) {
        double x_center = a.y * (sqr(b.x) + sqr(b.y) - sqr(c.x) - sqr(c.y))
                        + b.y * (sqr(c.x) + sqr(c.y) - sqr(a.x) - sqr(a.y))
                        + c.y * (sqr(a.x) + sqr(a.y) - sqr(b.x) - sqr(b.y));
        double denominator = a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y);
        x_center /= denominator;
        x_center *= -0.5;
        double y_center = a.x * (sqr(b.x) + sqr(b.y) - sqr(c.x) - sqr(c.y))
                          + b.x * (sqr(c.x) + sqr(c.y) - sqr(a.x) - sqr(a.y))
                          + c.x * (sqr(a.x) + sqr(a.y) - sqr(b.x) - sqr(b.y));
        y_center /= denominator;
        y_center *= 0.5;
        center = point(x_center, y_center);
        radius = distance(center, a);

    }

    bool contains_point(point const &x) {
        return distance(center, x) <= radius + EPS;
    }
};

double distance(const point &a, const point &b) {
    return sqrt(((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + .0));
}

Circle min_disk_with_point(point const& p, int last_in_point_ind) {
    //shuffle(vertexes.begin(), vertexes.begin() + last_in_point_ind + 1, std::mt19937(std::random_device()()));
    Circle ans(vertexes[0], p);
    point* k = nullptr;
    for (int i = 1; i <= last_in_point_ind; ++i) {
        if (!ans.contains_point(vertexes[i])) {
            ans = Circle(p, vertexes[i]);
            k = &vertexes[i];
            for (int j = 0; j < i; ++j) {
                if (!ans.contains_point(vertexes[j])){
                    ans = Circle(p, *k, vertexes[j]);
                }
            }


        }
    }
    return ans;
}

Circle min_disk() {
    shuffle(vertexes.begin(), vertexes.end(), std::mt19937(std::random_device()()));
    Circle ans(vertexes[0], vertexes[1]);
    for (int i = 2; i < n; ++i) {
        if (!ans.contains_point(vertexes[i])) {
            ans = min_disk_with_point(vertexes[i], i - 1);
        }
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cin >> n;
    vertexes.resize(n);
    for (int i = 0; i < n; ++i) {
        point vert;
        cin >> vert.x >> vert.y;
        vertexes[i] = vert;
    }
    if (n == 1) {
        cout << "0\n" << vertexes[0].x << " " << vertexes[0].y;
        return 0;
    }
    Circle ans = min_disk();
    cout << setprecision(20) << ans.radius << "\n" << ans.center.x << " " << ans.center.y;
    return 0;
}