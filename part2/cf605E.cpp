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

const int N = 1010;

int n;
double p[N][N];
bool vis[N];
double ans[N], rest[N], sum[N];

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) {
      int k;
      scanf("%d", &k);
      p[i][j] = k * 0.01;
    }
  fill(rest + 1, rest + n + 1, 1);
  fill(ans + 1, ans + n, 1e9);
  for (int rep = 1; rep < n; ++rep) {
    int u = -1;
    for (int i = 1; i <= n; ++i)
      if (!vis[i] && (u == -1 || ans[u] > ans[i]))
        u = i;
    vis[u] = true;
    for (int i = 1; i <= n; ++i)
      if (!vis[i]) {
        sum[i] += rest[i] * p[i][u] * ans[u];
        rest[i] *= (1 - p[i][u]);
        if (rest[i] != 1)
          ans[i] = (1 + sum[i]) / (1 - rest[i]);
      }
  }
  printf("%.15f\n", ans[1]);

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
