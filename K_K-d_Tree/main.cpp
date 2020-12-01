#include <iostream>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <list>
#include <vector>
#include <set>
#include <limits.h>


using namespace std;
using ll = int;
const int INF = 2147483647 / 2;
const double EPS = 1e-7;
const int count_point_in_list = 25;

struct point {
    ll x;
    ll y;
    int ind;

    point() = default;

    bool operator==(const point &other) {
        return x == other.x && y == other.y;
    }

    point(int x, int y) : x(x), y(y) {};
};

inline bool operator<(const point &lhs, const point &rhs) {
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

inline bool cmp_x(const point &lhs, const point &rhs) {
    return lhs.x < rhs.x;
}

inline bool cmp_y(const point &lhs, const point &rhs) {
    return lhs.y < rhs.y;
}

struct Node {
    vector<point> points;
    Node *left = nullptr;
    Node *right = nullptr;
    int median;
    point left_down;
    point right_top;


    bool is_list() const {
        return left == nullptr && right == nullptr;
    }
};

int n, m;
vector<point> vertexes;

double distance(const point &a, const point &b) {
    return sqrt(((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + .0));
}

vector<point> &build_tree(decltype(vertexes.begin()) start, decltype(vertexes.begin()) end, Node &cur, point left_down,
                          point right_top, bool depth) {
    cur.left_down = left_down;
    cur.right_top = right_top;
    if (end - start <= count_point_in_list) {
        cur.points = vector<point>(start, end);
        return cur.points;
    }
    cur.left = new Node();
    cur.right = new Node();
    auto &cmp = (depth) ? cmp_x : cmp_y;
    sort(start, end, cmp);
    size_t center = ((end - start) / 2) - 1 + ((end - start) % 2);
    auto bound = upper_bound(start, end, *(start + center), cmp);
    vector<point> *second_part;
    if (depth) {
        cur.median = (bound - 1)->x;

        cur.points = build_tree(start, bound, *cur.left, left_down, point(cur.median, right_top.y), !depth);
        second_part = &build_tree(bound, end, *cur.right, point(cur.median, left_down.y), right_top, !depth);
    } else {
        cur.median = (bound - 1)->y;
        cur.points = build_tree(start, bound, *cur.left, left_down, point(right_top.x, cur.median), !depth);
        second_part = &build_tree(bound, end, *cur.right, point(left_down.x, cur.median), right_top, !depth);
    }
    cur.points.insert(cur.points.end(), second_part->begin(), second_part->end());
    return cur.points;
}

bool in_rectangle(const point &query, const point &left_down, const point &right_top) {
    return left_down.x <= query.x && query.x <= right_top.x && left_down.y <= query.y && query.y <= right_top.y;
}

bool is_rect_in_rect(const point &f_left_down, const point &f_right_top, const point &s_left_down,
                     const point &s_right_top) {
    return f_left_down.x >= s_left_down.x && f_left_down.y >= s_left_down.y &&
           f_right_top.x <= s_right_top.x && f_right_top.y <= s_right_top.y;
}

bool bounding_box(int a, int b, int c, int d) {
    if (a > b) swap(a, b);
    if (c > d) swap(c, d);
    return max(a, c) <= min(b, d);
}


bool is_rects_intersected(const point &f_left_down, const point &f_right_top, const point &s_left_down,
                          const point &s_right_top) {
    return bounding_box(f_left_down.x, f_right_top.x, s_left_down.x, s_right_top.x) &&
           bounding_box(f_left_down.y, f_right_top.y, s_left_down.y, s_right_top.y);
}


const Node &find_leaf(const point &query, const Node &cur, bool level) {
    if (cur.is_list()) {
        return cur;
    }
    if (level) {
        if (query.x <= cur.median || cur.right->points.empty()) {
            return find_leaf(query, *cur.left, !level);
        } else {
            return find_leaf(query, *cur.right, !level);
        }
    } else {
        if (query.y <= cur.median || cur.right->points.empty()) {
            return find_leaf(query, *cur.left, !level);
        } else {
            return find_leaf(query, *cur.right, !level);
        }
    }
}

void find_window(Node &cur, const point &left_down, const point &right_top, vector<vector<point> *> & ans) {
    if (cur.left->is_list()) {
        ans.push_back(&cur.left->points);
    } else if (is_rect_in_rect(cur.left->left_down, cur.left->right_top, left_down, right_top)) {
        ans.push_back(&cur.left->points);
    } else if (is_rects_intersected(cur.left->left_down, cur.left->right_top, left_down, right_top)) {
        find_window(*cur.left, left_down, right_top, ans);
    }

    if (cur.right->is_list()) {
        ans.push_back(&cur.right->points);
    } else if (is_rect_in_rect(cur.right->left_down, cur.right->right_top, left_down, right_top)) {
        ans.push_back(&cur.right->points);
    } else if (is_rects_intersected(cur.right->left_down, cur.right->right_top, left_down, right_top)) {
        find_window(*cur.right, left_down, right_top, ans);
    }
}

pair<int, double> find_closest_by_linear_search(vector<point>  const& points, point const& query, double dist) {
    int ans = -1;
    for (auto point : points) {
        double cur_dist = distance(query, point);
        if (cur_dist < dist) {
            dist = cur_dist;
            ans = point.ind;
        }
    }
    return {ans, dist};
}
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cin >> n >> m;
    vertexes.resize(n);
    for (int i = 0; i < n; ++i) {
        point vert;
        cin >> vert.x >> vert.y;
        vert.ind = i + 1;
        vertexes[i] = vert;
    }
    set<point> s(vertexes.begin(), vertexes.end());
    vertexes.assign(s.begin(), s.end());
    Node root;
    point left_down_root;
    point right_top_root;
    sort(vertexes.begin(), vertexes.end(), cmp_y);
    left_down_root.y = vertexes[0].y;
    right_top_root.y = vertexes.back().y;
    sort(vertexes.begin(), vertexes.end(), cmp_x);
    left_down_root.x = vertexes[0].x;
    right_top_root.x = vertexes.back().x;
    build_tree(vertexes.begin(), vertexes.end(), root, left_down_root, right_top_root, true);
    for (int i = 0; i < m; ++i) {
        point query;
        cin >> query.x >> query.y;
        const Node &leaf = find_leaf(query, root, true);
        if (leaf.points.empty()) {
            return 1;
        }
        auto [ans, dist] = find_closest_by_linear_search(leaf.points, query, 1e9);
        if (dist < EPS || &leaf == &root) {
            cout << ans << "\n";
            continue;
        }
        ll ceiled_dist = ceil(dist);
        vector<vector<point> *> points;
        find_window(root, point(query.x - ceiled_dist, query.y - ceiled_dist),
                                                     point(query.x + ceiled_dist, query.y + ceiled_dist), points);

        for (auto p : points) {
            for (int j = 0; j < p->size(); ++j) {
                auto [cur_ans, cur_dist] = find_closest_by_linear_search(*p, query, dist);
                if (cur_ans != -1){
                    ans = cur_ans;
                    dist = cur_dist;
                }
            }
        }
        cout << ans << "\n";
    }
    return 0;
}
