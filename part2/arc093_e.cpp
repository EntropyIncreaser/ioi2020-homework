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

const int N = 1010, M = 2010, P = 1000000007, L = 10;

int n, m;
ll x;
int f[N], dep[N];
bool vis[N];
int p[N][L], mx[N][L];
pair<int, pair<int, int>> e[M];
vector<pair<int, int>> g[N];

int mpow(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll)x % P;
    x = x * (ll)x % P;
    k >>= 1;
  }
  return ret;
}

int find(int x) { return f[x] == x ? x : f[x] = find(f[x]); }

void dfs(int u) {
  vis[u] = true;
  for (int i = 1; i < L; ++i)
    p[u][i] = p[p[u][i - 1]][i - 1];
  for (int i = 1; i < L; ++i)
    mx[u][i] = max(mx[u][i - 1], mx[p[u][i - 1]][i - 1]);
  for (const auto& pr : g[u]) {
    int v, w;
    tie(v, w) = pr;
    if (!vis[v]) {
      dep[v] = dep[u] + 1;
      p[v][0] = u;
      mx[v][0] = w;
      dfs(v);
    }
  }
}

int qry(int u, int v) {
  int ret = 0;
  if (dep[u] > dep[v])
    swap(u, v);
  for (int i = L - 1; i >= 0; --i)
    if ((dep[v] - dep[u]) >> i & 1) {
      ret = max(ret, mx[v][i]);
      v = p[v][i];
    }
  if (u == v)
    return ret;
  for (int i = L - 1; i >= 0; --i)
    if (p[u][i] != p[v][i]) {
      ret = max({ret, mx[u][i], mx[v][i]});
      u = p[u][i];
      v = p[v][i];
    }
  return max({ret, mx[u][0], mx[v][0]});
}

#define U second.first
#define V second.second
#define W first

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> x;
  for (int i = 1; i <= m; ++i)
    cin >> e[i].U >> e[i].V >> e[i].W;
  sort(e + 1, e + m + 1);
  iota(f + 1, f + n + 1, 1);
  ll mst = 0;
  for (int i = 1; i <= m; ++i) {
    int x = find(e[i].U), y = find(e[i].V);
    if (x != y) {
      mst += e[i].W;
      f[x] = y;
      g[e[i].U].emplace_back(e[i].V, e[i].W);
      g[e[i].V].emplace_back(e[i].U, e[i].W);
    }
  }
  dfs(1);
  int ans;
  if (mst == x) {
    int a = 0;
    for (int i = 1; i <= m; ++i)
      if (e[i].W == qry(e[i].U, e[i].V))
        ++a;
    ans = (mpow(2, a) + P - 2LL) * mpow(2, m - a) % P;
  } else if (mst < x) {
    int a = 0, b = 0;
    for (int i = 1; i <= m; ++i) {
      ll val = mst - qry(e[i].U, e[i].V) + e[i].W;
      if (val < x)
        ++a;
      else if (val == x)
        ++b;
    }
    ans = (mpow(2, b) + P - 1LL) * mpow(2, m - a - b) % P * 2 % P;
  } else
    ans = 0;
  cout << ans << '\n';

  return 0;
}
