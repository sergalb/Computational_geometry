#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

using ll = long long ;


struct point {
    ll x;
    ll y;
    int id;
    int state;

    point(ll x, ll y) : x(x), y(y) {}

    point() {}

    bool operator<(const point &other) {
        return x < other.x || (x == other.x && y < other.y);
    }

};

bool cmp_y(const point &a, const point &b) {
    return a.y < b.y;
}

vector<point> points;
point ans_first;
point ans_second;
double cur_min = 1e20;
int n;

double distance(const point &a, const point &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + .0);
}

void nearest(int l, int r) {
    if (r - l <= 512) {
        for (int i = l; i < r - 1; i++) {
            for (int j = i + 1; j < r; j++) {
                double cur_distance = distance(points[i], points[j]);
                if (cur_distance < cur_min) {
                    cur_min = cur_distance;
                    ans_first = points[i];
                    ans_second = points[j];
                }
            }
        }
        sort(points.begin() + l, points.begin() + r, &cmp_y);
        return;
    }

    int mid = (r + l) >> 1;
    ll mid_x = points[mid].x;
    nearest(l, mid);
    nearest(mid, r);
    static vector<point> merged(n);
    merge(points.begin() + l, points.begin() + mid, points.begin() + mid, points.begin() + r, merged.begin(), &cmp_y);
    copy(merged.begin(), merged.begin() + r - l, points.begin() + l);
    int size = 0;
    for (int i = l; i < r; ++i) {
        if (abs(points[i].x - mid_x) < cur_min) {
            for (int j = size - 1; j >= 0 && points[i].y - merged[j].y < cur_min; --j) {
                double cur_distance = distance(points[i], merged[j]);
                if (cur_distance < cur_min) {
                    cur_min = cur_distance;
                    ans_first = points[i];
                    ans_second = merged[j];
                }
            }
            merged[size++] = points[i];
        }
    }
}


int main() {
//    freopen("input.txt", "r", stdin);
//    freopen("output.txt", "w", stdout);
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);



    cin >> n;
    points.clear();
    points.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
    }
    sort(points.begin(), points.begin() + n);
    nearest(0, n);
//        double d = distance(ans_first, ans_second);
//        ans.push_back(d);
    cout << ans_first.x << " " << ans_first.y << "\n" << ans_second.x << " "
         << ans_second.y;


//    for (auto d :ans) {
//        if (d >= 10000) {
//            cout << "INFINITY" << endl;
//        } else {
//            cout << d << endl;
//        }
//    }

    return 0;
}