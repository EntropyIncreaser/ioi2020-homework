#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
 
#include <algorithm>
#include <tuple>
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

const int N = 100010;

int n;
int dis[N], cnt[N];
bool vis[N];
vector<int> g[N];

void dfs1(int u) {
  vis[u] = true;
  for (int v : g[u])
    if (!vis[v]) {
      dis[v] = dis[u] + 1;
      dfs1(v);
    }
}

int dfn;
int tmp[N];

void dfs2(int u, int t) {
  vis[u] = true;
  for (int v : g[u])
    if (!vis[v] && (g[v].size() >= 2 || v == t)) {
      int link = (int)g[v].size() - (v != t);
      --dfn;
      tmp[dfn] = dfn - link + 2;
      while (--link) {
        --dfn;
        tmp[dfn] = dfn + 2;
      }
      dfs2(v, t);
      break;
    }
}

vector<int> solve(int s, int t) {
  memset(vis, 0, sizeof(vis));
  dfn = n - 1;
  tmp[dfn] = n;
  dfs2(s, t);
  return vector<int>(tmp, tmp + n);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  if (n == 2) {
    cout << "1 2\n";
    return 0;
  }
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs1(1);
  int rt = 0;
  for (int i = 1; i <= n; ++i)
    if (g[i].size() >= 2 && (rt == 0 || dis[i] > dis[rt]))
      rt = i;
  memset(vis, 0, sizeof(vis));
  dis[rt] = 0;
  dfs1(rt);
  int tr = rt;
  for (int i = 1; i <= n; ++i)
    if (g[i].size() >= 2) {
      if (cnt[dis[i]]++) {
        cout << "-1\n";
        return 0;
      }
      if (dis[i] > dis[tr])
        tr = i;
    }
  int s = 0, t = 0;
  for (int v : g[rt])
    if (g[v].size() < 2) {
      s = v;
      break;
    }
  for (int v : g[tr])
    if (g[v].size() < 2 && v != s) {
      t = v;
      break;
    }
  cout << min(solve(s, t), solve(t, s)) << '\n';

  return 0;
}
