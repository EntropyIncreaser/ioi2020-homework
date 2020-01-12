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

string naive(string ba) {
  string ret = ba;
  while (!ba.empty()) {
    ba.erase(0, 1);
    ba.erase(ba.find_first_of('a'), 1);
    ret = max(ret, ba);
  }
  return ret;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  
  int n;
  string s;
  cin >> n >> s;
  vector<int> pre(n * 2);
  int cur = 0;
  for (int i = 0; i < n * 2; ++i) {
    if (s[i] == 'a') ++cur;
    else --cur;
    pre[i] = cur;
  }
  string ans;
  vector<string> vs;
  int last = 0;
  for (int i = 0; i < n * 2; i += 2) {
    if (pre[i] < 0) {
      int r = find(pre.begin() + i, pre.end(), 0) - pre.begin();
      last = r + 1;
      vs.push_back(naive(string(s.begin() + i, s.begin() + last)));
      i = r - 1;
    }
  }
  vector<int> match(n * 2);
  queue<int> q;
  for (int i = last; i < n * 2; ++i)
    if (s[i] == 'a')
      q.push(i);
    else {
      match[q.front()] = i;
      q.pop();
    }
  for (int i = last; i < n * 2; ++i) {
    if (s[i] == 'b') continue;
    ans += "ab";
    i = match[i];
  }
  reverse(vs.begin(), vs.end());
  for (const auto& str : vs)
    ans = max(ans, str + ans);
  cout << ans;

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
