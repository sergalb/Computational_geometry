#include <iostream>
#include <utility>
#include <vector>
#include <vector>
#include <algorithm>
#include <experimental/iterator>
#include <set>
using namespace std;
using ll = long long;
struct Point;

void move_vector(Point &old, Point const &shift);

struct Neighbor_Face;

int last_ind = 4;
set<int> alive;

struct Point {
    Point() = default;

    Point(double x1, double x2, double x3, double x4) {
        vector_form = {x1, x2, x3, x4};
    }


    vector<double> vector_form = vector<double>(4);

    bool operator==(const Point &rhs) const {
        return vector_form == rhs.vector_form;
    }

    bool operator!=(const Point &rhs) const {
        return !(rhs == *this);
    }
};

Point center;

struct Face {
    vector<Point> vertexes;
    Point a0;

    vector<Neighbor_Face> neighbors;

    int flipped = 1;
    bool deleted = false;
    int ind;

    Face() = default;

    Face(vector<Point> edge, Point const &vertex) : vertexes(std::move(edge)) {
        vertexes.push_back(vertex);
        ind = ++last_ind;
        alive.insert(ind);
    }
};

struct Neighbor_Face {
    Neighbor_Face() = default;

    Neighbor_Face(Face *neighbor, vector<Point> edge) : face(neighbor), edge(std::move(edge)) {}

    Face *face;
    //must contains 3 points
    vector<Point> edge;
};

void update_polytope(Face &face, Point const &query, vector<Face> &polytope, vector<bool> &was);

//points should contains 4x4 matrix
double determinant(vector<vector<double>> const &m) {
    double det_22_33_23_32 = m[2][2] * m[3][3] - m[2][3] * m[3][2];
    double det_21_33_23_31 = m[2][1] * m[3][3] - m[2][3] * m[3][1];
    double det_21_32_22_31 = m[2][1] * m[3][2] - m[2][2] * m[3][1];
    double det_20_33_23_30 = m[2][0] * m[3][3] - m[2][3] * m[3][0];
    double det_20_32_22_30 = m[2][0] * m[3][2] - m[2][2] * m[3][0];
    double det_20_31_21_30 = m[2][0] * m[3][1] - m[2][1] * m[3][0];

    return m[0][0] * (m[1][1] * det_22_33_23_32
                      - m[1][2] * det_21_33_23_31
                      + m[1][3] * det_21_32_22_31)
           - m[0][1] * (m[1][0] * det_22_33_23_32
                        - m[1][2] * det_20_33_23_30
                        + m[1][3] * det_20_32_22_30)
           + m[0][2] * (m[1][0] * det_21_33_23_31
                        - m[1][1] * det_20_33_23_30
                        + m[1][3] * det_20_31_21_30)
           - m[0][3] * (m[1][0] * det_21_32_22_31
                        - m[1][1] * det_20_32_22_30
                        + m[1][2] * det_20_31_21_30);
}

void move_vector(Point &old, Point const &shift) {
    for (int i = 0; i < 4; ++i) {
        old.vector_form[i] -= shift.vector_form[i];
    }
}

void revert_normal(Face &face) {
    face.a0 = face.vertexes[0];
    vector<vector<double >> matrix;
    Point moved_point(center.vector_form[0], center.vector_form[1], center.vector_form[2], center.vector_form[3]);
    move_vector(moved_point, face.a0);
    matrix.push_back(moved_point.vector_form);
    for (int i = 1; i < 4; ++i) {
        move_vector(face.vertexes[i], face.a0);
        matrix.push_back(face.vertexes[i].vector_form);
    }
    if (determinant(matrix) < 0) {
        face.flipped = -1;
    }
}

void create_matrix(Face &face, Point const &query, vector<vector<double>> &matrix) {
    Point moved_point(query.vector_form[0], query.vector_form[1], query.vector_form[2], query.vector_form[3]);
    move_vector(moved_point, face.a0);
    matrix.emplace_back(moved_point.vector_form);
    for (int i = 1; i < 4; ++i) {
        matrix.push_back(face.vertexes[i].vector_form);
    }
}

void update_polytope(Face &face, Point const &query, vector<Face> &polytope, vector<bool> &was) {
    if (was[face.ind]) {
        return;
    }
    was[face.ind] = true;
    face.deleted = true;
    alive.erase(face.ind);
    int count_neighbors = face.neighbors.size();
    int real_neigh = 4;
    for (int k = count_neighbors - 1; k >= 0 && real_neigh > 0; --k) {
        auto& neighbor = face.neighbors[k];
        if (neighbor.face != nullptr && !neighbor.face->deleted) {
            --real_neigh;
            vector<vector<double >> matrix;
            create_matrix(*neighbor.face, query, matrix);
            double  det = determinant(matrix);
            if (det * neighbor.face->flipped < 0) {
                update_polytope(*neighbor.face, query, polytope, was);
            } else  {
                Face new_face(neighbor.edge, query);
                for (int i = 0; i < 3; ++i) {
                    vector<Point> edge;
                    for (int j = 0; j < 3; ++j) {
                        if (j != i) {
                            edge.push_back(neighbor.edge[j]);
                        }
                    }
                    edge.push_back(query);
                    new_face.neighbors.emplace_back(nullptr, edge);
                }
                revert_normal(new_face);
                polytope.push_back(new_face);
                polytope.back().neighbors.emplace_back(neighbor.face, neighbor.edge);
                neighbor.face->neighbors.emplace_back(&polytope.back(), neighbor.edge);
            }
        }
    }
}

void define_neighbors(vector<Face> &polytope, int start_ind) {
    for (int i = start_ind; i < polytope.size() - 1; ++i) {
        for (int j = i + 1; j < polytope.size(); ++j) {
            for (auto &fst_neighb : polytope[i].neighbors) {
                if (fst_neighb.face == nullptr) {
                    for (auto &snd_neighb : polytope[j].neighbors) {
                        if (snd_neighb.face == nullptr) {
                            if (fst_neighb.edge == snd_neighb.edge) {
                                fst_neighb.face = &polytope[j];
                                snd_neighb.face = &polytope[i];
                            }
                        }
                    }
                }
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    cout.tie(nullptr);
    vector<Point> start_points(5);
    for (int i = 0; i < 5; ++i) {
        double x1, x2, x3, x4;
        cin >> x1 >> x2 >> x3 >> x4;
        start_points[i] = Point(x1, x2, x3, x4);
    }
    vector<Face> polytope;
    polytope.reserve(100000);
    for (int i = 0; i < 5; ++i) {
        Face simplex_face;
        simplex_face.ind = i;
        for (int j = 0; j < 5; ++j) {
            if (j != i) {
                simplex_face.vertexes.push_back(start_points[j]);
            }
        }
        polytope.push_back(simplex_face);
    }
    for (auto fst_it = polytope.begin(); fst_it != polytope.end(); ++fst_it) {
        for (auto snd_it = next(fst_it); snd_it != polytope.end(); ++snd_it) {
            vector<Point> edge;
            for (auto &fst_point : fst_it->vertexes) {
                for (auto &snd_point: snd_it->vertexes) {
                    if (fst_point == snd_point) {
                        edge.push_back(fst_point);
                    }
                }
            }
            fst_it->neighbors.emplace_back(&*snd_it, edge);
            snd_it->neighbors.emplace_back(&*fst_it, edge);
        }
    }
    for (auto &point : start_points) {
        center.vector_form[0] += point.vector_form[0];
        center.vector_form[1] += point.vector_form[1];
        center.vector_form[2] += point.vector_form[2];
        center.vector_form[3] += point.vector_form[3];
    }
    center.vector_form[0] /= 5;
    center.vector_form[1] /= 5;
    center.vector_form[2] /= 5;
    center.vector_form[3] /= 5;
    for (auto &face : polytope) {
        revert_normal(face);
    }
    alive.insert(0);
    alive.insert(1);
    alive.insert(2);
    alive.insert(3);
    alive.insert(4);
    int n;
    cin >> n;

    for (int i = 0; i < n; ++i) {
        double x1, x2, x3, x4;
        cin >> x1 >> x2 >> x3 >> x4;
        Point query(x1, x2, x3, x4);
        bool is_out = false;
        Face *face = nullptr;
        for (auto ind : alive) {
            auto& it = polytope[ind];
            if (!it.deleted) {
                vector<vector<double >> matrix;
                create_matrix(it, query, matrix);
                double det = determinant(matrix);
                if (det * it.flipped < 0) {
                    is_out = true;
                    face = &it;
                    break;
                }
            }
        }
        if (!is_out) {
            cout << "Ignore\n";
        } else {
            cout << "Rebuild\n";
            int old_size = polytope.size();
            vector<bool> was(last_ind + 1, false);
            update_polytope(*face, query, polytope, was);
            define_neighbors(polytope, old_size);
        }

    }
    return 0;
}
