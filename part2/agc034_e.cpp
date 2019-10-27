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

const int N = 2010;

int n;
char s[N];
bool vis[N];
int sub[N];
vector<int> g[N];

int iabs(int x) { return x < 0 ? -x : x; }

pair<int, int> dfs(int u) {
  vis[u] = true;
  pair<int, int> ret(0, 0);
  sub[u] = s[u] == '1';
  for (int v : g[u])
    if (!vis[v]) {
      auto cur = dfs(v);
      sub[u] += sub[v];
      cur.first += sub[v];
      cur.second += sub[v];
      if (max(cur.first, ret.first) <= min(cur.second, ret.second))
        ret = make_pair((cur.first ^ ret.first) & 1, cur.second + ret.second);
      else
        ret = make_pair(iabs(cur.first - ret.first), cur.second + ret.second);
    }
  return ret;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> (s + 1);
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  int ans = -1;
  for (int u = 1; u <= n; ++u) {
    memset(sub, 0, sizeof(sub));
    memset(vis, 0, sizeof(vis));
    auto res = dfs(u);
    if (res.first == 0)
      if (ans == -1 || ans > res.second / 2)
        ans = res.second / 2;
  }
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
