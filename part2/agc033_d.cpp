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

const int N = 190;

int n, m;
char s[N][N];
int sum[N][N];
int dp[N][N][N], tmp[N][N][N];

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif

  scanf("%d%d", &n, &m);
  for (int i = 1; i <= n; ++i)
    scanf("%s", s[i] + 1);
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= m; ++j)
      sum[i][j] = (s[i][j] == '#') + sum[i - 1][j] + sum[i][j - 1] - sum[i - 1][j - 1];
  for (int i = 1; i <= n; ++i)
    for (int j = i; j <= n; ++j) {
      int l = 0;
      for (int k = 1; k <= m; ++k) {
        ++l;
        while (true) {
          int expect = (j - i + 1) * l, actual = sum[j][k] - sum[j][k - l] - sum[i - 1][k] + sum[i - 1][k - l];
          if (actual == 0 || actual == expect)
            break;
          --l;
        }
        dp[i][j][k] = l;
      }
    }
  int ans = 0;
  while (dp[1][n][m] != m) {
    ++ans;
    memset(tmp, 0, sizeof(tmp));
    for (int i = 1; i <= n; ++i) {
      for (int k = 1; k <= m; ++k) {
        int ptr = i;
        for (int j = i + 1; j <= n; ++j) {
          while (ptr < j - 1 && dp[i][ptr + 1][k] > dp[ptr + 2][j][k])
            ++ptr;
          tmp[i][j][k] = max(min(dp[i][ptr][k], dp[ptr + 1][j][k]), min(dp[i][ptr + 1][k], dp[ptr + 2][j][k]));
        }
      }
    }
    for (int i = 1; i <= n; ++i)
      for (int j = i; j <= n; ++j)
        for (int k = m; k; --k)
          dp[i][j][k] = max(tmp[i][j][k], dp[i][j][k] + dp[i][j][k - dp[i][j][k]]);
  }
  printf("%d\n", ans);

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
