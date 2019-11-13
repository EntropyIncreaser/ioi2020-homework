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

const int N = 200010, P = 1000000007;

int n, m;
char s[N];

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int& x, int y) {
  if ((x -= y) < 0)
    x += P;
}

int norm(int x) {
  return x >= P ? x - P : x;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> (s + 1);
  char a = s[1];
  if (count(s + 1, s + m + 1, a) == m) {
    static int dp[N][2][2];
    dp[1][0][0] = dp[1][1][1] = 1;
    for (int i = 2; i <= n; ++i)
      for (int j = 0; j < 2; ++j) {
        add(dp[i][j][0], dp[i - 1][j][0]);
        add(dp[i][j][0], dp[i - 1][j][1]);
        add(dp[i][j][1], dp[i - 1][j][0]);
      }
    int ans = norm(norm(dp[n][0][0] + dp[n][0][1]) + dp[n][1][0]);
    cout << ans << '\n';
    return 0;
  }
  int p = 1;
  while (s[p] == a) ++p;
  int len = p - 1;
  if (len % 2 == 0) ++len;
  int cur = 0;
  for (; p <= m; ++p)
    if (s[p] != a) {
      if (cur & 1)
        len = min(len, cur);
      cur = 0;
    } else
      ++cur;
  static int dp[N], sum[N];
  dp[1] = 1;
  for (int i = 2; i <= n; ++i) {
    sum[i - 1] = dp[i - 1];
    if (i - 3 >= 0)
      add(sum[i - 1], sum[i - 3]);
    dp[i] = sum[i - 2];
    if (i - len - 3 >= 0)
      sub(dp[i], sum[i - len - 3]);
  }
  int ans = 0;
  for (int i = 1; i <= n; ++i)
    if ((n - i) <= len && ((n - i) & 1))
      ans = (ans + dp[i] * (n - i + 1LL)) % P;
  cout << ans << '\n';

  return 0;
}
