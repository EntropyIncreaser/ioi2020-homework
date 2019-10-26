#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <unordered_map>
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

const int N = 250010, P = 998244353, P2 = 1000000007, B = 19260817, B2 = 1919810;

struct Int {
  int a, b;

  Int(int x = 0) : a(x), b(x) {}

  Int(int x, int y) : a(x), b(y) {}

  Int operator+(const Int& rhs) const { return Int((a + rhs.a) % P, (b + rhs.b) % P2); }

  Int operator*(const Int& rhs) const { return Int((a * (ll)rhs.a) % P, (b * (ll)rhs.b) % P2); }

  bool operator<(const Int &rhs) const {
    if (a < rhs.a)
      return true;
    if (rhs.a < a)
      return false;
    return b < rhs.b;
  }
};

typedef pair<Int, Int> pii;

struct NumberTheory {

  typedef pair<int, int> _P2_Field;

  mt19937 rng;

  NumberTheory() : rng(chrono::steady_clock::now().time_since_epoch().count()) {}

  void _exGcd(int a, int b, int& x, int& y) {
    if (!b) {
      x = 1;
      y = 0;
      return;
    }
    _exGcd(b, a % b, y, x);
    y -= a / b * x;
  }

  int inv(int a, int p = P) {
    int x = 0, y = 0;
    _exGcd(a, p, x, y);
    if (x < 0)
      x += p;
    return x;
  }

  template <class Integer>
  bool quadRes(Integer a, Integer b) {
    if (a <= 1)
      return true;
    while (a % 4 == 0)
      a /= 4;
    if (a % 2 == 0)
      return (b % 8 == 1 || b % 8 == 7) == quadRes(a / 2, b);
    return ((a - 1) % 4 == 0 || (b - 1) % 4 == 0) == quadRes(b % a, a);
  }

  // assume p in prime, x in quadratic residue
  int sqrt(int x, int p = P) {
    if (p == 2 || x <= 1)
      return x;
    int w, v, k = (p + 1) / 2;
    do {
      w = rng() % p;
    } while (quadRes(v = int((w * (ll)w - x + p) % p), p));
    _P2_Field res(1, 0), a(w, 1);
    while (k) {
      if (k & 1)
        res = _P2_Field((res.first * (ll)a.first + res.second * (ll)a.second % p * v) % p, (res.first * (ll)a.second + res.second * (ll)a.first) % p);
      if (k >>= 1)
        a = _P2_Field((a.first * (ll)a.first + a.second * (ll)a.second % p * v) % p, (a.first * (ll)a.second << 1) % p);
    }
    return min(res.first, p - res.first);
  }

} nt;

pii unite(const pii& lhs, const pii& rhs) { return make_pair(lhs.first + rhs.first * lhs.second, lhs.second * rhs.second); }

const pii LEFT(0, Int(B, B2)), RIGHT(0, Int(nt.inv(B), nt.inv(B2, P2))), PLUS(1, 1), MINUS(Int(P - 1, P2 - 1), 1);

char s[N];
pii op[256];
pii pref[N];

map<Int, int> mp;

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n >> (s + 1);
  op['<'] = LEFT;
  op['>'] = RIGHT;
  op['+'] = PLUS;
  op['-'] = MINUS;
  pref[0] = make_pair(0, 1);
  for (int i = 1; i <= n; ++i)
    pref[i] = unite(pref[i - 1], op[s[i]]);
  ll ans = 0;
  for (int i = 0; i <= n; ++i) {
    ans += mp[pref[i].first];
    ++mp[unite(pref[i], pref[n]).first];
  }
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
