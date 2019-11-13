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
    const MaxFlow* p;
    int u, id;

    Ref() {}

    Ref(const MaxFlow* p, int u, int id) : p(p), u(u), id(id) {}

    Z get() const { return std::get<2>((p->g)[u][id]); }
  };

  MaxFlow() {}
  MaxFlow(int n) : g(n) {}

  Ref addEdge(int u, int v, const Z& w) {
    if (u == v) return Ref();
    int ru = g[u].size(), rv = g[v].size();
    g[u].emplace_back(v, rv, w);
    g[v].emplace_back(u, ru, 0);
    return Ref(this, v, rv);
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

  int n;
  cin >> n;
  vector<vector<int>> e(n - 1), fnd(n);
  for (int i = 0; i < n - 1; ++i) {
    int c;
    cin >> c;
    e[i].resize(c);
    cin >> e[i];
    for (int j = 0; j < c; ++j)
      fnd[--e[i][j]].push_back(i);
  }
  MaxFlow<int> maxFlow(n * 2);
  int s = 0, t = n * 2 - 1;
  for (int i = 1; i < n; ++i)
    maxFlow.addEdge(i, t, 1);
  vector<vector<MaxFlow<int>::Ref>> eref(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    maxFlow.addEdge(s, n + i, 1);
    eref[i].resize(e[i].size());
    for (size_t j = 0; j < e[i].size(); ++j)
      if (e[i][j] != 0)
        eref[i][j] = maxFlow.addEdge(n + i, e[i][j], 1);
  }
  if (maxFlow.dinic(s, t) != n - 1) {
    cout << "-1\n";
    return 0;
  }
  vector<int> chosen(n - 1);
  for (int i = 0; i < n - 1; ++i) {
    for (size_t j = 0; j < e[i].size(); ++j)
      if (e[i][j] != 0 && eref[i][j].get())
        chosen[i] = e[i][j];
  }
  vector<bool> vis(n);
  vector<pair<int, int>> ans(n - 1);
  function<void(int)> dfs = [&](int u) {
    vis[u] = true;
    for (int id : fnd[u])
      if (chosen[id] != -1) {
        int v = chosen[id];
        chosen[id] = -1;
        ans[id] = make_pair(u + 1, v + 1);
        dfs(v);
      }
  };
  dfs(0);
  if (count(chosen.begin(), chosen.end(), -1) != n - 1)
    cout << "-1\n";
  else
    for (const auto& pr : ans)
      cout << pr.first << ' ' << pr.second << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
