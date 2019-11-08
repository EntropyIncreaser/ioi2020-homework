#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
 
#include <algorithm>
#include <tuple>
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

const int N = 1510, K = 100010, P = 1000000007;

int n, k;
int f[N], x[N], y[N], xx[N], yy[N], vx[N], vy[N], sf[N];
int fac[K], ifac[K];

int norm(int x) { return x >= P ? (x - P) : x; }

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int& x, int y) {
  if ((x -= y) < 0)
    x += P;
}

void exGcd(int a, int b, int& x, int& y) {
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

int binom(int n, int m) {
  if (m < 0 || m > n) return 0;
  return fac[n] * (ll)ifac[m] % P * ifac[n - m] % P;
}

int mpow(int x, int k) {
  if (k < 0) return 0;
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll)x % P;
    x = x * (ll)x % P;
    k >>= 1;
  }
  return ret;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int re, m, k, a, b;
  cin >> re >> m >> a >> b >> k;
  int p = a * (ll)inv(b) % P, q = norm(P + 1 - p);
  ifac[1] = 1;
  for (int i = 2; i <= k; ++i) ifac[i] = -(P / i) * (ll)ifac[P % i] % P + P;
  ifac[0] = 1;
  for (int i = 1; i <= k; ++i) ifac[i] = ifac[i - 1] * (ll)ifac[i] % P;
  fac[0] = 1;
  for (int i = 1; i <= k; ++i) fac[i] = fac[i - 1] * (ll)i % P;
  for (int i = 1; i <= m; ++i)
    f[i] = mpow(p, i - 1) * (ll)mpow(q, k - i + 1) % P * (ll)binom(k, i - 1) % P;
  for (int i = 1; i <= m; ++i)
    sf[i] = norm(sf[i - 1] + f[i]);
  int s = 1;
  while (re--) {
    memset(xx, 0, sizeof(xx));
    memset(yy, 0, sizeof(yy));
    int ss = 0;
    for (int i = 1; i <= m; ++i)
      vx[i] = norm(vx[i - 1] + x[i - 1]);
    for (int i = m; i; --i)
      vy[i] = norm(vy[i + 1] + y[i + 1]);
    for (int i = 1; i <= m; ++i) {
      ss = (ss + sf[i] * (ll)f[m - i + 1] % P * s) % P;
      ss = (ss + (P - vx[i]) * (ll)f[i] % P * sf[m - i + 1]) % P;
      ss = (ss + (P - vy[i]) * (ll)f[m - i + 1] % P * sf[i]) % P;
    }
    int sum = 0, sf = 0;
    for (int i = m; i; --i) {
      sum = (sum + (P + s - vy[i]) * (ll)f[m - i + 1]) % P;
      add(sf, f[m - i + 1]);
      xx[m - i + 1] = yy[i] = f[i] * (ll)(sum + P - sf * (ll)vx[i] % P) % P;
    }
    memcpy(x, xx, sizeof(x));
    memcpy(y, yy, sizeof(y));
    s = ss;
  }
  cout << s << '\n';

  return 0;
}
