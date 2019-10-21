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

const int N = 210, P = 10007;

int n, m;
char s[N];
int f[N][N][N], g[N][N][N], h[N][N][N];
int rec[N * 3 / 2], tmp[N * 3 / 2], a[N * 3 / 2], coe[N * 3 / 2];

void mul(int* a, int n, int x) {
  for (int i = n; i; --i)
    a[i] = (a[i] + a[i - 1] * (P - x)) % P;
}

void quo(int* a, int n, int x) {
  for (int i = 1; i <= n; ++i)
    a[i] = (a[i] + a[i - 1] * x) % P;
}

void pow(int t) {
  if (t == 0) {
    coe[0] = 1;
    return;
  }
  if (t == 1) {
    coe[1] = 1;
    return;
  }
  pow(t >> 1);
  static int pmt[N * 3];
  memset(pmt, 0, sizeof(pmt));
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < m; ++j)
      pmt[i + j] = (pmt[i + j] + coe[i] * coe[j]) % P;
  for (int i = m * 2 - 2; i >= m; --i)
    for (int j = 1; j <= m; ++j)
      pmt[i - j] = (pmt[i - j] + (P - rec[j]) * pmt[i]) % P;
  memcpy(coe, pmt, sizeof(int) * m);
  if (t & 1) {
    memmove(coe + 1, coe, sizeof(int) * m);
    coe[0] = 0;
    for (int j = 1; j <= m; ++j)
      coe[m - j] = (coe[m - j] + (P - rec[j]) * coe[m]) % P;
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> (s + 1) >> t;
  n = strlen(s + 1);

  for (int len = 0; len <= n; ++len)
    for (int l = 1, r = len; r <= n; ++l, ++r) {
      if (len == 0) {
        f[l][r][0] = 1;
      } else if (len == 1) {
        h[l][r][1] = 1;
        g[l][r][1] = 1;
      } else if (s[l] == s[r]) {
        for (int i = 1; i <= len; ++i) {
          f[l][r][i] = f[l + 1][r - 1][i - 1];
          g[l][r][i] = g[l + 1][r - 1][i - 1];
          h[l][r][i] = h[l + 1][r - 1][i - 1];
        }
      } else {
        for (int i = 1; i <= len; ++i) {
          f[l][r][i] = (f[l + 1][r][i - 1] + f[l][r - 1][i - 1]) % P;
          g[l][r][i] = (g[l + 1][r][i - 1] + g[l][r - 1][i - 1]) % P;
          h[l][r][i] = (h[l + 1][r][i - 1] + h[l][r - 1][i - 1]) % P;
        }
      }
    }
  rec[0] = 1;
  for (int rep = 0; rep < n; ++rep)
    mul(rec, ++m, 24);
  for (int rep = 0; rep < (n + 1) / 2; ++rep)
    mul(rec, ++m, 25);
  mul(rec, ++m, 26);

  tmp[0] = 1;
  quo(tmp, m, 26);
  for (int rep = 0; rep < n; ++rep)
    quo(tmp, m, 24);
  for (int i = n - 1; i * 2 - n >= 0; --i) {
    int use = i * 2 - n;
    mul(tmp, m, 24);
    mul(tmp, m, 24);
    quo(tmp, m, 25);
    if (f[1][n][i]) {
      if ((use & 1) == (t & 1))
        for (int j = use / 2; j <= m; ++j)
          a[j] = (a[j] + tmp[j - use / 2] * f[1][n][i]) % P;
      else {
        ++use;
        for (int j = use / 2; j <= m; ++j)
          a[j] = (a[j] + tmp[j - use / 2] * f[1][n][i] % P * 26) % P;
      }
    }
  }

  memset(tmp, 0, sizeof(tmp));
  tmp[0] = 1;
  quo(tmp, m, 25);
  for (int rep = 1; rep < n; ++rep)
    quo(tmp, m, 24);
  for (int i = n; i * 2 - n - 1 >= 0; --i) {
    int use = i * 2 - n - 1;
    if (g[1][n][i] && (use & 1) == (t & 1))
      for (int j = use / 2; j <= m; ++j)
        a[j] = (a[j] + tmp[j - use / 2] * g[1][n][i]) % P;
    mul(tmp, m, 24);
    mul(tmp, m, 24);
    quo(tmp, m, 25);
  }

  memset(tmp, 0, sizeof(tmp));
  tmp[0] = 1;
  quo(tmp, m, 26);
  quo(tmp, m, 25);
  for (int rep = 1; rep < n; ++rep)
    quo(tmp, m, 24);
  for (int i = n; i * 2 - n >= 0; --i) {
    int use = i * 2 - n;
    if (h[1][n][i]) {
      if ((use & 1) == (t & 1))
        for (int j = use / 2; j <= m; ++j)
          a[j] = (a[j] + tmp[j - use / 2] * h[1][n][i]) % P;
      else {
        ++use;
        for (int j = use / 2; j <= m; ++j)
          a[j] = (a[j] + tmp[j - use / 2] * h[1][n][i] % P * 26) % P;
      }
    }
    mul(tmp, m, 24);
    mul(tmp, m, 24);
    quo(tmp, m, 25);
  }

  t /= 2;
  pow(t);
  int ans = 0;
  for (int i = 0; i < m; ++i)
    ans = (ans + coe[i] * a[i]) % P;
  cout << ans << '\n';

  return 0;
}
