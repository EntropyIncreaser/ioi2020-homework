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

typedef vector<int> vi;

int norm(int x, int m) {
  x %= m;
  if (x < 0) x += m;
  return x;
}

vi power(const vi& p, int k) {
  int n = p.size();
  vector<bool> vis(n);
  vi ret(n);
  for (int i = 0; i < n; ++i)
    if (!vis[i]) {
      vector<int> cycle;
      int x = i;
      while (!vis[x]) {
        cycle.push_back(x);
        vis[x] = true;
        x = p[x];
      }
      for (int i = 0; i < cycle.size(); ++i)
        ret[cycle[i]] = cycle[norm(i + k, cycle.size())];
    }
  return ret;
}

vi inverse(const vi& p) { return power(p, -1); }

vi mult(const vi& p, const vi& q) {
  int n = p.size();
  vi ret(n);
  for (int i = 0; i < n; ++i)
    ret[i] = p[q[i]];
  return ret;
}

template <class T, class... Args>
vi mult(const T& first, const Args&... args...) {
  return mult(first, mult(args...));
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n, k;
  cin >> n >> k;
  --k;
  vi p(n), q(n);
  cin >> p >> q;
  for (int& v : p) --v;
  for (int& v : q) --v;
  vi ip = inverse(p), iq = inverse(q), w = mult(q, ip, iq, p);
  int m = k / 6;
  static vi little[6];
  little[0] = mult(power(w, m), q, p, iq, power(w, 1 - m));
  little[1] = mult(power(w, m), q, power(w, -m));
  int r = k % 6;
  for (int i = 2; i <= r; ++i)
    little[i] = mult(little[i - 1], inverse(little[i - 2]));
  vi ans = little[r];
  for (int& v : ans) ++v;
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
