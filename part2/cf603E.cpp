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

const int N = 100010, M = 300010;

struct Data {
  int v, id;

  Data() {}

  Data(int v, int id) : v(v), id(id) {}

  bool operator<(const Data& rhs) const { return v > rhs.v; }
};

struct LCT {
  struct Node {
    bool rev, par, subpar, solid;
    Data val, myn;
    union {
      struct {
        Node *ls, *rs;
      };
      Node* ch[2];
    };
    Node *prt, *path;

    bool relation() const { return this == prt->rs; }

    void pushDown() {
      if (rev) {
        rev = false;
        swap(ls, rs);
        if (ls)
          ls->rev = !ls->rev;
        if (rs)
          rs->rev = !rs->rev;
      }
    }

    void update() {
      myn = val;
      par = solid ^ subpar;
      if (ls) {
        myn = min(myn, ls->myn);
        par ^= ls->par;
      }
      if (rs) {
        myn = min(myn, rs->myn);
        par ^= rs->par;
      }
    }

    void rotate() {
      if (prt->prt)
        prt->prt->pushDown();
      prt->pushDown();
      pushDown();
      path = prt->path;
      prt->path = NULL;
      bool f = relation();
      Node* p = prt;
      if (p->prt)
        p->prt->ch[p->relation()] = this;
      prt = p->prt;
      p->ch[f] = ch[!f];
      if (ch[!f])
        ch[!f]->prt = p;
      ch[!f] = p;
      p->prt = this;
      p->update();
      update();
    }

    void splay() {
      while (prt) {
        if (!prt->prt)
          return rotate();
        prt->prt->pushDown();
        prt->pushDown();
        if (relation() == prt->relation()) {
          prt->rotate();
          rotate();
        } else {
          rotate();
          rotate();
        }
      }
    }

    void expose() {
      splay();
      pushDown();
      if (rs) {
        subpar ^= rs->par;
        rs->path = this;
        rs->prt = NULL;
        rs = NULL;
        update();
      }
    }

    bool splice() {
      splay();
      if (!path)
        return false;
      path->expose();
      path->subpar ^= par;
      path->rs = this;
      prt = path;
      path = NULL;
      prt->update();
      return true;
    }

    void access() {
      expose();
      while (splice())
        ;
    }

    void evert() {
      access();
      splay();
      rev = !rev;
    }

    Data query() {
      access();
      splay();
      return myn;
    }
  };

  Node ver[N + M];

  void link(int u, int v) {
    ver[u].evert();
    ver[v].evert();
    ver[v].subpar ^= ver[u].par;
    ver[v].update();
    ver[u].path = ver + v;
  }

  void cut(int u, int v) {
    ver[u].evert();
    ver[v].access();
    ver[u].expose();
    ver[u].subpar ^= ver[v].par;
    ver[u].update();
    ver[v].path = NULL;
  }

  Data query(int u, int v) {
    ver[u].evert();
    return ver[v].query();
  }

  bool qpar(int u) {
    ver[u].evert();
    return ver[u].par;
  }

  bool connected(int u, int v) {
    ver[u].evert();
    ver[v].access();
    ver[v].splay();
    Node* p = ver + u;
    while (p->prt) p = p->prt;
    if (p == ver + v) {
      ver[u].splay();
      return true;
    } else {
      ver[u].splay();
      return false;
    }
  }
};

int n, m;
int f[N];
bool szpar[N];
int a[M], b[M], l[M], ans[M];
LCT mst;

int find(int x) { return x == f[x] ? x : f[x] = find(f[x]); }

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 1; i <= m; ++i)
    cin >> a[i] >> b[i] >> l[i];
  for (int i = 1; i <= n; ++i) {
    mst.ver[i].solid = mst.ver[i].par = true;
  }
  for (int i = 1; i <= m; ++i)
    mst.ver[i + n].val = mst.ver[i + n].myn = Data(l[i], i);
  int tot = n;
  int ptr = 0;
  fill(szpar + 1, szpar + n + 1, true);
  memset(ans, -1, sizeof(ans));
  iota(f + 1, f + n + 1, 1);
  while (ptr <= m && tot) {
    ++ptr;
    int x = find(a[ptr]), y = find(b[ptr]);
    if (x != y) {
      tot -= szpar[x] + (int)szpar[y];
      szpar[x] ^= szpar[y];
      f[y] = x;
      tot += szpar[x];
    }
  }
  multiset<pair<int, int>, greater<pair<int, int>>> edges;
  function<void(int)> add = [&](int id) {
    if (mst.connected(a[id], b[id])) {
      Data gt = mst.query(a[id], b[id]);
      if (gt.v < l[id])
        return;
      mst.cut(a[gt.id], n + gt.id);
      mst.cut(b[gt.id], n + gt.id);
      edges.erase(edges.find(make_pair(gt.v, gt.id)));
    }
    mst.link(a[id], n + id);
    mst.link(b[id], n + id);
    edges.insert(make_pair(l[id], id));
  };
  for (int i = 1; i < ptr; ++i)
    add(i);
  while (ptr <= m) {
    add(ptr);
    while (true) {
      int id = edges.begin()->second;
      mst.cut(a[id], id + n);
      mst.cut(b[id], id + n);
      if (!mst.qpar(a[id]) && !mst.qpar(b[id])) {
        edges.erase(edges.begin());
      } else {
        mst.link(a[id], id + n);
        mst.link(b[id], id + n);
        break;
      }
    }
    ans[ptr] = edges.begin()->first;
    ++ptr;
  }

  for (int i = 1; i <= m; ++i)
    cout << ans[i] << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
