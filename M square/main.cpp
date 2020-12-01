#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <queue>
#include <map>
#include <cmath>
#include <string>
#include <fstream>
#include <stack>
#include <unordered_map>

using namespace std;
typedef long long ll;
typedef long double ld;
#define forn(i, n) for (ll i = 0; i < n; i++)
#define forna(i, a, n) for (ll i = a; i < n; i++)

const ld EPS = 1e-9;

struct ed {
    bool was;
    ll next, chetvert;
    ld tg;

    ed(ld _tg, pair<pair<ld, ld>, ll> &v1, pair<pair<ld, ld>, ll> &v2) {
        was = false;
        next = v2.second;
        tg = _tg;
        if (v1.first.second < v2.first.second) {
            chetvert = (v1.first.first <= v2.first.first ? 1 : 2);
        } else if (v1.first.second > v2.first.second) {
            chetvert = (v1.first.first >= v2.first.first ? 3 : 4);
        } else {
            chetvert = (v1.first.first < v2.first.first ? 1 : 3);
        }
    }
};

struct ver {
    ld x, y;
    vector<ed> ways;
};

vector<ver> g;
map<pair<ld, ld>, ll> num_of_way;

void obhod(ll u, ed &e, vector<ed> &ivec) {
    ivec.push_back(e);
    e.was = true;
    ed &rebro = g[e.next].ways[(num_of_way[{e.next, u}] - 1 + g[e.next].ways.size()) % g[e.next].ways.size()];
    if (!rebro.was)
        obhod(e.next, rebro, ivec);
}

ld square(vector<ed> &ivec) {
    ld ans = 0.0;
    forn(i, ivec.size()) {
        ans += g[ivec[i].next].x * g[ivec[(i + 1) % ivec.size()].next].y;
        ans -= g[ivec[i].next].x * g[ivec[(i - 1 + ivec.size()) % ivec.size()].next].y;
    }
    ans /= 2.0;
    return fabs(ans);
}

int main() {
    ll n;
    cin >> n;
    vector<pair<ld, ld>> lines;
    vector<vector<pair<pair<ld, ld>, ll>>> points(n, vector<pair<pair<ld, ld>, ll>>());
    forn(i, n) {
        ld x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;
        ld tg = 0.0, c = 0.0;
        tg = (fabs(x1 - x2) < EPS ? 1e9 : (ld) (y1 - y2) / (ld) (x1 - x2));
        c = (fabs(x1 - x2) < EPS ? x1 : (ld) y1 - (ld) x1 * tg);
        lines.emplace_back(tg, c);
    }
    ll cnt = 0;
    map<pair<ld, ld>, ll> _map;
    forn(i, n) {
        forna(j, i + 1, n) {
            if (fabs(lines[i].first - lines[j].first) < EPS) continue;
            ld x = 0.0, y = 0.0;
            if (fabs(lines[i].first - 1e9) < EPS) {
                x = lines[i].second;
                y = lines[j].first * x + lines[j].second;
            } else if (fabs(lines[j].first - 1e9) < EPS) {
                x = lines[j].second;
                y = lines[i].first * x + lines[i].second;
            } else {
                x = (lines[j].second - lines[i].second) / (lines[i].first - lines[j].first);
                y = lines[i].first * x + lines[i].second;
            }
            if (!_map.count({x, y})) _map[{x, y}] = cnt++;
        }
    }
    forn(i, n) {
        for (auto it : _map) {
            ld x = it.first.first, y = it.first.second;
            if ((fabs(lines[i].first - 1e9) < EPS && fabs(lines[i].second - x) < EPS) ||
                (fabs(lines[i].first - 1e9) > EPS && fabs(lines[i].first * x + lines[i].second - y) < EPS)) {
                points[i].push_back({{x, y}, it.second});
            }
        }
    }
    g.resize(cnt);
    forn(i, n) {
        forna(j, 1, points[i].size()) {
            g[points[i][j].second].ways.emplace_back(lines[i].first, points[i][j], points[i][j - 1]);
            g[points[i][j - 1].second].ways.emplace_back(lines[i].first, points[i][j - 1], points[i][j]);
        }
    }
    for (auto it : _map) {
        g[it.second].x = it.first.first;
        g[it.second].y = it.first.second;
    }
    vector<ld> ans;
    forn(i, cnt) {
        sort(g[i].ways.begin(), g[i].ways.end(), [](ed a, ed b) {
            return (a.chetvert < b.chetvert) || (a.chetvert == b.chetvert && a.tg < b.tg);
        });
    }
    forn(i, cnt) {
        forn(j, g[i].ways.size()) {
            num_of_way.insert({{i, g[i].ways[j].next}, j});
        }
    }
    forn(i, cnt) {
        for (auto edge : g[i].ways) {
            if (!edge.was) {
                vector<ed> ivec;
                obhod(i, edge, ivec);
                ld _square = square(ivec);
                if (_square > 1e-8) ans.push_back(_square);
            }
        }
    }
    sort(ans.begin(), ans.end());
    if (ans.empty()) cout << 0;
    else {
        cout.precision(15);
        cout << ans.size() - 1 << endl;
        for (ld i = 0; i < ans.size() - 1; i++) cout << ans[i] << endl;
    }
}