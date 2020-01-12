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
typedef vector<int> vi;

// mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

template<class T>
istream &operator>>(istream &is, vector<T> &v) {
  for (T &x : v)
    is >> x;
  return is;
}

ostream &operator<<(ostream &os, const pair<char, int> &unit) {
  return os << unit.first << "^" << unit.second;
}

template<class T>
ostream &operator<<(ostream &os, const vector<T> &v) {
  if (!v.empty()) {
    os << v.front();
    for (int i = 1; i < v.size(); ++i)
      os << ' ' << v[i];
  }
  return os;
}

const int N = 110;

int P;
int fac[N], ifac[N], nifac[N], pw1[N * N], pw2[N * N];

int mpow(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll) x % P;
    k >>= 1;
    x = x * (ll) x % P;
  }
  return ret;
}

int norm(int x) { return x >= P ? x - P : x; }

void add(int &x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int &x, int y) {
  if ((x -= y) < 0)
    x += P;
}

void exGcd(int a, int b, int &x, int &y) {
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exGcd(b, a % b, y, x);
  y -= a / b * x;
}

int inv(int a) {
  int x, y;
  exGcd(a, P, x, y);
  return norm(x + P);
}

void prepare(int n) {
  fac[0] = 1;
  for (int i = 1; i <= n; ++i) fac[i] = fac[i - 1] * (ll) i % P;
  ifac[1] = 1;
  for (int i = 2; i <= n; ++i)
    ifac[i] = -(P / i) * (ll) ifac[P % i] % P + P;
  ifac[0] = 1;
  for (int i = 1; i <= n; ++i)
    ifac[i] = ifac[i - 1] * (ll) ifac[i] % P;
  memcpy(nifac, ifac, sizeof(ifac));
  for (int i = 1; i <= n; i += 2)
    nifac[i] = P - nifac[i];
}

void gpw(int* pw, int x, int l) {
  pw[0] = 1;
  for (int k = 1; k <= l; ++k)
    pw[k] = pw[k - 1] * (ll)x % P;
}

int f[N][N];

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, k;
  cin >> n >> m >> k >> P;
  prepare(max(n, m));
  f[0][0] = 1;
  for (int i = 1; i <= k; ++i) {
    gpw(pw1, inv(i), n * m);
    gpw(pw2, inv(k - i + 1), n * m);
    for (int a = 0; a <= n; ++a)
      for (int b = 0; b <= m; ++b)
        f[a][b] = f[a][b] * (ll) pw1[a * m + b * n - a * b] % P * fac[n - a] % P * fac[m - b] % P * pw2[a * b] % P;
    for (int a = 0; a <= n; ++a)
      for (int b = m; b; --b)
        for (int j = 1; j <= b; ++j)
          f[a][b] = (f[a][b] + f[a][b - j] * (ll)ifac[j]) % P;
    for (int b = 0; b <= m; ++b)
      for (int a = n; a; --a)
        for (int j = 1; j <= a; ++j)
          f[a][b] = (f[a][b] + f[a - j][b] * (ll)ifac[j]) % P;
    gpw(pw2, k - i + 1, n * m);
    for (int a = 0; a <= n; ++a)
      for (int b = 0; b <= m; ++b)
        f[a][b] = f[a][b] * (ll)pw2[a * b] % P;
    if (i == k) {
      int ans = f[n][m] * (ll)mpow(i, n * m) % P;
      cout << ans;
      break;
    }

    gpw(pw1, i, n * m);
    gpw(pw2, inv(k - i), n * m);
    for (int a = 0; a <= n; ++a)
      for (int b = 0; b <= m; ++b)
        f[a][b] = f[a][b] * (ll)pw2[a * b] % P;
    for (int a = 0; a <= n; ++a)
      for (int b = m; b; --b)
        for (int j = 1; j <= b; ++j)
          f[a][b] = (f[a][b] + f[a][b - j] * (ll)nifac[j]) % P;
    for (int b = 0; b <= m; ++b)
      for (int a = n; a; --a)
        for (int j = 1; j <= a; ++j)
          f[a][b] = (f[a][b] + f[a - j][b] * (ll)nifac[j]) % P;
    gpw(pw2, k - i, n * m);
    for (int c = 0; c <= n; ++c)
      for (int d = 0; d <= m; ++d)
        f[c][d] = f[c][d] * (ll) pw1[c * m + d * n - c * d] % P * ifac[n - c] % P * ifac[m - d] % P * pw2[c * d] % P;
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
