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

const int P = 1000000007;

int norm(int x) { return x >= P ? x - P : x; }

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

int mpow(int x, int k) {
  int ret = 1;
  while (k) {
    if (k & 1)
      ret = ret * (ll)x % P;
    k >>= 1;
    x = x * (ll)x % P;
  }
  return ret;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, k, m;
  cin >> n >> k >> m;
  vi a(m);
  cin >> a;
  for (int& v : a) --v;
  function<vi(int, int, vi)> calc = [&](int n, int k, vi dp) {
    vi ret(n + 1);
    for (int i = 0; i <= n; ++i) {
      vi sum = dp, newdp(k);
      for (int j = k - 2; j; --j)
        add(sum[j], sum[j + 1]);
      ret[i] = sum[1];
      for (int j = 1; j < k; ++j)
        newdp[j] = (dp[j - 1] * (ll)(k - j + 1) + sum[j]) % P;
      dp = newdp;
    }
    return ret;
  };
  function<vi(int, int, int)> fromc = [&](int n, int k, int l) {
    vi dp(k);
    dp[l] = 1;
    return calc(n, k, dp);
  };
  function<vector<vi>(int, int)> inversedp = [&](int n, int k) {
    vector<vi> ret(n + 1);
    ret[0].assign(k, 1);
    ret[0][0] = 0;
    for (int i = 1; i <= n; ++i) {
      ret[i] = ret[i - 1];
      for (int j = 1; j < k; ++j)
        add(ret[i][j], ret[i][j - 1]);
      for (int j = 1; j < k - 1; ++j)
        ret[i][j] = (ret[i][j] + ret[i - 1][j + 1] * (ll)(k - j)) % P;
    }
    return ret;
  };
  vector<int> cnt(k);
  int tot = 0;
  bool flag = false;
  for (int i = 0; i < m; ++i) {
    if (cnt[a[i]]++ == 0) ++tot;
    if (i >= k) {
      if (--cnt[a[i - k]] == 0) --tot;
    }
    if (tot == k) {
      flag = true;
      break;
    }
  }
  if (flag) {
    int ans = (n - m + 1) * (ll)mpow(k, n - m) % P;
    cout << ans << '\n';
    return 0;
  }
  vector<bool> vis(k);
  for (int v : a)
    if (vis[v]) {
      flag = true;
      break;
    } else
      vis[v] = true;
  if (flag) {
    vis.assign(k, false);
    int lenl = 0, lenr = 0;
    for (int i = 0; i < m; ++i) {
      if (vis[a[i]]) {
        lenl = i;
        break;
      } else
        vis[a[i]] = true;
    }
    vis.assign(k, false);
    for (int i = m - 1; i >= 0; --i) {
      if (vis[a[i]]) {
        lenr = m - i - 1;
        break;
      } else
        vis[a[i]] = true;
    }
    vi l = fromc(n - m, k, lenl), r = fromc(n - m, k, lenr);
    int ans = 0, pw = mpow(k, n - m);
    for (int i = 0; i <= n - m; ++i)
      ans = (ans + (P - l[i]) * (ll)r[n - m - i] + pw) % P;
    cout << ans << '\n';
    return 0;
  }

  vector<vi> inver = inversedp(n, k), invers = inver;
  for (int i = 0; i <= n; ++i)
    for (int j = 1; j < k; ++j)
      add(invers[i][j], invers[i][j - 1]);
  int cur = 1;

  int ans = 0;
  for (int i = 0; i + m < k && i + m <= n; ++i) {
    add(ans, cur * (ll)inver[n - i - m][i + m] % P);
    for (int j = 0; j + i + m + 1 <= n; ++j)
      add(ans, cur * (ll) invers[j][i + m] % P * inver[n - j - i - m - 1][i + m] % P);
    cur = cur * (ll)(k - i - m) % P;
  }

  ans = norm(P + int((n - m + 1LL) * mpow(k, n - m) % P) - ans);
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
