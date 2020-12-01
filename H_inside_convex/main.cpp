#include <iostream>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <cmath>


using namespace std;
using ll = long long;

struct point {
    ll x;
    ll y;

    bool operator==(const point &other) {
        return x == other.x && y == other.y;
    }

    point(ll x, ll y) : x(x), y(y) {}

    point() {}
};

inline bool operator<(const point &lhs, const point &rhs) {
    if (lhs.x == 0 && rhs.x == 0)
        return lhs.y < rhs.y;
    return lhs.y * rhs.x < lhs.x * rhs.y;
}

long double EPS = 1e-7;
vector<point> points;
const ll LLM = 9223372036854775800ll;
point first(LLM, LLM);

int n;

ll turn(const point &a, const point &b, const point &c) {
    ll square = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return square;
}

bool left_turn(const point &a, const point &b, const point &c) {
    return turn(a, b, c) > 0;
}

bool right_turn(const point &a, const point &b, const point &c) {
    return turn(a, b, c) < 0;
}

bool is_collinear(const point &a, const point &b, const point &c) {
    bool res = abs(turn(a, b, c)) < EPS;
    return res;
}

double distance(const point &a, const point &b) {
    return sqrt(((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + .0));
}

bool square_is_null() {
    for (int i = 2; i < points.size(); i++) {
        if (turn(points[i - 2], points[i - 1], points[i]) != 0)
            return false;
    }
    return (is_collinear(first, points[0], points[1]) &&
            is_collinear(points[points.size() - 2], points.back(), first));
}

void delete_collinear() {
    if (points.size() < 3) {
        return;
    }
    for (int i = 2; i < points.size();) {
        int start = i - 1;
        int ind = i;
        while (ind < points.size() && is_collinear(points[ind - 2], points[ind - 1], points[ind])) {
            ++ind;
        }
        points.erase(points.begin() + start, points.begin() + ind - 1);
        i = start + 2;
    }
    if (points.size() > 2 && is_collinear(points[points.size() - 2], points.back(), points[0])) {
        points.erase(points.end() - 1);
    }

}

void inside_polygon(const point &query) {
    if (right_turn(first, points[0], query) || left_turn(first, points.back(), query)) {
        cout << "OUTSIDE" << "\n";
        return;
    }
    if (is_collinear(first, points[0], query)) {
        if (right_turn(points[0], points[1], query)) {
            cout << "OUTSIDE" << "\n";
        } else {
            cout << "BORDER" << "\n";
        }
        return;
    }
    if (is_collinear(first, points.back(), query)) {
        if (left_turn(points.back(), points[points.size() - 2], query)) {
            cout << "OUTSIDE" << "\n";
        } else {
            cout << "BORDER" << "\n";
        }
        return;
    }
    auto cmp = [query](const point &lhs, const point &rhs) {
        return turn(first, lhs, query) > turn(first, rhs, query);
    };
    auto right = upper_bound(points.begin(), points.end(), query);
    auto left = right - 1;
    double t = turn(*left, *right, query);
    if (abs(t) < EPS) {
        cout << "BORDER" << "\n";
        return;
    }
    if (t < 0) {
        cout << "OUTSIDE" << "\n";
    } else {
        cout << "INSIDE" << "\n";
    }
}

int main() {
    cin >> n;
    points.resize(n);

    int first_ind = 0;

    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
        if (points[i].x < first.x || (points[i].x == first.x && points[i].y < first.y)) {
            first = points[i];
            first_ind = i;
        }
    }
    for (int i = 0; i < n; ++i) {
        points[i].x -= first.x;
        points[i].y -= first.y;
    }
    rotate(points.begin(), points.begin() + first_ind, points.end());
    //first = points[0];
    point real_first = first;
    delete_collinear();
    first = point(0, 0);
    auto cmp_polygon = [](const point &lhs, const point &rhs) {
        return left_turn(first, lhs, rhs) ||
               (is_collinear(first, lhs, rhs) && distance(first, lhs) < distance(first, rhs));
    };
    set<point, decltype(cmp_polygon)> s(points.begin() + 1, points.end(), cmp_polygon);

    points.assign(s.begin(), s.end());
    vector<point> queries;
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        point p;
        cin >> p.x >> p.y;
        p.x -= real_first.x;
        p.y -= real_first.y;
        queries.push_back(p);
    }

    if (points.size() == 1) {
        return 1;
        for (auto query : queries) {
            if (query == first) {
                cout << "BORDER\n";
            } else {
                cout << "OUTSIDE\n";
            }
        }
        return 0;
    }
    if (square_is_null()) {
        return 1;
        point last = points.back();
        for (auto query : queries) {
            if (turn(first, last, query) != 0) {
                cout << "OUTSIDE\n";
                continue;
            }
            //return 1;
            if (((first.x <= query.x && query.x <= last.x) || (first.x >= query.x && query.x >= last.x)) &&
                ((first.y <= query.y && query.y <= last.y) || (first.y >= query.y && query.y >= last.y))) {
                cout << "BORDER\n";
            } else {
                cout << "OUTSIDE\n";
            }
        }
        return 0;
    }
//    while (true) {
//        point p;
//        cin >> p.x >> p.y;
//        p.x -= real_first.x;
//        p.y -= real_first.y;
//        inside_polygon(p);
//    }
    for_each(queries.begin(), queries.end(), inside_polygon);
    return 0;
}
