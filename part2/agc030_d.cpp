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

const int N = 3010, P = 1000000007;

int a[N];
int p[N][N][2];

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, q;
  cin >> n >> q;
  for (int i = 1; i <= n; ++i)
    cin >> a[i];
  for (int i = 1; i < n; ++i)
    for (int j = i + 1; j <= n; ++j)
      if (a[i] > a[j])
        p[i][j][1] = 1;
      else if (a[i] < a[j])
        p[i][j][0] = 1;
  int pr = (P + 1) >> 1;
  int rep = q;
  while (rep--) {
    int x, y;
    cin >> x >> y;
    if (x > y) swap(x, y);
    for (int i = 1; i <= n; ++i)
      if (i < x) {
        for (int j = 0; j < 2; ++j) {
          int a = (p[i][x][j] + p[i][y][j]) * (ll)pr % P;
          p[i][x][j] = p[i][y][j] = a;
        }
      } else if (x < i && i < y) {
        for (int j = 0; j < 2; ++j) {
          int a = (p[x][i][j] + p[i][y][!j]) * (ll)pr % P;
          p[x][i][j] = p[i][y][!j] = a;
        }
      } else if (y < i) {
        for (int j = 0; j < 2; ++j) {
          int a = (p[x][i][j] + p[y][i][j]) * (ll)pr % P;
          p[x][i][j] = p[y][i][j] = a;
        }
      }
    int a = (p[x][y][0] + p[x][y][1]) * (ll)pr % P;
    p[x][y][0] = p[x][y][1] = a;
  }
  int ans = 0;
  for (int i = 1; i < n; ++i)
    for (int j = i + 1; j <= n; ++j)
      ans = (ans + p[i][j][1]) % P;
  while (q--)
    ans = ans * 2 % P;
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
