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
#include <tuple>
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

struct SegmentTree {
  struct Node {
    int l, r;
    Node *ls, *rs;
    ll v;
    int c;

    ll get() const { return l == r ? (c ? v : numeric_limits<ll>::min()) : v; }

    void update() {
      v = max(ls->get(), rs->get());
    }

    void change(int k, int x) {
      if (l == r) {
        c += x;
        return;
      }
      (k <= ls->r ? ls : rs)->change(k, x);
      update();
    }

    int find() const {
      if (l == r)
        return l;
      return (v == ls->get() ? ls : rs)->find();
    }
  };

  vector<Node> pool;
  Node* root;

  SegmentTree() {}

  void init(int n, vector<ll> value, const vector<int>& cnt) {
    pool.assign(n * 2 - 1, Node());
    int cn = 0;
    function<Node*(int, int)> build = [&](int l, int r) {
      Node* ret = pool.begin().base() + cn++;
      ret->l = l;
      ret->r = r;
      if (l == r) {
        ret->v = value[l];
        ret->c = cnt[l];
        return ret;
      }
      int mid = (l + r) >> 1;
      ret->ls = build(l, mid);
      ret->rs = build(mid + 1, r);
      ret->update();
      return ret;
    };
    root = build(0, n - 1);
  }

  ll get() const { return root->get(); }

  void change(int k, int x) { root->change(k, x); }

  int find() const { return root->find(); }
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
  vector<vector<ll>> r(4, vector<ll>(n)), b(r);
  vector<int> rc(n), bc(n);
  vector<SegmentTree> s(4), t(4);
  vector<vector<SegmentTree>> augs(4, vector<SegmentTree>(4)), augt(augs);

  for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y >> rc[i];
    for (int j = 0; j < 4; ++j)
      r[j][i] = ((j & 1) ? x : -x) + ((j & 2) ? y : -y);
  }
  for (int i = 0; i < n; ++i) {
    int x, y;
    cin >> x >> y >> bc[i];
    for (int j = 0; j < 4; ++j)
      b[j][i] = ((j & 1) ? -x : x) + ((j & 2) ? -y : y);
  }
  for (int j = 0; j < 4; ++j) {
    s[j].init(n, r[j], rc);
    t[j].init(n, b[j], bc);
  }
  for (int j = 0; j < 4; ++j)
    for (int k = 0; k < 4; ++k)
      if (j != k) {
        vector<ll> v(n);
        for (int i = 0; i < n; ++i)
          v[i] = -r[j][i] + r[k][i];
        augs[j][k].init(n, v, vector<int>(n));
        for (int i = 0; i < n; ++i)
          v[i] = +b[j][i] - b[k][i];
        augt[j][k].init(n, v, vector<int>(n));
      }
  int f = accumulate(rc.begin(), rc.end(), 0);
  ll ans = 0;
  while (f--) {
    vector<tuple<int, int, ll>> edges;
    vector<pair<ll, int>> dist(6, make_pair(-1LL << 60, -1));
    int S = 4, T = 5;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j)
        if (i != j) {
          ll w = max(augs[i][j].get(), augt[i][j].get());
          if (w > numeric_limits<ll>::min())
            edges.emplace_back(i, j, w);
        }
      edges.emplace_back(S, i, s[i].get());
      edges.emplace_back(i, T, t[i].get());
    }
    dist[S].first = 0;
    int rep = 5;
    while (rep--)
      for (const auto& pr : edges) {
        int u, v;
        ll w;
        tie(u, v, w) = pr;
        if (dist[v].first < dist[u].first + w)
          dist[v] = make_pair(dist[u].first + w, u);
      }
    ans += dist[T].first;
    vector<tuple<SegmentTree*, int, int>> augments;
    while (T != S) {
      int u = dist[T].second, v = T;
      if (u == 4) {
        int k = s[v].find();
        for (int i = 0; i < 4; ++i) {
          augments.emplace_back(&s[i], k, -1);
          if (i != v)
            augments.emplace_back(&augs[v][i], k, 1);
        }
      } else if (v == 5) {
        int k = t[u].find();
        for (int i = 0; i < 4; ++i) {
          augments.emplace_back(&t[i], k, -1);
          if (i != u)
            augments.emplace_back(&augt[i][u], k, 1);
        }
      } else {
        if (dist[v].first - dist[u].first == augs[u][v].get()) {
          int k = augs[u][v].find();
          for (int i = 0; i < 4; ++i) {
            if (i != u)
              augments.emplace_back(&augs[u][i], k, -1);
            if (i != v)
              augments.emplace_back(&augs[v][i], k, 1);
          }
        } else {
          int k = augt[u][v].find();
          for (int i = 0; i < 4; ++i) {
            if (i != u)
              augments.emplace_back(&augt[i][u], k, 1);
            if (i != v)
              augments.emplace_back(&augt[i][v], k, -1);
          }
        }
      }
      T = u;
    }
    for (const auto& pr : augments) {
      SegmentTree* p;
      int k, x;
      tie(p, k, x) = pr;
      p->change(k, x);
    }
  }
  cout << ans << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
