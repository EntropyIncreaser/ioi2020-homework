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

int M;

void add(int& x, int y) {
  if ((x += y) >= M)
    x -= M;
}

void sub(int& x, int y) {
  if ((x -= y) < 0)
    x += M;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n >> M;

  vector<int> low(n), up(n * 2);
  for (int i = 0; i < n * 2; ++i)
    up[i] = sqrt(n * n * 4 - i * i);
  for (int i = 0; i < n; ++i)
    low[i] = ceil(sqrt(n * n - i * i)) - 1;
  up[0] = n * 2 - 1;
  int ans = 0;

  vector<vector<int>> pre(n, vector<int>(n));
  vector<int> pt(n);
  int ptr = n * 2 - 1;
  for (int i = n - 1; i >= 0; --i) {
    int qtr = ptr;
    while (qtr >= n && up[qtr] <= low[i]) --qtr;
    for (int j = 0; j < n - i; ++j) {
      int res = 1;
      for (int k = ptr; k > qtr; --k)
        res = res * (up[k] + 1LL - (n * 2 - k - 1 + j)) % M;
      pre[i][j] = res;
    }
    ptr = qtr;
    pt[i] = n * 2 - ptr - 1;
  }
  for (int i = 0; i <= n; ++i) {
    vector<int> dp(i + 1);
    dp[0] = 1;
    for (int j = n - 1; j >= 0; --j) {
      for (int k = 0; k <= i; ++k)
        dp[k] = dp[k] * (ll)pre[j][k] % M;
      for (int k = i; k >= 0; --k) {
        dp[k] = dp[k] * (up[j] + 1LL - (n * 2 - j - 1 + i - k)) % M;
        if (k)
          add(dp[k], dp[k - 1] * (low[j] + 1LL - (pt[j] + (k - 1))) % M);
      }
    }
    int res = dp[i];
    for (int j = n; j <= ptr; ++j)
      res = res * (up[j] + 1LL - (n * 2 - j - 1 + i)) % M;
    if (i & 1)
      sub(ans, res);
    else
      add(ans, res);
  }
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
