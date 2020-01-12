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

char s[300010];

int norm(int x) { return x >= P ? x - P : x; }

void add(int& x, int y) {
  if ((x += y) >= P)
    x -= P;
}

void sub(int& x, int y) {
  if ((x -= y) < 0)
    x += P;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> (s + 1);
  int n = strlen(s + 1);
  s[0] = '1';
  s[n + 1] = '1';
  vector<int> dp(6);
  dp[0] = 1;
  for (int i = 0; i <= n + 1; ++i) {
    vector<int> newDp(6);
    if (s[i] != '1') {
      for (int j = 0; j < 6; ++j)
        add(newDp[j % 3], dp[j]);
    }
    if (s[i] != '0') {
      for (int j = 0; j < 6; ++j)
        add(newDp[j % 3 + 3], dp[j]);
      if (i & 1) {
        sub(newDp[3], dp[3]);
        add(newDp[4], dp[3]);
      } else {
        sub(newDp[4], dp[4]);
        add(newDp[5], dp[4]);
      }
    }
    dp = newDp;
  }
  cout << dp[5] << '\n';

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
