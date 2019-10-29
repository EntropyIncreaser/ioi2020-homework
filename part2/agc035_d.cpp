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

const int N = 20;

int n;
int a[N];
map<pair<int, int>, ll> dp[N][N];

ll dfs(int l, int r, int x, int y) {
  if (l + 1 == r) return 0;
  auto it = dp[l][r].find(make_pair(x, y));
  if (it != dp[l][r].end())
    return it->second;
  ll cur = 1LL << 60;
  for (int i = l + 1; i < r; ++i)
    cur = min(cur, dfs(l, i, x, x + y) + dfs(i, r, x + y, y) + a[i] * (ll)(x + y));
  return dp[l][r][make_pair(x, y)] = cur;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i)
    cin >> a[i];
  cout << (dfs(1, n, 1, 1) + a[1] + a[n]) << '\n';

  return 0;
}
