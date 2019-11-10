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

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
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

const int N = 16, P = 1000000007;

int n, m;
int a[N + 1], dp[N + 1][1 << N];
int fac[(1 << N) + 5], ifac[(1 << N) + 5];

void prepare(int n) {
  ifac[1] = 1;
  for (int i = 2; i <= n; ++i) ifac[i] = -(P / i) * (ll)ifac[P % i] % P + P;
  ifac[0] = 1;
  for (int i = 1; i <= n; ++i) ifac[i] = ifac[i - 1] * (ll)ifac[i] % P;
  fac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * (ll)i % P;
}

int fall(int n, int m) {
  if (m < 0 || m > n) return 0;
  return fac[n] * (ll)ifac[n - m] % P;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 1; i <= m; ++i)
    cin >> a[i];
  prepare(1 << n);
  dp[m][0] = 1;
  for (int i = m; i; --i) {
    memcpy(dp[i - 1], dp[i], sizeof(dp[i]));
    for (int j = 0; j < 1 << n; ++j)
      for (int k = 0; k < n; ++k)
        if (!((j >> k) & 1))
          dp[i - 1][j | (1 << k)] = (dp[i - 1][j | (1 << k)] + (ll)(P - fall((1 << n) - a[i] - j, (1 << k) - 1)) * dp[i][j] % P * (1 << k)) % P;
  }
  int ans = 0;
  for (int i = 0; i < 1 << n; ++i)
    ans = (ans + dp[0][i] * (ll)fac[(1 << n) - 1 - i]) % P;
  ans = ans * (1LL << n) % P;
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
