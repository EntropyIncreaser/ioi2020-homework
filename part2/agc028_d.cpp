#include <cstdio>

#include <algorithm>

using namespace std;

typedef long long ll;

const int N = 310, P = 1000000007;

int n;

int match[N * 2], pre[N], sum[N * 2];
bool ab[N * 2][N * 2];
int con[N * 2][N * 2], spec[N * 2][N * 2];

int main() {
  int k;
  scanf("%d%d", &n, &k);
  while (k--) {
    int a, b;
    scanf("%d%d", &a, &b);
    match[a] = b;
    match[b] = a;
  }
  pre[0] = 1;
  for (int i = 1; i <= n; ++i)
    pre[i] = pre[i - 1] * (ll)(i * 2 - 1) % P;
  for (int l = 1; l <= n * 2; ++l) {
    int myn = n * 2, mex = 1;
    for (int r = l; r <= n * 2; ++r) {
      if (match[r]) {
        myn = min(myn, match[r]);
        mex = max(mex, match[r]);
      }
      ab[l][r] = myn >= l && mex <= r;
    }
  }
  for (int i = 1; i <= n * 2; ++i)
    sum[i] = sum[i - 1] + bool(match[i]);
  int ans = 0;
  for (int len = 2; len <= n * 2; len += 2)
    for (int l = 1; l + len - 1 <= n * 2; ++l) {
      int r = l + len - 1;
      if (!ab[l][r])
        continue;
      con[l][r] = pre[(len - sum[r] + sum[l - 1]) >> 1];
      spec[l][r] = con[l][r];
      for (int i = l + 1; i < r; i += 2)
        spec[l][r] = (P + spec[l][r] - spec[l][i] * (ll)con[i + 1][r] % P) % P;
      ans = (ans + spec[l][r] * (ll)pre[(n * 2 - len - sum[l - 1] - sum[n * 2] + sum[r]) >> 1]) % P;
    }
  printf("%d\n", ans);
  return 0;
}
