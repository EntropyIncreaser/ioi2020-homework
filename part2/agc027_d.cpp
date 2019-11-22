#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>
#include <cassert>

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

ostream& operator<<(ostream& os, const pair<char, int>& unit) {
  return os << unit.first << "^" << unit.second;
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

ll lcm(ll a, ll b) { return a / __gcd(a, b) * b; }

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n = 500;
  int val = n * n / 4;
  vector<vector<ll>> ans(n + 2, vector<ll>(n + 2, 1));
  int cnt = 0, l = 3, r = val * 2 + 1;
  set<ll> st;
  for (int i = 3; i <= n * 2; i += 2)
    for (int x = max(1, i - n), y = i - x; x + 1 <= n && y - 1 > 0; x += 2, y -= 2) {
      if (++cnt & 1) {
        ans[x][y] = l;
        ans[x + 1][y - 1] = l * 2;
        st.insert(l); st.insert(l * 2);
        l += 2;
      } else {
        ans[x][y] = r;
        ans[x + 1][y - 1] = r * 2;
        st.insert(r); st.insert(r * 2);
        r -= 2;
      } 
    }
  for (int i = 1; i <= n; ++i)
    for (int j = 1; j <= n; ++j) {
      if (ans[i][j] == 1) {
        ll l = lcm(lcm(ans[i - 1][j], ans[i + 1][j]), lcm(ans[i][j - 1], ans[i][j + 1]));
        ll v = 1;
        while (st.count(v * l + 1)) ++v;
        ans[i][j] += l * v;
        st.insert(ans[i][j]);
      }
    }
  cin >> n;
  for (int i = 1; i <= n; ++i)
    cout << vector<ll>(ans[i].begin() + 1, ans[i].begin() + n + 1) << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
