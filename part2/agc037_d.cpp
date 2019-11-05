#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <random>
#include <bitset>
#include <queue>
#include <functional>
#include <set>
#include <map>
#include <vector>
#include <chrono>
#include <iostream>
#include <limits>
#include <numeric>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

template <class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

template <class Z>
struct MaxFlow {
  vector<vector<tuple<int, int, Z>>> g;

  struct Ref {
    MaxFlow* p;
    int u, id;

    Ref() {}

    Ref(MaxFlow* p, int u, int id) : p(p), u(u), id(id) {}

    Z& get() { return std::get<2>((p->g)[u][id]); }
  };

  MaxFlow() {}
  MaxFlow(int n) : g(n) {}

  pair<Ref, Ref> addEdge(int u, int v, const Z& w) {
    if (u == v) return make_pair(Ref(), Ref());
    int ru = g[u].size(), rv = g[v].size();
    g[u].emplace_back(v, rv, w);
    g[v].emplace_back(u, ru, 0);
    return make_pair(Ref(this, u, ru), Ref(this, v, rv));
  }

  Z dinic(int s, int t) {
    int n = g.size();
    function<vector<int>()> getLevel = [&]() {
      vector<int> level(n, -1);
      queue<int> q;
      level[s] = 0;
      q.push(s);
      while (!q.empty()) {
        int u = q.front(); q.pop();
        for (const auto& pr : g[u])
          if (get<2>(pr) && level[get<0>(pr)] == -1) {
            level[get<0>(pr)] = level[u] + 1;
            q.push(get<0>(pr));
          }
      }
      return level;
    };
    vector<int> r, level;
    function<Z(int, Z)> cap = [&](int u, Z limit) {
      if (u == t) return limit;
      Z ret = 0;
      while (r[u] && limit > ret) {
        int v = get<0>(g[u][r[u] - 1]), rev = get<1>(g[u][r[u] - 1]);
        Z& w = get<2>(g[u][r[u] - 1]);
        if (level[v] == level[u] + 1) {
          Z flow = cap(v, min(limit - ret, w));
          ret += flow;
          w -= flow;
          get<2>(g[v][rev]) += flow;
          if (limit == ret)
            return ret;
          --r[u];
        } else
          --r[u];
      }
      return ret;
    };
    Z ret = 0;
    while (level = getLevel(), level[t] != -1) {
      r = vector<int>(n);
      for (int i = 0; i < n; ++i)
        r[i] = g[i].size();
      ret += cap(s, numeric_limits<Z>::max());
    }
    return ret;
  }
};



int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<vector<int>> a(n, vector<int>(m)), b(a), c(a);
  cin >> a;
  MaxFlow<int> maxMatching(n * 2 + 2);
  vector<MaxFlow<int>::Ref> val(n * m), rs(n), rt(n);
  int s = n * 2, t = n * 2 + 1;
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < m; ++j) {
      --a[i][j];
      val[a[i][j]] = maxMatching.addEdge(i, a[i][j] / m + n, 1).second;
    }
  for (int i = 0; i < n; ++i) {
    rs[i] = maxMatching.addEdge(s, i, 0).first;
    rt[i] = maxMatching.addEdge(i + n, t, 0).first;
  }
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      ++rs[j].get();
      ++rt[j].get();
    }
    maxMatching.dinic(s, t);
    for (int j = 0; j < n; ++j)
      for (int k = 0; k < m; ++k)
        if (val[a[j][k]].get()) {
          --val[a[j][k]].get();
          b[j][i] = a[j][k] + 1;
          c[a[j][k] / m][i] = a[j][k] + 1;
        }
  }
  for (int i = 0; i < n; ++i)
    cout << b[i] << '\n';
  for (int i = 0; i < n; ++i)
    cout << c[i] << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
