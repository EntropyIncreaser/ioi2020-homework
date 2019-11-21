#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>

#include <algorithm>
#include <complex>
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

typedef complex<double> C;

const double PI = acos(-1.);

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, l;
  cin >> n >> l;
  vector<int> t(n);
  cin >> t;
  vector<double> theta(n);
  transform(t.begin(), t.end(), theta.begin(), [&](int t) { return 2 * PI * t / l; });
  function<C(double)> unit = [](double theta) {
    return C(cos(theta), sin(theta));
  };
  C ans = 0;
  C sum = 0;
  for (int i = 1; i < n - 1; ++i) {
    sum += unit(theta[i - 1] * .5);
    for (int j = i + 1; j < n; ++j) {
      C center = unit((theta[i] + theta[j]) * .5);
      double r = sin((theta[j] - theta[i]) * .25) * 2;
      C relative = sum * unit((theta[i] + theta[j]) * .25 - PI * .5);
      ans += center * (double)i + r * relative;
    }
  }
  ans /= n * (n - 1) * (n - 2LL) / 6;
  printf("%.15f %.15f\n", ans.real(), ans.imag());

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
