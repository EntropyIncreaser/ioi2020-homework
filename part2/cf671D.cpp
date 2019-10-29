#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
 
#include <algorithm>
#include <tuple>
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
  int l, r;
  ll v, lazy;
  Node *ls, *rs;
};

const int N = 300010, L = 20;
const ll INF = 1LL << 60;

int n;
bool vis[N];
int dep[N];
vector<int> g[N];
vector<pair<int, int>> path[N];

Node* stk[N * L];
Node** stop = stk;

Node* create(int l, int r, ll v) {
  static Node pool[N * L], *top = pool;
  Node* p = (stop == stk) ? top++ : *(stop--);
  p->l = l;
  p->r = r;
  p->v = v;
  p->lazy = 0;
  p->ls = NULL;
  p->rs = NULL;
  return p;
}

void release(Node* o) {
  if (!o) return;
  *(++stop) = o;
  release(o->ls);
  release(o->rs);
}

void pushDown(Node* o) {
  if (o->lazy) {
    o->v += o->lazy;
    if (o->ls) o->ls->lazy += o->lazy;
    if (o->rs) o->rs->lazy += o->lazy;
    o->lazy = 0;
  }
}

void ins(Node* o, int k, ll x) {
  while (o->l != o->r) {
    pushDown(o);
    o->v = min(o->v, x);
    int mid = (o->l + o->r) >> 1;
    if (k <= mid) {
      if (!o->ls)
        o->ls = create(o->l, mid, x);
      o = o->ls;
    } else {
      if (!o->rs)
        o->rs = create(mid + 1, o->r, x);
      o = o->rs;
    }
  }
  pushDown(o);
  o->v = min(o->v, x);
}

ll get(Node* o) { return o->v + o->lazy; }

ll query(Node* o, int k) {
  ll ret = INF;
  while (o) {
    if (k == o->r) {
      ret = min(ret, get(o));
      break;
    }
    pushDown(o);
    int mid = (o->l + o->r) >> 1;
    if (k <= mid)
      o = o->ls;
    else {
      if (o->ls)
        ret = min(ret, get(o->ls));
      o = o->rs;
    }
  }
  return ret;
}

void update(Node* p) {
  p->v = INF;
  if (p->ls) p->v = min(p->v, get(p->ls));
  if (p->rs) p->v = min(p->v, get(p->rs));
}

Node* merge(Node* p, Node* q, ll& x, ll& y) {
  if (!p) {
    if (!q) return NULL;
    y = min(y, get(q));
    if (x >= INF) {
      release(q);
      return NULL;
    }
    q->lazy += x;
    return q;
  }
  if (!q) {
    x = min(x, get(p));
    if (y >= INF) {
      release(p);
      return NULL;
    }
    p->lazy += y;
    return p;
  }
  pushDown(p); pushDown(q);
  if (p->l == p->r) {
    ll xx = min(x, p->v), yy = min(y, q->v);
    p->v = min(xx + q->v, yy + p->v);
    x = xx;
    y = yy;
    return p;
  }
  p->rs = merge(p->rs, q->rs, x, y);
  p->ls = merge(p->ls, q->ls, x, y);
  update(p);
  if (p->v >= INF) {
    release(p);
    return NULL;
  }
  return p;
}

Node* trail(Node* o, int k) {
  if (!o || o->l > k) {
    release(o);
    return NULL;
  }
  if (k >= o->r) return o;
  pushDown(o);
  int mid = (o->l + o->r) >> 1;
  if (k <= mid) {
    o->rs = NULL;
    o->ls = trail(o->ls, k);
  } else
    o->rs = trail(o->rs, k);
  update(o);
  if (o->v >= INF) {
    release(o);
    return NULL;
  }
  return o;
}

Node* dfs(int u) {
  vis[u] = true;
  Node* ret = NULL;
  ret = create(0, n, 0);
  ins(ret, dep[u], 0);
  for (int v : g[u])
    if (!vis[v]) {
      dep[v] = dep[u] + 1;
      ll x = INF, y = INF;
      Node* p = trail(dfs(v), dep[u]);
      ret = merge(ret, p, x, y);
    }
  ll v = query(ret, dep[u]);
  if (v < INF)
    for (const auto& pr : path[u])
      ins(ret, dep[pr.first], v + pr.second);
  return ret;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m;
  cin >> n >> m;
  for (int rep = 1; rep < n; ++rep) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  while (m--) {
    int s, t, c;
    cin >> s >> t >> c;
    path[s].emplace_back(t, c);
  }
  ll ans = query(dfs(1), 0);
  if (ans >= INF)
    cout << "-1\n";
  else
    cout << ans << '\n';

  return 0;
}
