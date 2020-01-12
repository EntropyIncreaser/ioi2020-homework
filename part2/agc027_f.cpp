#include <bits/stdc++.h> 
 
#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

typedef vector<int> vi;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template <class U, class V> istream& operator>>(istream& is, pair<U, V>& p);

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

template <class U, class V>
istream& operator>>(istream& is, pair<U, V>& p) {
  return is >> p.first >> p.second;
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

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<pair<int, int>> a(n - 1), b(n - 1);
    vector<vector<int>> bdj(n + 1);
    cin >> a >> b;
    vector<int> deg(n + 1);
    for (const auto& pr : a) {
      ++deg[pr.first];
      ++deg[pr.second];
    }
    vector<vector<bool>> g(n + 1, vector<bool>(n + 1));
    for (const auto& pr : b) {
      g[pr.first][pr.second] = g[pr.second][pr.first] = true;
      bdj[pr.first].push_back(pr.second);
      bdj[pr.second].push_back(pr.first);
    }
    bool same = true;
    for (const auto& pr : a)
      if (!g[pr.first][pr.second]) {
        same = false;
        break;
      }
    if (same) {
      cout << "0\n";
      continue;
    }
    function<vi(const vector<vi>&, int)> getO = [&](const vector<vi>& adj, int rt) {
      vi ret(n + 1);
      vector<bool> vis(n + 1);
      function<void(int)> dfs = [&](int u) {
        vis[u] = true;
        for (int v : adj[u])
          if (!vis[v]) {
            ret[v] = u;
            dfs(v);
          }
      };
      dfs(rt);
      return ret;
    };
    int ans = n + 1;
    for (int i = 1; i <= n; ++i)
      if (deg[i] == 1) {
        vi bo = getO(bdj, i);
        vector<vi> ch(n + 1);
        for (int k = 1; k <= n; ++k)
          if (i != k)
            ch[bo[k]].push_back(k);
        for (int j = 1; j <= n; ++j)
          if (i != j) {
            vector<vector<int>> adj(n + 1);
            for (const auto& pr : a)
              if (pr.first != i && pr.second != i) {
                adj[pr.first].push_back(pr.second);
                adj[pr.second].push_back(pr.first);
              }
            adj[i].push_back(j);
            adj[j].push_back(i);
            vi ao = getO(adj, i);
            bool flag = true;
            for (int k = 1; k <= n; ++k)
              if (k != i && ao[k] == bo[k] && ao[ao[k]] != bo[bo[k]]) {
                flag = false;
                break;
              }
            if (!flag)
              continue;
            vi need(n + 1, -1);
            int cnt = 0;
            for (int k = 1; k <= n; ++k)
              if (ao[k] != bo[k]) {
                need[k] = 0;
                ++cnt;
              }
            for (int k = 1; k <= n; ++k)
              if (ao[k] != bo[k] && need[ao[k]] != -1)
                ++need[ao[k]];
            queue<int> q;
            
            for (int k = 1; k <= n; ++k)
              if (ao[k] != bo[k] && need[k] == 0 && need[bo[k]] == -1)
                q.push(k);
            while (!q.empty()) {
              int u = q.front(); q.pop();
              need[u] = -1;
              for (int v : ch[u])
                if (need[v] == 0)
                  q.push(v);
              if (need[ao[u]] != -1)
                if (--need[ao[u]] == 0 && need[bo[ao[u]]] == -1)
                  q.push(ao[u]);
              ao[u] = bo[u];
            }
            if (ao == bo)
              ans = min(ans, cnt + 1);
          }
        }
    cout << (ans == n + 1 ? -1 : ans) << '\n';
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
