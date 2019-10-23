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
typedef unsigned int ui;

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

void exGcd(ll a, ll b, ll& x, ll& y) {
  if (!b) {
    x = 1;
    y = 0;
    return;
  }
  exGcd(b, a % b, y, x);
  y -= a / b * x;
}

ui inv(ui a) {
  ll x, y;
  exGcd(a, 1LL << 32, x, y);
  if (x < 0) x += 1LL << 32;
  return x;
}

ui coe[155];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, p, q;
  cin >> n >> p >> q;
  ui ans = 0, cur = 1;
  int dig = 0;
  p = min(p, n - 1);
  coe[0] = 1;
  for (int i = 1; i <= p; ++i) {
    int v = n - i + 1;
    while (!(v & 1)) {
      ++dig;
      v >>= 1;
    }
    cur *= ui(v);
    v = i;
    while (!(v & 1)) {
      --dig;
      v >>= 1;
    }
    cur *= inv(v);
    if (dig < 32)
      coe[i] = cur << dig;
  }
  for (int i = 1; i <= q; ++i) {
    ui r = 0;
    for (int j = p; j >= 0; --j)
      r = r * ui(i) + coe[j];
    ans ^= r * ui(i);
  }
  cout << ans << '\n';

  return 0;
}
