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

const int N = 5010;

char s[N * 2], ans[N], tmp[N];
int cur[N * 2];

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, k;
  cin >> n >> k >> (s + 1);
  for (int i = 1; i <= n; ++i)
    s[i + n] = s[n - i + 1];
  char c = *min_element(s + 1, s + n + 1);
  int len = 0;
  for (int i = 1; i <= n * 2; ++i)
    if (s[i] == c)
      len = max(len, cur[i] = cur[i - 1] + 1);
  if (k >= 15 || (len << (k - 1)) >= n) {
    for (int rep = 0; rep < n; ++rep)
      putchar(c);
    putchar('\n');
    return 0;
  }
  for (int i = n + 1; i <= n * 2; ++i)
    if (cur[i] == len) {
      for (int j = 1; j <= (n - (len << (k - 1))); ++j)
        tmp[j] = s[i - len - j + 1];
      if (ans[1] == 0 || strcmp(ans + 1, tmp + 1) > 0)
        memcpy(ans + 1, tmp + 1, n - (len << (k - 1)));
    }
  for (int rep = 0; rep < len << (k - 1); ++rep)
    putchar(c);
  puts(ans + 1);

#ifdef LBT
  LOG("Time: %dms\n", int ((clock()
          -nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
  return 0;
}
