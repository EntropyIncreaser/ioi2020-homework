#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstring>

#include <algorithm>
#include <numeric>
#include <functional>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;

const int N = 110;

int n;
int a[N], l[N];
int pos[N * 3], p[N][3];
int cn[N];
pair<int, int> tmp[N * 3];

int dp[N * 3], induce[N * 3][N * 3], myn[N * 3][N * 3];

void chkMin(int& x, int y) {
  if (x > y)
    x = y;
}

void chkMax(int& x, int y) {
  if (x < y)
    x = y;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
#endif

  scanf("%d", &n);
  int tot = 0;
  for (int i = 1; i <= n; ++i) {
    scanf("%d%d", &a[i], &l[i]);
    pos[++tot] = a[i] - l[i];
    tmp[tot] = make_pair(pos[tot], i);
    pos[++tot] = a[i];
    tmp[tot] = make_pair(pos[tot], i);
    pos[++tot] = a[i] + l[i];
    tmp[tot] = make_pair(pos[tot], i);
  }
  sort(pos + 1, pos + n * 3 + 1);
  sort(tmp + 1, tmp + n * 3 + 1);
  for (int i = 1; i <= n * 3; ++i)
    p[tmp[i].second][cn[tmp[i].second]++] = i;
  pos[n * 3 + 1] = 1 << 30;
  for (int i = 1; i <= n * 3 + 1; ++i)
    for (int j = 1; j <= n * 3 + 1; ++j)
      myn[i][j] = i;
  for (int i = 1; i <= n; ++i)
    chkMin(myn[p[i][1]][p[i][1]], p[i][0]);
  for (int i = 1; i <= n * 3 + 1; ++i)
    for (int j = i + 1; j <= n * 3 + 1; ++j)
      myn[i][j] = min(myn[i][j - 1], myn[j][j]);
  for (int i = 1; i <= n * 3 + 1; ++i)
    for (int j = i; j <= n * 3 + 1; ++j) {
      if (i == j) {
        induce[i][j] = i;
        continue;
      }
      induce[i][j] = i;
      induce[i][j] = induce[myn[i][j]][j];
    }
  for (int i = 1; i <= n * 3 + 1; ++i) {
    for (int j = 1; j <= n; ++j)
      if (p[j][1] < i) {
        int pp = induce[p[j][0]][p[j][1]];
        chkMax(dp[i], dp[pp] + pos[p[j][1]] - pos[pp]);
        if (p[j][2] < i) {
          if (myn[p[j][1] + 1][p[j][2]] > p[j][1])
            chkMax(dp[i], dp[p[j][1]] + pos[p[j][2]] - pos[p[j][1]]);
          else {
            pp = induce[myn[p[j][1] + 1][p[j][2]]][p[j][1] - 1];
            chkMax(dp[i], dp[pp] + pos[p[j][2]] - pos[pp]);
          }
        } else {
          pp = p[j][1];
          if (p[j][1] + 1 <= i - 1)
            if (myn[p[j][1] + 1][i - 1] < p[j][1])
              pp = induce[myn[p[j][1] + 1][i - 1]][p[j][1] - 1];
          chkMax(dp[i], dp[pp] + pos[i] - pos[pp]);
        }
      }
  }
  printf("%d\n", dp[n * 3 + 1]);

  return 0;
}
