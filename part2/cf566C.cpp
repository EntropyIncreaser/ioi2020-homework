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

const int N = 200010;

int n;
int w[N], sub[N], heav[N];
vector<pair<int, int>> g[N];
bool vis[N];
int ql, qr;
int que[N], prt[N];

double tot, totDer;
double der[N];

void dfs(int u, int p, int dist, int b) {
  double v = sqrt(dist) * w[u];
  tot += v * dist;
  totDer += v;
  der[b] += v;
  for (const auto& pr : g[u])
    if (pr.first != p)
      dfs(pr.first, u, dist + pr.second, b);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i)
    cin >> w[i];
  for (int rep = 1; rep < n; ++rep) {
    int u, v, w;
    cin >> u >> v >> w;
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  }
  pair<double, int> ans(numeric_limits<double>::max(), -1);
  int u = 1;
  while (!vis[u]) {
    ql = qr = 0;
    que[qr++] = u;
    prt[u] = 0;
    while (ql < qr) {
      u = que[ql++];
      vis[u] = true;
      for (const auto& pr : g[u])
        if (!vis[pr.first]) {
          prt[pr.first] = u;
          que[qr++] = pr.first;
        }
    }
    for (int i = 0; i < qr; ++i) {
      vis[que[i]] = false;
      sub[que[i]] = 1;
      heav[que[i]] = 1;
    }
    int cent = que[qr - 1];
    for (int i = qr - 1; i >= 0; --i) {
      u = que[i];
      if (i)
        sub[prt[u]] += sub[u];
      heav[prt[u]] = max(heav[prt[u]], sub[u]);
      heav[u] = max(heav[u], qr - sub[u]);
      if (heav[u] < heav[cent])
        cent = u;
    }
    vis[cent] = true;
    tot = 0;
    totDer = 0;
    u = cent;
    for (const auto& pr : g[cent]) {
      der[pr.first] = 0;
      dfs(pr.first, cent, pr.second, pr.first);
    }
    for (const auto& pr : g[cent]) {
      if (totDer - 2 * der[pr.first] < 0) {
        u = pr.first;
      }
    }
    ans = min(ans, make_pair(tot, cent));
  }

  cout.precision(10);
  cout << ans.second << ' ' << ans.first << '\n';

  return 0;
}
