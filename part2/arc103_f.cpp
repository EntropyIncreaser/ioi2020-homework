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

const int N = 100010;

int prt[N], sub[N];
ll d[N], sum[N];

void gg() {
  cout << "-1\n";
  exit(0);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  for (int i = 1; i <= n; ++i)
    cin >> d[i];
  map<ll, int, greater<ll>> mp;
  for (int i = 1; i <= n; ++i)
    mp.insert(make_pair(d[i], i));
  fill(sub + 1, sub + n + 1, 1);
  for (int rep = 1; rep < n; ++rep) {
    int u = mp.begin()->second;
    mp.erase(mp.begin());
    ll goal = d[u] - (n - 2 * sub[u]);
    auto it = mp.find(goal);
    if (it == mp.end())
      gg();
    prt[u] = it->second;
    sub[it->second] += sub[u];
    sum[it->second] += sum[u] + sub[u];
  }
  int rt = mp.begin()->second;
  if (sum[rt] != d[rt])
    gg();
  for (int i = 1; i <= n; ++i)
    if (i != rt)
      cout << i << ' ' << prt[i] << '\n';

  return 0;
}
