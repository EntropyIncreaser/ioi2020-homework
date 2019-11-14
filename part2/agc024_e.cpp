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

const int N = 310;
int M;

int n, k;
int binom[N][N], dp[N], sum[N], tmp[N];

int norm(int x) { return x >= M ? x - M : x; }

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
#endif

  scanf("%d%d%d", &n, &k, &M);
  ++n;
  for (int i = 0; i <= n; ++i) {
    binom[i][0] = 1;
    for (int j = 1; j <= i; ++j)
      binom[i][j] = norm(binom[i - 1][j - 1] + binom[i - 1][j]);
  }
  sum[1] = 1;
  for (int i = 1; i <= k; ++i) {
    memset(tmp, 0, sizeof(tmp));
    tmp[1] = 1;
    for (int j = 2; j <= n; ++j)
      for (int l = 1; l < j; ++l)
        tmp[j] = (tmp[j] + tmp[l] * (ll)sum[j - l] % M * binom[j - 2][l - 1]) % M;
    for (int j = 1; j <= n; ++j)
      sum[j] = norm(sum[j] + tmp[j]);
   }
  printf("%d\n", tmp[n]);

  return 0;
}
