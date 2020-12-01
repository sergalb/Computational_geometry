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
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

vector<point> points;
ll const LLM = 922337203685477580ll;
//point first(-LLM, LLM);
int n;

bool left_turn(const point &a, const point &b, const point &c) {
    ll square = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return square > 0;
}

bool right_turn(const point &a, const point &b, const point &c) {
    ll square = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return square < 0;
}


double distance(const point &a, const point &b) {
    return sqrt(((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + .0));
}

int main() {
    cin >> n;
    points.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
//        if (points[i].y < first.y || (points[i].y == first.y && points[i].x > first.x)) {
//            first = points[i];
//        }
    }
    set<point> s(points.begin(), points.end());
    points.assign(s.begin(), s.end());
    sort(points.begin(), points.end());
    point first = points[0];
    point last = points.back();
    vector<point> up = {first};
    vector<point> down = {first};
    for (int i = 1; i < points.size(); ++i) {
        if (i == points.size() - 1 || right_turn(first, points[i], last)) {
            while (up.size() >= 2 && !right_turn(up[up.size() - 2], up[up.size() - 1], points[i]))
                up.pop_back();
            up.push_back(points[i]);
        }
        if (i == points.size() - 1 || left_turn(first, points[i], last)) {
            while (down.size() >= 2 && !left_turn(down[down.size() - 2], down[down.size() - 1], points[i]))
                down.pop_back();
            down.push_back(points[i]);
        }
    }
    vector<point> ans;

    for (int i = 0; i < up.size(); ++i) {
        ans.push_back(up[i]);
    }
    for (int i = down.size() - 2; i > 0; --i) {
        ans.push_back(down[i]);
    }

    cout << ans.size() << endl;

    for (auto point : ans) {
        cout << point.x << " " << point.y << "\n";
    }

//    double per = 0;
//    for (int j = 0; j < ans.size() - 1; ++j) {
//        per += distance(ans[j], ans[j + 1]);
//    }
//    per += distance(ans[0], ans.back());
//    cout << per;
    return 0;
}