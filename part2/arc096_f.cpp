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

const int N = 51;

const int C = N * N * N, INF = (1 << 30) - 10;

struct Knap {
  int V;
  int dp[C];
  
  void init(int V) {
    this->V = V;
    fill(dp + 1, dp + V + 1, INF);
  }
  
  void addsingle(int w, int v) {
    for (int i = V; i >= w; --i)
      dp[i] = min(dp[i - w] + v, dp[i]);
  }
  
  void add(int w, int v, int c) {
    int k = 1;
    while (k <= c) {
      addsingle(w * k, min(v * (ll)k, (ll)INF));
      c -= k;
      k <<= 1;
    }
    if (c > 0)
      addsingle(w * c, min(v * (ll)c, (ll)INF));
  }
} knap;

struct Ele {
  int m, s, w;
  
  Ele() {}
  
  Ele(int m, int s, int w) : m(m), s(s), w(w) {}
  
  bool operator>(const Ele& rhs) const { return s * (ll)rhs.m > rhs.s * (ll)m; }
};

int n, x, d;
int m[N], p[N], s[N];
int ec;
Ele e[N];

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> x >> d >> m[1];
  for (int i = 2; i <= n; ++i)
    cin >> m[i] >> p[i];
  fill(s + 1, s + n + 1, 1);
  for (int i = n; i > 1; --i) {
    s[p[i]] += s[i];
    m[p[i]] = min(m[i] + m[p[i]], x + 1);
  }
  knap.init(n * n * n);
  for (int i = 1; i <= n; ++i) {
    int lmt = i == 1 ? INF : d;
    knap.add(s[i], m[i], min(n, lmt));
    if (lmt > n)
      e[++ec] = Ele(m[i], s[i], lmt - n);
  }
  sort(e + 1, e + ec + 1, greater<Ele>());
  int ans = 0;
  for (int i = 0; i <= n * n * n; ++i) {
    if (knap.dp[i] > x) continue;
    int rest = x - knap.dp[i], cur = i;
    for (int j = 1; j <= ec; ++j) {
      int got = min(e[j].w, rest / e[j].m);
      rest -= got * e[j].m;
      cur += e[j].s * got;
    }
    ans = max(ans, cur);
  }
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
