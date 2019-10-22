#include <cstdio>
#include <cmath>
#include <ctime>
#include <cstring>
#include <cstdlib>

#include <algorithm>
#include <numeric>
#include <limits>
#include <functional>
#include <stack>
#include <vector>
#include <set>
#include <map>
#include <queue>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

const int N = 200010;

int n;
int a[N];

bool pred(int x) {
  map<int, int> cur;
  for (int i = 2; i <= n; ++i) {
    if (a[i] > a[i - 1])
      continue;
    int pos = a[i] - 1;
    cur.erase(cur.lower_bound(a[i]), cur.end());
    while (pos && cur[pos] == x - 1)
      cur.erase(pos--);
    if (cur[pos] == x - 1)
      return false;
    ++cur[pos];
  }
  return true;
}

int main() {
#ifdef LBT
  freopen("test.in", "r", stdin);
  int nol_cl = clock();
#endif

  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    scanf("%d", &a[i]);
  bool flag = true;
  for (int i = 2; i <= n; ++i)
    flag &= a[i] > a[i - 1];
  if (flag) {
    puts("1");
    return 0;
  }
  int l = 2, r = n;
  while (l < r) {
    int mid = (l + r) >> 1;
    if (pred(mid))
      r = mid;
    else
      l = mid + 1;
  }
  printf("%d\n", l);

#ifdef LBT
  LOG("Time: %dms\n", int((clock() - nol_cl) / (double)CLOCKS_PER_SEC * 1000));
#endif
	return 0;
}
