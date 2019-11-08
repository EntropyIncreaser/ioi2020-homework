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

const int N = 110;

int n;
int maxson[N], dep[N];
bool vis[N];
vector<int> g[N];

void dfs(int u) {
  vis[u] = true;
  int sons = 0;
  for (int v : g[u])
    if (!vis[v]) {
      ++sons;
      dep[v] = dep[u] + 1;
      dfs(v);
    }
  maxson[dep[u]] = max(maxson[dep[u]], sons);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  vector<pair<int, int>> edges;
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
    edges.emplace_back(u, v);
  }
  pair<int, ll> ans(n + 1, 0);
  for (int i = 1; i <= n; ++i) {
    memset(maxson, 0, sizeof(maxson));
    memset(vis, 0, sizeof(vis));
    memset(dep, 0, sizeof(dep));
    dep[i] = 0;
    dfs(i);
    int a1 = *max_element(dep + 1, dep + n + 1);
    ll a2 = 1;
    for (int i = 0; i < a1; ++i) a2 *= maxson[i];
    ans = min(ans, make_pair(a1 + 1, a2));
  }
  for (const auto& pr : edges) {
    int u = pr.first, v = pr.second;
    memset(maxson, 0, sizeof(maxson));
    memset(vis, 0, sizeof(vis));
    memset(dep, 0, sizeof(dep));
    vis[u] = vis[v] = true; dep[u] = dep[v] = 0;
    dfs(u); dfs(v);
    int a1 = *max_element(dep + 1, dep + n + 1);
    ll a2 = 2;
    for (int i = 0; i < a1; ++i) a2 *= maxson[i];
    ans = min(ans, make_pair(a1 + 1, a2));
  }
  cout << ans.first << ' ' << ans.second << '\n';

  return 0;
}
