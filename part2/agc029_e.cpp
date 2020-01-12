#include <bits/stdc++.h>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef vector<int> vi;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
}

ostream &operator<<(ostream &os, const pair<char, int> &unit) {
  return os << unit.first << "^" << unit.second;
}

template<class T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

const int N = 200010;

int n;
vector<int> g[N];
int prt[N], mx[N], ans[N];
bool vis[N];

void dfs(int u) {
  vis[u] = true;
  for (int v : g[u])
    if (!vis[v]) {
      prt[v] = u;
      mx[v] = max(mx[u], u);
      dfs(v);
    }
}

int f(int u, int x) {
  static unordered_map<ll, int> memo;
  auto it = memo.find(u * (ll)n + x);
  if (it == memo.end()) {
    int ret = 1;
    for (int v : g[u])
      if (prt[v] == u && v < x)
        ret += f(v, x);
    it = memo.insert(make_pair(u * (ll)n + x, ret)).first;
  }
  return it->second;
}

void dfs2(int u) {
  if (u != 1) {
    if (u > mx[prt[u]])
      ans[u] = ans[prt[u]] + f(u, mx[u]);
    else
      ans[u] = ans[prt[u]] + f(u, mx[u]) - f(u, mx[prt[u]]);
  }
  for (int v : g[u])
    if (prt[v] == u)
      dfs2(v);
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs(1);
  dfs2(1);
  cout << vi(ans + 2, ans + n + 1);

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
      -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
