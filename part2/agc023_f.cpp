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

struct Node {
  int a, b, x;

  Node() {}

  Node(int a, int b, int x) : a(a), b(b), x(x) {}

  bool operator<(const Node& rhs) const {
    if (b * (ll)rhs.a == rhs.b * (ll)a) return x < rhs.x;
    else
      return b * (ll)rhs.a < rhs.b * (ll)a;
  }
};

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> p(n - 1), v(n);
  cin >> p >> v;
  p.insert(p.begin(), {0, 0});
  v.insert(v.begin(), {0});
  vector<int> f(n + 1);
  vector<Node> a(n + 1);
  for (int i = 1; i <= n; ++i)
    a[i] = Node(v[i] == 0, v[i] == 1, i);
  iota(f.begin(), f.end(), 0);
  function<int(int)> find = [&](int x) {
    return f[x] == x ? x : f[x] = find(f[x]);
  };
  set<Node> heap(a.begin() + 2, a.end());
  ll ans = 0;
  for (int rep = 1; rep < n; ++rep) {
    Node tmp = *heap.begin();
    heap.erase(heap.begin());
    int y = find(p[tmp.x]);
    if (y != 1) heap.erase(a[y]);
    ans += a[y].b * (ll)tmp.a;
    f[tmp.x] = y;
    a[y].a += tmp.a;
    a[y].b += tmp.b;
    if (y != 1) heap.insert(a[y]);
  }
  cout << ans;

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
