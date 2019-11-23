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

const int P = 1000000007;

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int& x, int y) {
  if ((x -= y) < 0)
    x += P;
}

int norm(int x) { return x >= P ? x - P : x; }

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> a(n * 2), pos(n * 2, -1);
  cin >> a;
  for (int i = 0; i < n * 2; ++i)
    if (a[i] != -1)
      pos[--a[i]] = i;
  int empties = 0;
  for (int i = 0; i < n; ++i)
    if (a[i * 2] == -1 && a[i * 2 + 1] == -1)
      ++empties;
  vector<vector<int>> dp(n + 1, vector<int>(n + 1));
  dp[0][0] = 1;
  for (int i = 0; i < n * 2; ++i) {
    if (pos[i] != -1 && a[pos[i] ^ 1] != -1)
      continue;
    vector<vector<int>> newDp(n + 1, vector<int>(n + 1));
    if (pos[i] == -1) {
      for (int j = 0; j <= n; ++j)
        for (int k = 0; k <= n; ++k) {
          if (k < n)
            add(newDp[j][k + 1], dp[j][k]);
          if (j < n)
            add(newDp[j + 1][k], dp[j][k]);
          if (k)
            add(newDp[j][k - 1], dp[j][k]);
        }
    } else {
      for (int j = 0; j <= n; ++j)
        for (int k = 0; k <= n; ++k) {
          if (k < n)
            add(newDp[j][k + 1], dp[j][k]);
          if (j)
            add(newDp[j - 1][k], dp[j][k] * (ll)j % P);
        }
    }
    dp = newDp;
  }
  int ans = dp[0][0];
  for (int i = 1; i <= empties; ++i)
    ans = ans * (ll)i % P;
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
