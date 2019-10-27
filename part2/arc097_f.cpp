#include <cstdio>
#include <cstring>

#include <algorithm>
#include <queue>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

struct E {
  int v;
  E* next;
};

const int N = 200010;

int n, good;
char col[N];
int deg[N], dp1[N], dp2[N];
bool vis[N], ct[N], f[N];

E epool[N * 2], *etop;
E* g[N];

void adde(int u, int v) {
  E* p = etop++;
  p->v = v;
  p->next = g[u];
  g[u] = p;
  ++deg[u];
}

void dfs1(int u) {
  vis[u] = true;
  for (E* p = g[u]; p; p = p->next)
    if (!vis[p->v] && !ct[p->v]) {
      dfs1(p->v);
      int upd1 = dp1[p->v] + -1 + (f[u] ? 1 : -1), upd2 = dp2[p->v] + -1 + (f[p->v] ? 1 : -1);
      good = min({good, dp1[u] + upd2, dp2[u] + upd1});
      dp1[u] = min(dp1[u], upd1);
      dp2[u] = min(dp2[u], upd2);
    }
}

int main() {
  int tc = 1;
  while (tc--) {
    memset(g, 0, sizeof(g));
    memset(ct, 0, sizeof(ct));
    memset(deg, 0, sizeof(deg));
    etop = epool;
    scanf("%d", &n);
    for (int rep = 1; rep < n; ++rep) {
      int u, v;
      scanf("%d%d", &u, &v);
      adde(u, v);
      adde(v, u);
    }
    scanf("%s", col + 1);
    for (int i = 1; i <= n; ++i)
      col[i] = col[i] == 'B';
    if (count(col + 1, col + n + 1, 1) == n) {
      puts("0");
      continue;
    }
    queue<int> q;
    for (int i = 1; i <= n; ++i) {
      if (deg[i] == 1 && col[i])
        q.push(i);
    }
    int cnt = n;
    while (!q.empty()) {
      --cnt;
      int u = q.front();
      ct[u] = true;
      q.pop();
      for (E* p = g[u]; p; p = p->next)
        if (--deg[p->v] == 1 && col[p->v])
          q.push(p->v);
    }
    for (int i = 1; i <= n; ++i)
      f[i] = col[i] ^ (deg[i] & 1);
    int rt = find(col + 1, col + n + 1, 0) - col;
    int ans = 2 * (cnt - 1);
    for (int i = 1; i <= n; ++i)
      if (!ct[i])
        ans += !f[i];
    memset(vis, 0, sizeof(vis));
    memset(dp1, 0, sizeof(dp1));
    memset(dp2, 0, sizeof(dp2));
    good = 0;
    dfs1(rt);
    printf("%d\n", good + ans);
  }
  return 0;
}
