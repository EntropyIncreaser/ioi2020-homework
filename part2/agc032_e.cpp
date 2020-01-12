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
  
  int n, m;
  cin >> n >> m; 
  vector<int> a(n * 2);
  cin >> a;
  sort(a.begin(), a.end());
  int l = 0, r = n;
  function<bool(int)> pred = [&](int x) {
    for (int i = 1; i <= x; ++i)
      if (a[n * 2 - i] + a[n * 2 - x * 2 + i - 1] < m)
        return false;
    return true;
  };
  while (l < r) {
    int mid = (l + r + 1) >> 1;
    if (pred(mid))
      l = mid;
    else
      r = mid - 1;
  }
  int ans = 0;
  int x = l;
  for (int i = 1; i <= x; ++i)
    ans = max(ans, a[n * 2 - i] + a[n * 2 - x * 2 + i - 1] - m);
  for (int i = 0; i < n - x; ++i)
    ans = max(ans, (a[i] + a[(n - x) * 2 - i - 1]) % m);
  cout << ans;

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
