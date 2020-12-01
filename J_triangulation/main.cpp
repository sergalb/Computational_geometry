#include <iostream>
#include <cmath>
#include <algorithm>
#include <assert.h>
#include <list>
#include <vector>

using namespace std;
using ll = long long;
const ll LLM = 9223372036854775807L;
const double EPS = 1e-7;

struct vertex {
    ll x;
    ll y;
    int ind;

    vertex() = default;

    bool operator==(const vertex& other) {
        return x == other.x && y == other.y;
    }
};

bool bounding_box(double a, double b, double c, double d);

bool intersection(vertex* a, vertex* b, vertex* c, vertex* d);

struct Polygon {

    list<vertex> vertexes;

    Polygon (list<vertex>& vertexes) : vertexes(vertexes){}

    decltype(vertexes.begin()) begin() {
        return vertexes.begin();
    }

    decltype(vertexes.begin()) end() {
        return vertexes.end();
    }

    int size() {
        return vertexes.size();
    }

    decltype(vertexes.begin()) next(decltype(vertexes.begin()) it) {
        if (++it == vertexes.end()) {
            return vertexes.begin();
        } else {
            return it;
        }
    }

    decltype(vertexes.begin()) prev(decltype(vertexes.begin()) it) {
        if (it == vertexes.begin()) {
            return --vertexes.end();
        } else {
            return --it;
        }
    }

    void erase(decltype(vertexes.begin()) it) {
        vertexes.erase(it);
    }

    bool is_polygon_intersect(decltype(vertexes.begin()) start, decltype(vertexes.begin()) best){
        auto cur = start;
        auto next_vertex = next(cur);
        while (next_vertex != start) {
            cur = next_vertex;
            next_vertex = next(next_vertex);
            if (next_vertex == best || cur == best || next_vertex == start) {
                continue;
            }
            if (intersection(&(*start), &(*best), &(*cur), &(*next_vertex))) {
                return true;
            }
        }
        return false;
    }
};

int n;


inline ll turn(vertex *a, vertex *b, vertex *c) {
    ll square = (b->x - a->x) * (c->y - a->y) - (b->y - a->y) * (c->x - a->x);
    return square;
}

inline bool is_left_turn(vertex *a, vertex *b, vertex *c) {
    return turn(a, b, c) > 0;
}

inline bool is_right_turn(vertex *a, vertex *b, vertex *c) {
    return turn(a, b, c) < 0;
}


inline bool in_triangle(vertex *query, vertex *a, vertex *b, vertex *c) {
    return !is_right_turn(a, b, query) && !is_right_turn(b, c, query) && !is_right_turn(c, a, query);
}

bool is_collinear(vertex *a, vertex *b, vertex *c) {
    bool res = abs(turn(a, b, c)) < EPS;
    return res;
}

double distance(const vertex &a, const vertex &b) {
    return sqrt(((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + .0));
}

bool is_ear(vertex & prev, vertex & vert, vertex & next, Polygon & polygon){
    for (auto it : polygon) {
        if (it == prev || it == vert || it == next) {
            continue;
        }
        if (in_triangle(&it, &prev, &vert, &next)){
            return false;
        }
    }
    return true;
}

void print_trinagle(vertex & prev, vertex & vert, vertex & next) {
    cout << to_string(prev.ind) << " " << to_string(vert.ind) << " " << to_string(next.ind) << "\n";
}

void triangulate(Polygon&& polygon) {
    list<vertex> stack;

    for (auto it : polygon) {
        while (stack.size() >= 3) {
            vertex prev = *(------stack.end());
            auto vert = *(----stack.end());
            auto next = *(--stack.end());
            if (is_left_turn(&prev, &vert, &next) && is_ear(prev, vert, next, polygon)) {
                stack.erase(----stack.end());
                print_trinagle(prev, vert, next);
            } else {
                break;
            }
        }
        stack.push_back(it);
    }
    while (stack.size() >= 3) {
        vertex prev = *(-- -- --stack.end());
        auto vert = *(-- --stack.end());
        auto next = *(--stack.end());
        stack.erase(----stack.end());
        print_trinagle(prev, vert, next);
    }


}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    cin >> n;

    list<vertex> vertexes;
    for (int i = 0; i < n; ++i) {
        vertex vert;
        cin >> vert.x >> vert.y;
        vert.ind = i + 1;
        vertexes.push_back(vert);
    }

    triangulate(Polygon(vertexes));
//    for (auto &v : vertexes) {
//        cout << v.ind << " ";
//    }
//    cout  << vert->prev->ind << " " << vert->ind << " " << vert->next->ind;

    return 0;
}