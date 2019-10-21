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

int n, m, k, p;

const int N = 100010, M = 5010;

int h[N], a[N], cnt[M];

bool pred(ll x) {
  memset(cnt, 0, sizeof(cnt));
  ll sum = 0;
  for (int i = 0; i < n; ++i)
    if (h[i] + m * (ll)a[i] > x) {
      ll cur = (h[i] + m * (ll)a[i] - x + p - 1) / p;
      if ((sum += cur) > m * k)
        return false;
      ll hi = cur * p + x - a[i] * (ll)m;
      int j = 0;
      while (hi + a[i] * (ll)(m - j) > x) {
        ll t = (p - hi + a[i] - 1) / a[i];
        if (hi >= p)
          t = 0;
        j += t;
        hi += a[i] * (ll)t;
        hi -= p;
        ++cnt[j + 1];
      }
    }
  for (int i = m; i; --i) {
    cnt[i] += cnt[i + 1];
    if (cnt[i] > (m - i + 1) * k)
      return false;
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> k >> p;
  for (int i = 0; i < n; ++i)
    cin >> h[i] >> a[i];
  ll l = *max_element(a, a + n), r = 0;
  for (int i = 0; i < n; ++i)
    r = max(r, h[i] + m * (ll)a[i]);
  while (l < r) {
    ll mid = (l + r) >> 1;
    if (pred(mid))
      r = mid;
    else
      l = mid + 1;
  }
  cout << l << '\n';

  return 0;
}
