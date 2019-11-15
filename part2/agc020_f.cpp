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

template <class T>
istream& operator>>(istream& is, vector<T>& v) {
  for (T& x : v)
    is >> x;
  return is;
}

ostream& operator<<(ostream& os, const pair<char, int>& unit) {
  return os << unit.first << "^" << unit.second;
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

  int n, c;
  cin >> n >> c;
  vector<int> l(n);
  cin >> l;
  sort(l.begin(), l.end());
  vector<vector<ll>> dp(1 << (n - 1), vector<ll>(n * c + 1, 0));
  dp[0][l.back() * n] = 1;
  for (int i = 0; i < c; ++i) {
    vector<vector<ll>> sv = dp;
    for (int j = 0; j < n - 1; ++j)
      for (int s = (1 << (n - 1)) - 1; s >= 0; --s) {
        if (!((s >> j) & 1)) {
          int m = __builtin_popcount(s);
          for (int k = i * n; k <= n * c; ++k) {
            if (k % n > m) continue;
            int r = k % n;
            for (int p = 1; p <= m + 1; ++p) {
              if (r >= p)
                dp[s | (1 << j)][min(n * c, max(k + 1, (i + l[j]) * n + p))] += dp[s][k];
              else
                dp[s | (1 << j)][min(n * c, max(k, (i + l[j]) * n + p))] += dp[s][k];
            }
          }
        }
      }
    for (int k = i * n; k < (i + 1) * n; ++k) {
      vector<vector<ll>> tmp(1 << (n - 1), vector<ll>(n * c + 1, 0));
      for (int s = 0; s < 1 << (n - 1); ++s)
        tmp[s][k] = sv[s][k];
      for (int j = 0; j < n - 1; ++j)
        for (int s = (1 << (n - 1)) - 1; s >= 0; --s)
          if (!((s >> j) & 1)) {
            int m = __builtin_popcount(s);
            for (int t = i * n; t <= n * c; ++t) {
              if (t % n > m) continue;
              int r = t % n;
              for (int p = k % n + 1; p <= m + 1; ++p)
                if (r >= p)
                  tmp[s | (1 << j)][min(n * c, max(t + 1, (i + l[j]) * n + p))] += tmp[s][t];
                else
                  tmp[s | (1 << j)][min(n * c, max(t, (i + l[j]) * n + p))] += tmp[s][t];
            }
          }
      for (int s = 0; s < 1 << (n - 1); ++s)
        for (int j = 0; j <= n * c; ++j)
          dp[s][j] -= tmp[s][j];
    }
  }
  double ans = dp.back().back();
  for (int i = 1; i < n; ++i) ans /= i * c;
  printf("%.16f\n", ans);

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
