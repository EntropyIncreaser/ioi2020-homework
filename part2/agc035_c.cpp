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

int main() {
  int n;
  scanf("%d", &n);
  if (!(n & (n - 1))) {
    puts("No");
    return 0;
  }
  puts("Yes");
  int m = n;
  printf("%d %d\n", 1 + n, 3);
  if (n % 2 == 0) --m;
  for (int i = 2; i < m; i += 2) {
    printf("%d %d\n", i, i + 1);
    printf("%d %d\n", i, 1);
    printf("%d %d\n", i + n, i + 1 + n);
    printf("%d %d\n", i + n + 1, 1);
  }
  if (n % 2 == 0) {
    int x = n & -n;
    int y = n - x;
    printf("%d %d\n", x, n);
    printf("%d %d\n", y + n + 1, n * 2);
  }
  return 0;
}
