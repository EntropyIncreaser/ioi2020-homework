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

const int N = 5010;
const ll INF = 1LL << 60;

int n, a, b;
int p[N];
ll dp[N][N];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> a >> b;
  for (int i = 1; i <= n; ++i)
    cin >> p[i];
  for (int i = 1; i <= n; ++i) {
    fill(dp[i], dp[i] + n + 1, INF);
    for (int j = 0; j <= n; ++j) {
      if (p[i] <= j)
        dp[i][j] = min(dp[i - 1][p[i] - 1], dp[i - 1][j] + b);
      else
        dp[i][j] = dp[i - 1][j] + a;
      if (j)
        dp[i][j] = min(dp[i][j], dp[i][j - 1]);
    }
  }
  cout << dp[n][n] << '\n';

  return 0;
}
