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

const int N = 101010, B = 323;

int n, m, tot, b, vc;
int vertex[N], len[N];
int trie[N][26], fail[N], prt[N], dfn[N], dfnr[N];
vector<pair<int, int>> qry1[N], qry2[N];
char s[N];
ll ans[N];
vector<int> ch[N];

int inb[N], blk[B];

int sub[N];
ll pre[N];

void dfs(int u) {
  static int t;
  dfn[u] = ++t;
  for (int v : ch[u])
    dfs(v);
  dfnr[u] = t;
}

void dfs2(int u) {
  for (int v : ch[u]) {
    dfs2(v);
    sub[u] += sub[v];
  }
}

void change(int k, int x) {
  for (; k % b != 0; ++k)
    inb[k] += x;
  k /= b;
  for (; k <= b; ++k)
    blk[k] += x;
}

int query(int k) {
  return inb[k] + blk[k / b];
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  int rt = ++vc;
  for (int i = 1; i <= n; ++i) {
    cin >> (s + 1);
    len[i] = strlen(s + 1);
    tot += len[i];
    int v = rt;
    for (int j = 1; j <= len[i]; ++j) {
      if (!trie[v][s[j] - 'a']) {
        trie[v][s[j] - 'a'] = ++vc;
        prt[vc] = v;
      }
      v = trie[v][s[j] - 'a'];
    }
    vertex[i] = v;
    memset(s + 1, 0, len[i]);
  }
  b = sqrt(tot) + 1;
  for (int i = 1; i <= m; ++i) {
    int l, r, k;
    cin >> l >> r >> k;
    if (len[k] >= b) {
      qry2[k].emplace_back(l - 1, -i);
      qry2[k].emplace_back(r, i);
    } else {
      qry1[l - 1].emplace_back(k, -i);
      qry1[r].emplace_back(k, i);
    }
  }
  queue<int> q;
  for (int i = 0; i < 26; ++i)
    if (trie[1][i]) {
      q.push(trie[1][i]);
      fail[trie[1][i]] = 1;
    }
  while (!q.empty()) {
    int u = q.front(); q.pop();
    for (int i = 0; i < 26; ++i)
      if (trie[u][i]) {
        int p = fail[u];
        while (p != rt && !trie[p][i])
          p = fail[p];
        if (trie[p][i])
          fail[trie[u][i]] = trie[p][i];
        else
          fail[trie[u][i]] = rt;
        q.push(trie[u][i]);
      }
  }
  for (int i = 2; i <= vc; ++i)
    ch[fail[i]].push_back(i);
  dfs(1);
  for (int i = 1; i <= n; ++i) {
    change(dfn[vertex[i]], 1);
    change(dfnr[vertex[i]] + 1, -1);
    for (const auto& pr : qry1[i]) {
      int u = vertex[pr.first];
      if (pr.second < 0)
        for (; u != rt; u = prt[u])
          ans[-pr.second] -= query(dfn[u]);
      else
        for (; u != rt; u = prt[u])
          ans[pr.second] += query(dfn[u]);
    }
  }
  for (int i = 1; i <= n; ++i)
    if (!qry2[i].empty()) {
      memset(sub, 0, sizeof(sub));
      int u = vertex[i];
      for (; u != rt; u = prt[u])
        sub[u] = 1;
      dfs2(1);
      for (int j = 1; j <= n; ++j)
        pre[j] = pre[j - 1] + sub[vertex[j]];
      for (const auto& pr : qry2[i])
        if (pr.second < 0)
          ans[-pr.second] -= pre[pr.first];
        else
          ans[pr.second] += pre[pr.first];
    }
  for (int i = 1; i <= m; ++i)
    cout << ans[i] << '\n';

  return 0;
}
