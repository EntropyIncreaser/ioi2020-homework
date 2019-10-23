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

const int N = 110, P = 1000000007;

int n;
int h[N], stk[N], ch[N][2];
int dp[N][2];

int mpow(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll)x % P;
    x = x * (ll)x % P;
    k >>= 1;
  }
  return ret;
}

void dfs(int x, int v) {
  dp[x][0] = 1;
  for (int k = 0; k < 2; ++k)
    if (ch[x][k]) {
      int c = ch[x][k];
      dfs(c, h[x]);
      dp[x][1] = (dp[x][1] * (ll)(dp[c][0] + dp[c][1]) * 2 + dp[x][0] * ((ll)dp[c][0] + dp[c][1] * 2)) % P;
      dp[x][0] = dp[x][0] * (ll)dp[c][0] % P;
    }
  dp[x][0] = dp[x][0] * (ll)mpow(2, h[x] - v) % P;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; ++i)
    cin >> h[i];
  int cnt = 0;
  for (int i = 1; i <= n; ++i) {
    int cur = 0;
    while (cnt && h[stk[cnt]] > h[i]) {
      ch[stk[cnt]][1] = cur;
      cur = stk[cnt--];
    }
    ch[i][0] = cur;
    stk[++cnt] = i;
  }
  int cur = 0;
  while (cnt > 1) {
    ch[stk[cnt]][1] = cur;
    cur = stk[cnt--];
  }
  int rt = stk[1];
  ch[rt][1] = cur;
  dfs(rt, 1);
  int ans = (dp[rt][0] + dp[rt][1]) % P * 2 % P;
  cout << ans << '\n';

  return 0;
}
