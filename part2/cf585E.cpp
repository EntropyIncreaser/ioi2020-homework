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

const int N = 20000010, M = 500010, P = 1000000007;

int n, pc;
int pr[1300000], st[M], x[N], y[N];
bool vis[N];

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int& x, int y) {
  if ((x -= y) < 0)
    x += P;
}

int norm(int x) { return x >= P ? x - P : x; }

void trans(int* a) {
  for (int i = 1; i <= pc; ++i)
    for (int j = n / pr[i]; j; --j)
      add(a[j], a[j * pr[i]]);
}

void itrans(int* a) {
  for (int i = 1; i <= pc; ++i)
    for (int j = 1; j <= n / pr[i]; ++j)
      sub(a[j], a[j * pr[i]]);
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m;
  cin >> m;
  for (int i = 1; i <= m; ++i)
    st[i] = norm(st[i - 1] * 2 + 1);
  while (m--) {
    int ai;
    cin >> ai;
    ++x[ai];
    n = max(n, ai);
  }
  for (int i = 2; i <= n; ++i) {
    if (!vis[i])
      pr[++pc] = i;
    for (int j = 1; i * pr[j] <= n; ++j) {
      vis[i * pr[j]] = true;
      if (i % pr[j] == 0)
        break;
    }
  }
  trans(x);
  for (int i = 1; i <= n; ++i)
    y[i] = st[x[i]];
  itrans(y);
  y[1] = 0;
  trans(y);
  for (int i = 1; i <= n; ++i)
    x[i] = x[i] * (ll)y[i] % P;
  itrans(x);
  cout << x[1] << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
