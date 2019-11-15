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

#define px first
#define py second

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n;
  cin >> n;
  vector<pair<int, int>> p(n);
  for (int i = 0; i < n; ++i)
    cin >> p[i].px >> p[i].py;
  bool par = (p[0].px ^ p[0].py) & 1;
  for (int i = 1; i < n; ++i)
    if (par != ((p[i].px ^ p[i].py) & 1)) {
      puts("-1");
      return 0;
    }
  // R U L D
  int dim = 33;
  ll s = (1LL << dim) - 1 + par;
  printf("%d\n", dim + !par);
  if (!par)
    puts("1");
  for (int i = 0; i < dim; ++i)
    printf("%lld\n", 1LL << i);
  for (const auto& pr : p) {
    if (!par) putchar('R');
    ll vx = pr.px + pr.py + s, vy = pr.px - pr.py + s;
    for (int i = 1; i <= dim; ++i)
      putchar("LDUR"[((vx >> (i - 1)) & 2) | ((vy >> i) & 1)]);
    putchar('\n');
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
