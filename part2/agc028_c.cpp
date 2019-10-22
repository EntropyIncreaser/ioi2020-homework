#include <cstdio>

#include <algorithm>
#include <numeric>

using namespace std;

typedef long long ll;

const int N = 100010;

int n;
int a[N], b[N];
int arr[N * 2];
ll sum[N * 2];

int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    scanf("%d%d", &a[i], &b[i]);
  ll ans = min(accumulate(a + 1, a + n + 1, 0LL), accumulate(b + 1, b + n + 1, 0LL));
  copy(a + 1, a + n + 1, arr + 1);
  copy(b + 1, b + n + 1, arr + n + 1);
  sort(arr + 1, arr + n * 2 + 1);
  for (int i = 1; i <= n * 2; ++i)
    sum[i] = sum[i - 1] + arr[i];
  for (int i = 1; i <= n; ++i) {
    int x = lower_bound(arr + 1, arr + n * 2 + 1, a[i]) - arr,
            y = lower_bound(arr + 1, arr + n * 2 + 1, b[i]) - arr;
    if (x == y)
      ++y;
    for (int j = n; j >= n - 2; --j)
      if ((j + (x > j) + (y > j)) == n)
        ans = min(ans, sum[j] + (x > j) * arr[x] + (y > j) * arr[y]);
  }
  printf("%lld\n", ans);
  return 0;
}
