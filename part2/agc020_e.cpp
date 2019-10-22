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
#include <unordered_map>
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

typedef __int128 BS;

const int N = 110, P = 998244353;

int n;
char s[N];

map<BS, int> dp1;

int dfs(BS bs) {
  if (bs == 1) return 1;
  if (dp1.count(bs))
    return dp1[bs];
  int ret = dfs(bs >> 1) * ((bs & 1) + 1) % P;
  int i = 100;
  while (!((bs >> i) & 1)) --i;
  BS j = bs ^ (BS(1) << i);
  for (int l = 1; l <= i; ++l) {
    BS mask = j & ((BS(1) << l) - 1);
    for (int k = 2; k * l <= i; ++k) {
      mask &= j >> ((k - 1) * l);
      ret = (ret + dfs((BS(1) << l) | mask) * (ll)dfs((BS(1) << (i - k * l)) | (j >> (k * l)))) % P;
    }
  }
  return dp1[bs] = ret;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> (s + 1);
  n = strlen(s + 1);
  for (int i = 1; i <= n; ++i)
    s[i] -= '0';
  BS bs(1);
  bs <<= n;
  for (int i = 0; i < n; ++i)
    if (s[i + 1])
      bs |= BS(1) << i;
  cout << dfs(bs) << '\n';

  return 0;
}
