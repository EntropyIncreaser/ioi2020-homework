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

const int N = 100010, P = 1000000007;

int n, m, cnt;
int x[N], y[N], dx[N];
pair<int, int> a[N];
int fw[N];

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

int norm(int x) { return x >= P ? (x - P) : x; }

int lowBit(int k) { return k & -k; }

void ch(int k, int x) {
  for (; k <= cnt; k += lowBit(k))
    add(fw[k], x);
}

int qry(int k) {
  int ret = 0;
  while (k) {
    add(ret, fw[k]);
    k &= k - 1;
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

  cin >> n >> m;
  for (int i = 1; i <= n; ++i)
    cin >> x[i];
  for (int i = 1; i <= m; ++i)
    cin >> y[i];
  for (int i = 1; i <= n; ++i) {
    if (x[i] < y[1] || x[i] > y[m])
      continue;
    int id = lower_bound(y + 1, y + m + 1, x[i]) - y;
    a[++cnt] = make_pair(x[i] - y[id - 1], y[id] - x[i]);
    dx[cnt] = a[cnt].second;
  }
  int ans = 1;
  sort(dx + 1, dx + cnt + 1);
  sort(a + 1, a + cnt + 1, [](const auto& lhs, const auto& rhs) { return lhs.first == rhs.first ? (lhs.second > rhs.second) : (lhs.first < rhs.first); });
  for (int i = 1; i <= cnt; ++i) {
    if (a[i] == a[i - 1]) continue;
    int pos = lower_bound(dx + 1, dx + cnt + 1, a[i].second) - dx;
    int cur = norm(qry(pos - 1) + 1);
    add(ans, cur);
    ch(pos, cur);
  }
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
