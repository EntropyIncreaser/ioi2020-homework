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

const int N = 41;

int n;
char a[N][N];
ll dp[N][N][N], dp2[N][N][N][N], dp3[N][N][N];

ll dfs2(int l1, int r1, int l2, int r2);

ll dfs(int l, int m, int r) {
  if (dp[l][m][r] != -1)
    return dp[l][m][r];
  if (l == r)
    return dp[l][m][r] = 1;
  dp[l][m][r] = 0;
  for (int i = l; i < m; i += 2)
    for (int j = r; j > m; j -= 2)
      dp[l][m][r] += dfs2(l, i, j, r) * dfs(i + 1, m, j - 1);
  return dp[l][m][r];
}

ll dfs3(int p, int l, int r) {
  if (dp3[p][l][r] != -1)
    return dp3[p][l][r];
  dp3[p][l][r] = 0;
  for (int i = l; i <= r; ++i)
    if (a[p][i])
      dp3[p][l][r] += dfs(l, i, r);
  return dp3[p][l][r];
}

ll dfs2(int l1, int r1, int l2, int r2) {
  if (dp2[l1][r1][l2][r2] != -1)
    return dp2[l1][r1][l2][r2];
  dp2[l1][r1][l2][r2] = 0;
  for (int i = l1; i <= r1; ++i)
    dp2[l1][r1][l2][r2] += dfs(l1, i, r1) * dfs3(i, l2, r2);
  return dp2[l1][r1][l2][r2];
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
#endif

  scanf("%d", &n);
  for (int i = 0; i < n * 2; ++i)
    scanf("%s", a[i]);
  for (int i = 0; i < n * 2; ++i)
    for (int j = 0; j < n * 2; ++j)
      a[i][j] -= '0';
  
  memset(dp, -1, sizeof(dp));
  memset(dp2, -1, sizeof(dp2));
  memset(dp3, -1, sizeof(dp3));
  printf("%lld\n", dfs3(0, 1, n * 2 - 1));

  return 0;
}
