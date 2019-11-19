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
typedef vector<int> vi;

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

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, d1, d2;
  cin >> n >> d1 >> d2;
  function<vector<vi>(int)> calc = [&](int d) {
    vector<vi> ret(n * 2, vi(n * 2, -1));
    vector<pair<int, int>> v;
    for (int i = 0; i * i <= d; ++i) {
      int j = sqrt(d - i * i);
      if (i * i + j * j != d) continue;
      v.emplace_back(i, j);
      v.emplace_back(-i, j);
      v.emplace_back(i, -j);
      v.emplace_back(-i, -j);
    }
    function<void(int, int)> dfs = [&](int x, int y) {
      for (const auto& pr : v) {
        int xi = x + pr.first, yi = y + pr.second;
        if (xi >= 0 && xi < n * 2 && yi >= 0 && yi < n * 2 && ret[xi][yi] == -1) {
          ret[xi][yi] = !ret[x][y];
          dfs(xi, yi);
        }
      }
    };
    for (int i = 0; i < n * 2; ++i)
      for (int j = 0; j < n * 2; ++j)
        if (ret[i][j] == -1) {
          ret[i][j] = 0;
          dfs(i, j);
        }
    return ret;
  };
  vector<vi> r1 = calc(d1), r2 = calc(d2);
  vector<vi> res(n * 2, vi(n * 2));
  vi cn(4);
  for (int i = 0; i < n * 2; ++i)
    for (int j = 0; j < n * 2; ++j)
      ++cn[res[i][j] = r1[i][j] | (r2[i][j] << 1)];
  int v = max_element(cn.begin(), cn.end()) - cn.begin();
  int cnt = 0;
  for (int i = 0; i < n * 2; ++i) {
    for (int j = 0; j < n * 2; ++j)
      if (res[i][j] == v) {
        cout << i << ' ' << j << '\n';
        if (++cnt == n * n) break;
      }
    if (cnt == n * n) break;
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
