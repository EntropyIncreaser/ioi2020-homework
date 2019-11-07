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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<int> deg(n);
  vector<vector<int>> g(n);
  for (int rep = 0; rep < m; ++rep) {
    int u, v;
    cin >> u >> v;
    ++deg[u - 1]; ++deg[v - 1];
    g[u - 1].push_back(v - 1);
    g[v - 1].push_back(u - 1);
  }
  if (m <= n + 1) {
    puts("No");
    return 0;
  }
  for (int i = 0; i < n; ++i)
    if (deg[i] % 2) {
      puts("No");
      return 0;
    }
  if (m == n + 2 && count(deg.begin(), deg.end(), 4) == 2) {
    int u = find(deg.begin(), deg.end(), 4) - deg.begin();
    queue<pair<int, int>> q;
    for (const auto& v : g[u])
      q.emplace(u, v);
    while (!q.empty()) {
      int x = q.front().first, y = q.front().second;
      q.pop();
      if (deg[y] == 4 && y == u) {
        puts("Yes");
        return 0;
      }
      if (deg[y] == 4) continue;
      for (const auto& v : g[y])
        if (v != x)
          q.emplace(y, v);
    }
    puts("No");
  } else
    puts("Yes");

  return 0;
}
