#include <iostream>
#include <vector>
#include <iostream>
#include <set>
#include <algorithm>
#include <cmath>
#include <assert.h>


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

struct edge {
    point up;
    point down;
    bool is_horizontal = false;
};

inline bool operator<(const point &lhs, const point &rhs) {
    if (lhs.x == 0 && rhs.x == 0)
        return lhs.y < rhs.y;
    return lhs.y * rhs.x < lhs.x * rhs.y;
}

long double EPS = 1e-7;
vector<point> points;
vector<edge> edges;
int n;

ll turn(const point &a, const point &b, const point &c) {
    ll square = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return square;
}

bool left_turn(const point &a, const point &b, const point &c) {
    return turn(a, b, c) > 0;
}


bool is_collinear(const point &a, const point &b, const point &c) {
    bool res = abs(turn(a, b, c)) < EPS;
    return res;
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

int main() {
    cin >> n;
    point query;
    cin >> query.x >> query.y;
    points.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
    }
    //delete_collinear();
    edges.resize(n);
    for (int i = 0; i < n; i++) {
        int next_i = (i + 1) % n;
        if (points[i].y == points[next_i].y) {
            edges[i].is_horizontal = true;
        }
        if (points[i].y > points[next_i].y) {
            edges[i].up = points[i];
            edges[i].down = points[next_i];
        } else {
            edges[i].up = points[next_i];
            edges[i].down = points[i];
        }
    }

    int count_intersection = 0;
    for (auto edge : edges) {
        if (is_collinear(query, edge.up, edge.down) &&
            ((edge.down.x <= query.x && query.x <= edge.up.x) || (edge.down.x >= query.x && query.x >= edge.up.x))) {
            //assert((edge.down.y <= query.y && query.y <= edge.up.y)));
            cout << "YES";
            return 0;
        }
        if (query.y > edge.up.y || query.y <= edge.down.y) {
            continue;
        }
        if (left_turn(edge.down, edge.up, query)) {
            count_intersection++;
        }
    }
    if (count_intersection % 2 != 0) {
        cout << "YES";
    } else {
        cout << "NO";
    }
    return 0;
}
