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

int P;

struct Mat {
  int a[2][2];
  Mat() { memset(a, 0, sizeof(a)); }
  Mat(const Mat& m) { memcpy(a, m.a, sizeof(a)); }
  int* operator[](int k) { return a[k]; }
  const int* operator[](int k) const { return a[k]; }
  Mat operator*(const Mat& rhs) const {
    Mat ret = Mat();
    for (int i = 0; i < 2; ++i)
      for (int j = 0; j < 2; ++j)
        for (int k = 0; k < 2; ++k)
          ret[i][k] = (ret[i][k] + a[i][j] * (ll)rhs[j][k]) % P;
    return ret;
  }
};

struct Node {
  Node *ls, *rs;
  Mat v;
  int l, r;
  Mat qry(int l, int r) const {
    if (this->l == l && this->r == r) return v;
    if (r <= ls->r) return ls->qry(l, r);
    if (l >= rs->l) return rs->qry(l, r);
    return ls->qry(l, ls->r) * rs->qry(rs->l, r);
  }
};

const int N = 50010;

int n, m;
int s[N];
Mat tr[N];
Node* segTree;

Mat trans(int a, int b) {
  Mat ret = Mat();
  ret[1][0] = 1;
  ret[0][1] = a; ret[1][1] = b;
  return ret;
}

Node* build(int l, int r) {
  static Node pool[N * 2], *top = pool;
  Node* p = top++;
  p->l = l; p->r = r;
  if (p->l == p->r) {
    p->v = tr[l];
    return p;
  }
  int mid = ((r - l) >> 1) + l;
  p->ls = build(l, mid);
  p->rs = build(mid + 1, r);
  p->v = p->ls->v * p->rs->v;
  return p;
}

Mat mpow(Mat x, ll k) {
  Mat ret = Mat();
  ret[0][0] = ret[1][1] = 1;
  while (k) {
    if (k & 1) ret = ret * x;
    if (k >>= 1) x = x * x;
  }
  return ret;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  ll k;
  cin >> k >> P >> n;
  for (int i = 0; i < n; ++i) {
    cin >> s[i];
    s[i] %= P;
  }
  for (int i = 0; i < n; ++i)
    tr[i] = trans(s[(i + n - 2) % n], s[(i + n - 1) % n]);
  cin >> m;
  vector<ll> crit;
  map<ll, int> ch;
  while (m--) {
    ll j; int v;
    cin >> j >> v;
    v %= P;
    ch[j] = v;
    crit.push_back(j);
    crit.push_back(j + 1);
    crit.push_back(j + 2);
  }
  crit.push_back(k);
  sort(crit.begin(), crit.end());
  while (crit.back() > k) crit.pop_back();
  Mat cur = Mat();
  cur[0][0] = 1; cur[1][1] = 1;
  if (k == 0) {
    puts("0");
    return 0;
  }
  ll pos = 1;
  segTree = build(0, n - 1);
  for (ll nxt : crit) {
    if (nxt <= pos) continue;
    if (nxt - pos == 1) {
      int a = ch.count(nxt - 2) ? ch[nxt - 2] : s[(nxt + n - 2) % n],
          b = ch.count(nxt - 1) ? ch[nxt - 1] : s[(nxt + n - 1) % n];
      cur = cur * trans(a, b);
    } else {
      if ((pos + 1) / n == nxt / n)
        cur = cur * segTree->qry((pos + 1) % n, nxt % n);
      else {
        ll len = nxt - pos;
        if (pos % n != n - 1) {
          cur = cur * segTree->qry((pos + 1) % n, n - 1);
          len -= n - ((pos + 1) % n);
        }
        cur = cur * mpow(segTree->qry(0, n - 1), (len - nxt % n - 1) / n);
        cur = cur * segTree->qry(0, nxt % n);
      }
    }
    pos = nxt;
  }

  int ans = cur[1][1] % P;
  cout << ans << '\n';

  return 0;
}
