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

const int P = 1000000007;

int norm(int x) { return x >= P ? x - P : x; }

struct Node {
  int l, r, v, tag;
  Node *ls, *rs;

  int get() const { return v * (ll)tag % P; }

  void pd() {
    if (tag != 1) {
      v = v * (ll)tag % P;
      ls->tag = ls->tag * (ll)tag % P;
      rs->tag = rs->tag * (ll)tag % P;
      tag = 1;
    }
  }

  void upd() {
    v = norm(ls->get() + rs->get());
  }

  int qry(int k) {
    if (r == k) return get();
    pd();
    if (k <= ls->r) return ls->qry(k);
    return norm(ls->get() + rs->qry(k));
  }

  void ch(int k, int x) {
    if (l == r) {
      tag = 1;
      v = x;
      return;
    }
    pd();
    (k <= ls->r ? ls : rs)->ch(k, x);
    upd();
  }
};

const int N = 200010, INV2 = (P + 1) >> 1;

Node* build(int l, int r) {
  static Node pool[N * 4];
  static Node* top = pool;
  Node* p = top++;
  p->l = l;
  p->r = r;
  p->tag = 1;
  if (l == r) return p;
  int mid = (l + r) >> 1;
  p->ls = build(l, mid);
  p->rs = build(mid + 1, r);
  return p;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> a(n);
  cin >> a;
  vector<pair<int, int>> sorted(n);
  for (int i = 0; i < n; ++i)
    sorted[i] = make_pair(a[i], i);
  sort(sorted.begin(), sorted.end());
  int tot = 1;
  for (int i = 0; i < n; ++i)
    tot = tot * (ll)(sorted[i].first - i) % P;
  if (tot == 0) {
    cout << 0;
  } else {
    int ans = 0;
    vector<int> pos(n);
    for (int i = 0; i < n; ++i)
      pos[sorted[i].second] = i;
    vector<int> inv(n + 1);
    inv[1] = 1;
    for (int i = 2; i <= n; ++i)
      inv[i] = -(P / i) * (ll)inv[P % i] % P + P;
    Node* segTree = build(0, n - 1), *segTree2 = build(0, n - 1);
    for (int i = n - 1; i >= 0; --i) {
      int left = segTree->qry(sorted[i].second), right = norm(segTree->get() + P - left);
      int lc = segTree2->qry(sorted[i].second);
      segTree->tag = segTree->tag * (sorted[i].first - i - 1LL) % P * inv[sorted[i].first - i] % P;
      segTree->ch(sorted[i].second, inv[sorted[i].first - i]);
      segTree2->ch(sorted[i].second, 1);
      ans = (ans + right * (ll)tot % P * (sorted[i].first - i - 1) % P * INV2) % P;
      ans = (ans + (P - left) * (ll)tot % P * (sorted[i].first - i - 1) % P * INV2) % P;
      ans = (ans + lc * (ll)tot) % P;
    }
    cout << ans;
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
