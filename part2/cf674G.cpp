#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <memory>
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

struct IO_Tp {
  const static int _I_Buffer_Size = 10 << 20;
  char _I_Buffer[_I_Buffer_Size], *_I_pos = _I_Buffer;

  const static int _O_Buffer_Size = 10 << 20;
  char _O_Buffer[_O_Buffer_Size], *_O_pos = _O_Buffer;

  IO_Tp() {
#ifdef LBT
    freopen("test.in", "r", stdin);
#endif
    fread(_I_Buffer, 1, _I_Buffer_Size, stdin);
  }

  ~IO_Tp() {
    fwrite(_O_Buffer, 1, _O_pos - _O_Buffer, stdout);
  }

  IO_Tp &operator>>(int &res) {
    while (!isdigit(*_I_pos)) ++_I_pos;
    res = *_I_pos++ & 15;
    while (isdigit(*_I_pos)) res = res * 10 + (*_I_pos++ & 15);
    return *this;
  }

  IO_Tp &operator<<(int n) {
    static char _buf[20];
    char *_pos = _buf;
    do
      *_pos++ = '0' + n % 10;
    while (n /= 10);
    while (_pos != _buf) *_O_pos++ = *--_pos;
    return *this;
  }

  IO_Tp &operator<<(char ch) {
    *_O_pos++ = ch;
    return *this;
  }
} IO;

const int N = 150010;

template <class K, class V>
struct ZipTree {

  mt19937 rng;

  struct Node {
    int rank;
    K key;
    V value, sub;
    Node *ls, *rs;

    Node() : rank(), key(), value(), sub(), ls(), rs() {}

    Node(int rank, K key, V value = 0) : rank(rank), key(key), value(value), sub(value), ls(), rs() {}

    void update() {
      sub = value;
      if (ls)
        sub = ls->sub + sub;
      if (rs)
        sub = sub + rs->sub;
    }

    void* operator new(size_t) {
      static Node pool[N * 6], *top = pool;
      return top++;
    }
  };

  Node* root;

  ZipTree() : rng(chrono::steady_clock::now().time_since_epoch().count()), root() {}

  Node* zip(Node* p, Node* q) {
    if (!p) return q;
    if (!q) return p;
    if (p->rank > q->rank) {
      p->rs = zip(p->rs, q);
      p->update();
      return p;
    }
    q->ls = zip(p, q->ls);
    q->update();
    return q;
  }

  Node* change(Node* p, const K& key, const V& value) {
    if (!p) return new Node(rng(), key, value);
    if (p->key == key) {
      p->value = p->value + value;
      if (p->value <= 0)
        return zip(p->ls, p->rs);
      p->update();
      return p;
    }
    if (key < p->key) {
      Node* ptr = nullptr;
      swap(ptr, p->ls);
      p->update();
      return zip(change(ptr, key, value), p);
    }
    Node* ptr = nullptr;
    swap(ptr, p->rs);
    p->update();
    return zip(p, change(ptr, key, value));
  }

  void debug(Node* p) {
    if (!p) return;
    if (p->ls) debug(p->ls);
    cerr << p->key << ',' << p->value << ' ';
    if (p->rs) debug(p->rs);
  }

  void change(const K& key, const V& value) {
    root = change(root, key, value);
  }

  V query(const K& key) const {
    V ret(0);
    Node* p = root;
    while (p) {
      if (p->key <= key) {
        if (p->ls)
          ret = ret + p->ls->sub;
        ret = ret + p->value;
        if (key == p->key)
          return ret;
        p = p->rs;
      } else
        p = p->ls;
    }
    return ret;
  }

  pair<K, V> succ(const K& key) const {
    Node* p = root;
    pair<K, V> ret(1000000, -1);
    while (p) {
      if (p->key < key)
        p = p->rs;
      else {
        ret = make_pair(p->key, p->value);
        p = p->ls;
      }
    }
    return ret;
  }
};

struct Node {
  int l, r, tag, cnt;
  Node *ls, *rs;
  int res[5];
};

int n, m, p;
int a[N];
map<int, int> segment;
Node* segTree;

ZipTree<int, int> zipTree[N];

int qry(int c, int k) {
  int ret = zipTree[c].query(k);
  auto pr = zipTree[c].succ(k + 1);
  int l = pr.first - pr.second + 1;
  if (l <= k)
    ret += k - l + 1;
  return ret;
}

bool check(int c, int l, int r) {
  //cerr << c << ' ' << l << ' ' << r << ": " << qry(c, r) << " - " << qry(c, l - 1) << '\n';
  return (qry(c, r) - qry(c, l - 1)) * 100 >= (r - l + 1) * p;
}

vector<int> unite(const vector<int>& lhs, const vector<int>& rhs, int l, int r) {
  static int tmp[5];
  int cnt = 0;
  for (int v : lhs) {
    if (find(rhs.begin(), rhs.end(), v) != rhs.end() || check(v, l, r))
      tmp[cnt++] = v;
  }
  for (int v : rhs)
    if (find(tmp, tmp + cnt, v) == tmp + cnt && check(v, l, r))
      tmp[cnt++] = v;
  return vector<int>(tmp, tmp + cnt);
}

vector<int> get(Node* p) { return vector<int>(p->res, p->res + p->cnt); }

vector<int> query(Node* p, int l, int r) {
  if (p->tag)
    return vector<int>(1, p->tag);
  if (l == p->l && r == p->r)
    return get(p);
  if (r <= p->ls->r)
    return query(p->ls, l, r);
  if (l >= p->rs->l)
    return query(p->rs, l, r);
  return unite(query(p->ls, l, p->ls->r), query(p->rs, p->rs->l, r), l, r);
}

void update(Node* p) {
  auto tmp = unite(get(p->ls), get(p->rs), p->l, p->r);
  copy(tmp.begin(), tmp.end(), p->res);
  p->cnt = tmp.size();
}

void pushDown(Node* p) {
  if (p->tag) {
    p->ls->tag = p->rs->tag = p->tag;
    p->ls->res[0] = p->rs->res[0] = p->tag;
    p->ls->cnt = p->rs->cnt = 1;
    p->tag = 0;
  }
}

Node* build(int l, int r) {
  static Node pool[N * 2], *top = pool;
  Node* p = top++;
  p->l = l;
  p->r = r;
  if (l == r) {
    p->res[0] = a[l];
    p->cnt = 1;
    return p;
  }
  int mid = (l + r) >> 1;
  p->ls = build(l, mid);
  p->rs = build(mid + 1, r);
  update(p);
  return p;
}

void change(Node* p, int l, int r, int c) {
  if (p->l == l && p->r == r) {
    p->tag = c;
    p->res[0] = c;
    p->cnt = 1;
    return;
  }
  pushDown(p);
  if (r <= p->ls->r)
    change(p->ls, l, r, c);
  else if (l >= p->rs->l)
    change(p->rs, l, r, c);
  else {
    change(p->ls, l, p->ls->r, c);
    change(p->rs, p->rs->l, r, c);
  }
  update(p);
}

int main() {
#ifdef LBT
  int nol_cl = clock();
#endif

  IO >> n >> m >> p;
  for (int i = 1; i <= n; ++i)
    IO >> a[i];
  for (int i = 1; i <= n; ++i) {
    segment.insert(make_pair(i, a[i]));
    zipTree[a[i]].change(i, 1);
  }
  segTree = build(1, n);
  segment.insert(make_pair(0, -1));
  while (m--) {
    int opt, l, r, id;
    IO >> opt >> l >> r;
    if (opt == 1) {
      IO >> id;
      function<void(int)> split = [&](int k) {
        auto segit = segment.lower_bound(k);
        if (segit->first != k) {
//          cerr << "split " << k << '\n';
          int c = segit->second;
          int prv = prev(segit)->first + 1;
          segment.insert(make_pair(k, c));
          auto pr = zipTree[c].succ(k);
          int len = k - (pr.first - pr.second);
          zipTree[c].change(k, len);
          zipTree[c].change(segit->first, -len);
        }
      };
      if (l != 1) split(l - 1);
      split(r);
      auto it = segment.find(r);
      while (it->first >= l) {
        zipTree[it->second].change(it->first, -N);
  //      cerr << "ers " << sit->second << " => " << it->first << '\n';
        segment.erase(it--);
      }
      segment.insert(make_pair(r, id));
      zipTree[id].change(r, r - l + 1);
      change(segTree, l, r, id);
    } else {
      vector<int> ans = query(segTree, l, r);
      IO << (int)ans.size();
      for (int v : ans)
        IO << ' ' << v;
      IO << '\n';
    }
    /*for (const auto& pr : seg[5])
      cerr << "[" << pr.second << ',' << pr.first << "] ";
    cerr << '\n';
    zipTree[5].debug(zipTree[5].root); cerr << ") tree " << seg[5].size() << '\n';*/
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
