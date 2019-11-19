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

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int q;
  cin >> q;
  function<string(int, int, int, int)> query = [&](int a, int b, int c, int d) {
    int len = (a + b) / (min(a, b) + 1);

    function<bool(int)> pred = [&](int k) {
      int useA = k / (len + 1) * len;
      if (k % (len + 1)) useA += k % (len + 1);
      if (useA > a) return false;
      if (k % (len + 1))
        return (b - k / (len + 1) + a - useA) / (a - useA + 1) <= len;
      return (b - k / (len + 1) + a - useA + 1) / (a - useA + 1) <= len;
    };

    int l = 0, r = a + b;
    while (l < r) {
      int mid = ((r - l + 1) >> 1) + l;

      if (pred(mid))
        l = mid;
      else
        r = mid - 1;
    }
    
    function<char(int)> qc = [&](int k) {
      if (k <= l)
        return k % (len + 1) == 0 ? 'B' : 'A';
      k = a + b - k + 1;
      return k % (len + 1) == 0 ? 'A' : 'B';
    };

    string ret;
    for (int k = c; k <= d; ++k)
      ret.push_back(qc(k));
    return ret;
  };
  while (q--) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    cout << query(a, b, c, d) << '\n';
  }

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
