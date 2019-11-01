#include <bits/stdc++.h>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

struct Fac {
  int a;
  ll b;
  
  Fac(int a = 0, ll b = 1) : a(a), b(b) {}
  
  bool operator<(const Fac& rhs) const { return a * rhs.b > b * rhs.a; }
};

const int N = 100010;

int k, n, m;
int a[N];

int typ[N];
vector<pair<int, int> > dif[N];
pair<int, int> ass[N];
pair<Fac, int> mult[N];

int main() {
  scanf("%d%d%d", &k, &n, &m);
  for (int i = 1; i <= k; ++i)
    scanf("%d", &a[i]);
  int cnt = 0;
  for (int id = 1; id <= n; ++id) {
    int t, i, b;
    scanf("%d%d%d", &t, &i, &b);
    typ[id] = t;
    if (t == 1)
      ass[i] = max(ass[i], make_pair(b, id));
    else if (t == 2)
      dif[i].push_back(make_pair(b, id));
    else
      mult[++cnt] = make_pair(Fac(b - 1, 1), id);
  }
  for (int i = 1; i <= k; ++i) {
    if (ass[i].first > a[i])
      dif[i].push_back(make_pair(ass[i].first - a[i], ass[i].second));
    ll sum = a[i];
    sort(dif[i].begin(), dif[i].end(), greater<pair<int, int> >());
    for (size_t j = 0; j < dif[i].size(); ++j) {
      mult[++cnt] = make_pair(Fac(dif[i][j].first, sum), dif[i][j].second);
      sum += dif[i][j].first;
    }
  }
  m = min(m, cnt);
  sort(mult + 1, mult + cnt + 1);
  printf("%d\n", m);
  int cn = 0;
  for (int j = 1; j <= 3; ++j)
    for (int i = 1; i <= m; ++i)
      if (typ[mult[i].second] == j)
        printf("%d%c", mult[i].second, " \n"[++cn == m]);
  return 0;
}
