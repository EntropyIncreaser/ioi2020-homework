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

const int N = 510;

int n;
ll a[N][N], dp[N][N]; 

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
#endif

  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j)
      if (i != j)
        scanf("%lld", &a[i][j]);
  for (int i = 1; i <= n; ++i)
    for (int j = i + 1; j <= n; ++j)
      a[i][j] += a[i][j - 1];
  for (int j = 1; j <= n; ++j)
    for (int i = j + 1; i <= n; ++i)
      a[i][j] += a[i - 1][j];
  for (int i = 1; i <= n; ++i) {
    ll as = 0;
    for (int j = i; j; --j) {
      dp[i][j] = 1LL << 60;
      as += a[j][i];
      ll bs = 0;
      for (int k = j != 1; k < j; ++k) {
        dp[i][j] = min(dp[i][j], dp[j - 1][k] + bs);
        bs += a[i][k] - a[j - 1][k];
      }
      dp[i][j] += as;
    }
  }
  ll ans = *min_element(dp[n] + 1, dp[n] + n + 1);
  printf("%lld\n", ans);

  return 0;
}
