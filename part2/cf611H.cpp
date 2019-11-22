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
  if (n < 10) {
    for (int i = 1; i < n; ++i)
      cout << i << ' ' << i + 1 << '\n';
    return 0;
  }
  int l = log10(n) + 1;
  vector<pair<int, int>> lengths(n - 1);
  vector<vector<int>> cnt(l + 1, vector<int>(l + 1));
  for (int rep = 0; rep < n - 1; ++rep) {
    string u, v;
    cin >> u >> v;
    int x = u.length(), y = v.length();
    lengths[rep] = make_pair(x, y);
    if (x > y) swap(x, y);
    ++cnt[x][y];
  }
  vector<int> prufer(l - 2, 1);
  int tot = pow(l, l - 2);
  for (int rep = tot; rep; --rep) {
    if (rep != tot) {
      auto it = prufer.begin();
      while (*it == l)
        *it++ = 1;
      ++*it;
    }
    function<vector<vector<int>>(vector<int>)> getTree = [&](vector<int> prufer) {
      vector<int> deg(l + 1, 1);
      for (int v : prufer) ++deg[v];
      priority_queue<int> q;
      for (int i = 1; i <= l; ++i)
        if (deg[i] == 1)
          q.push(i);
      vector<vector<int>> ret(l + 1);
      for (int u : prufer) {
        int v = q.top(); q.pop();
        ret[u].push_back(v);
        ret[v].push_back(u);
        if (--deg[u] == 1)
          q.push(u);
      }
      int u = q.top(); q.pop();
      int v = q.top(); q.pop();
      ret[u].push_back(v);
      ret[v].push_back(u);
      return ret;
    };
    function<vector<int>(const vector<vector<int>>&)> getOrientation = [&](const vector<vector<int>>& g) {
      vector<int> ret(l + 1);
      function<void(int)> dfs = [&](int u) {
        for (int v : g[u])
          if (ret[u] != v) {
            ret[v] = u;
            dfs(v);
          }
      };
      dfs(1);
      return ret;
    };
    vector<int> orientation = getOrientation(getTree(prufer));
    vector<int> fst(l + 1), points(l + 1);
    for (int i = 1; i <= l; ++i)
      fst[i] = pow(10, i - 1);
    for (int i = 1; i < l; ++i)
      points[i] = fst[i + 1] - 1 - fst[i];
    points[l] = n - fst[l];

    bool flag = true;
    for (int i = 2; i <= l; ++i)
      if (cnt[orientation[i]][i] + cnt[i][orientation[i]] == 0)
        flag = false;
    if (!flag)
      continue;
    MaxFlow<int> maxFlow(l * (l + 1) / 2 + 2);
    int s = 0, t = l * (l + 1) / 2 + 1;
    vector<vector<MaxFlow<int>::Ref>> ref(l + 1, vector<MaxFlow<int>::Ref>(l + 1));
    int expected = 0;
    for (int i = 1; i <= l; ++i) {
      if (points[i] < cnt[i][i]) {
        cout << "-1\n";
        return 0;
      }
      maxFlow.addEdge(i, t, points[i] - cnt[i][i]);
      expected += points[i] - cnt[i][i];
    }
    int cur = l;
    for (int i = 1; i < l; ++i)
      for (int j = i + 1; j <= l; ++j) {
        ++cur;
        bool existEdge = orientation[j] == i || orientation[i] == j;
        maxFlow.addEdge(s, cur, cnt[i][j] - existEdge);
        ref[i][j] = maxFlow.addEdge(cur, j, cnt[i][j] - existEdge);
        ref[j][i] = maxFlow.addEdge(cur, i, cnt[i][j] - existEdge);
      }
    int flow = maxFlow.dinic(s, t);
    if (flow != expected)
      continue;
    vector<vector<vector<pair<int, int>>>> edges(l + 1, vector<vector<pair<int, int>>>(l + 1));
    for (int i = 2; i <= l; ++i) {
      int j = orientation[i];
      edges[min(i, j)][max(i, j)].emplace_back(fst[orientation[i]], fst[i]);
    }
    for (int i = 1; i <= l; ++i) {
      for (int rep = 0; rep < cnt[i][i]; ++rep) {
        edges[i][i].emplace_back(fst[i], fst[i] + 1);
        ++fst[i];
      }
    }
    for (int i = 1; i < l; ++i)
      for (int j = i + 1; j <= l; ++j) {
        for (int rep = 0; rep < ref[i][j].get(); ++rep)
          edges[i][j].emplace_back(fst[i], ++fst[j]);
        for (int rep = 0; rep < ref[j][i].get(); ++rep)
          edges[i][j].emplace_back(++fst[i], fst[j]);
      }
    for (auto [x, y] : lengths) {
      if (x <= y) {
        cout << edges[x][y].back().first << ' ' << edges[x][y].back().second << '\n';
        edges[x][y].pop_back();
      } else {
        cout << edges[y][x].back().second << ' ' << edges[y][x].back().first << '\n';
        edges[y][x].pop_back();
      }
    }
    return 0;
  }
  cout << "-1\n";

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
