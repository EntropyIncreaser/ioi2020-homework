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

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, P;
  cin >> n >> P;
  vector<int> ifac(n + 1);
  ifac[1] = 1;
  for (int i = 2; i <= n; ++i)
    ifac[i] = -(P / i) * (ll)ifac[P % i] % P + P;
  ifac[0] = 1;
  for (int i = 1; i <= n; ++i)
    ifac[i] = ifac[i - 1] * (ll)ifac[i] % P;

  vector<vector<int>> stirling2(n + 2, vector<int>(n + 2));
  stirling2[0][0] = 1;
  for (int i = 1; i <= n + 1; ++i)
    for (int j = 1; j <= i; ++j)
      stirling2[i][j] = (stirling2[i - 1][j - 1] + stirling2[i - 1][j] * (ll)j) % P;
  vector<int> pw2(n + 1), ppw2(n * n + 1);
  pw2[0] = 2;
  for (int i = 1; i <= n; ++i)
    pw2[i] = pw2[i - 1] * (ll)pw2[i - 1] % P;
  ppw2[0] = 1;
  for (int i = 1; i <= n * n; ++i)
    ppw2[i] = (ppw2[i - 1] << 1) % P;
  int ans = 0;
  for (int k = 0; k <= n; ++k)
    for (int i = 0; i <= n; ++i) {
      int coe = (stirling2[n - i][k + 1] * (k + 1LL) + stirling2[n - i][k]) % P * ifac[n - i] % P;
      if ((n - i) & 1) coe = (P - coe) % P;
      ans = (ans + coe * (ll)pw2[i] % P * ppw2[i * k] % P * ifac[i]) % P;
    }
  for (int i = 1; i <= n; ++i)
    ans = ans * (ll)i % P;
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
