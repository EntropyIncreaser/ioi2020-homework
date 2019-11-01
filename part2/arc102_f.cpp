#include <bits/stdc++.h>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

const int N = 300010;

int n;
int a[N], pre[N], cnt[N];
int last[2];

void gg() {
  puts("No");
  exit(0);
}

int main() {
  scanf("%d", &n);
  for (int i = 1; i <= n; ++i)
    scanf("%d", &a[i]);
  for (int i = 1; i <= n; ++i)
    if ((a[i] ^ i) & 1)
      gg();
  last[0] = 0; last[1] = -1;
  for (int i = 1; i <= n; ++i) {
    if (i <= 2) pre[i] = a[i];
    else pre[i] = max(a[i], pre[i - 2]);
    if (pre[i] == i) {
      if (last[i & 1] < i - 2) {
        --cnt[last[i & 1] + 1];
        ++cnt[i];
      }
      last[i & 1] = i;
    }
  }
  for (int i = n; i; --i)
    if ((cnt[i] += cnt[i + 1]) > 1)
      gg();
  int a1 = 0, a2 = 0;
  for (int i = 1; i <= n; i += 2) {
    if (a[i] < a2) gg();
    if (a[i] < a1)
      a2 = a[i];
    else a1 = a[i];
  }
  a1 = 0; a2 = 0;
  for (int i = 2; i <= n; i += 2) {
    if (a[i] < a2) gg();
    if (a[i] < a1)
      a2 = a[i];
    else a1 = a[i];
  }
  puts("Yes");
  
  return 0;
}
