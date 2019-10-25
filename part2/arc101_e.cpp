#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;

const int N = 5010, P = 1000000007;

int n;
int fac[N];
int f[N][N];
int sub[N];
vector<int> g[N];

void dfs(int u) {
  sub[u] = 1;
  f[u][1] = 1;
  for (int v : g[u])
    if (!sub[v]) {
      dfs(v);
      if (!(sub[v] & 1)) {
        for (int i = 1; i <= sub[v] / 2; ++i)
          f[v][0] = (f[v][0] + f[v][i * 2] * (ll)fac[i]) % P;
        f[v][0] = (P - f[v][0]) % P;
      }
      static int tmp[N];
      memset(tmp, 0, sizeof(tmp));
      for (int i = 0; i <= sub[u]; ++i)
        for (int j = 0; j <= sub[v]; ++j)
          tmp[i + j] = (tmp[i + j] + f[u][i] * (ll)f[v][j]) % P;
      memcpy(f[u], tmp, sizeof(tmp));
      sub[u] += sub[v];
    }
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
#endif

  scanf("%d", &n);
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    scanf("%d%d", &u, &v);
    g[u].push_back(v);
    g[v].push_back(u);
  }
  fac[0] = 1;
  for (int i = 1; i <= n; ++i)
    fac[i] = fac[i - 1] * (2 * i - 1LL) % P;
  dfs(1);
  int ans = 0;
  for (int i = 0; i <= n / 2; ++i)
    ans = (ans + fac[i] * (ll)f[1][i * 2]) % P;
  printf("%d\n", ans);

  return 0;
}
