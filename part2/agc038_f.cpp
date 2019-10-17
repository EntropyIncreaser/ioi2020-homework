#include <cstdio>
#include <cstring>

#include <vector>
#include <queue>

using namespace std;


template <class T>
class is_zero {
public:
  bool operator()(const T& x) const;
};

template <>
class is_zero<int> {
public:
  bool operator()(const int& x) const { return x == 0; }
};

template <class WeightT = int, class ZeroPredicator = is_zero<WeightT> >
class flow_graph {
public:
  typedef WeightT weight_type;
  typedef const weight_type& const_weight_reference;
  typedef weight_type* weight_array_type;

private:
  struct edge {
    int v;
    weight_type w;
    edge *next, *rev;
  };

  int v, e;
  edge *pool, *used;
  edge** adj;
  ZeroPredicator zero_predicator;

  edge* add_edge(int u, int v, const_weight_reference w) {
    edge* p = used;
    ++used;
    p->v = v;
    p->w = w;
    p->next = adj[u];
    adj[u] = p;
    return p;
  }

  bool bfs(int s, int t, int* level) {
    memset(level, -1, sizeof(int) * v);
    std::queue<int> q;
    q.push(s);
    level[s] = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (edge* p = adj[u]; p; p = p->next)
        if (!zero_predicator(p->w) && level[p->v] == -1) {
          level[p->v] = level[u] + 1;
          if (p->v == t)
            return true;
          q.push(p->v);
        }
    }
    return false;
  }

  weight_type dfs(int u, int t, weight_type lim, edge** cadj, int* level) {
    if (u == t)
      return lim;
    weight_type flow = 0;
    for (edge*& p = cadj[u]; p; p = p->next)
      if (!zero_predicator(p->w) && level[u] < level[p->v]) {
        weight_type f = dfs(p->v, t, std::min(p->w, lim), cadj, level);
        flow += f;
        lim -= f;
        p->w -= f;
        p->rev->w += f;
        if (zero_predicator(lim))
          return flow;
      }
    return flow;
  }

  weight_type dfs(int u, int t, edge** cadj, int* level) {
    weight_type flow = 0;
    for (edge*& p = cadj[u]; p; p = p->next)
      if (!zero_predicator(p->w) && level[u] < level[p->v]) {
        weight_type f = dfs(p->v, t, p->w, cadj, level);
        flow += f;
        p->w -= f;
        p->rev->w += f;
      }
    return flow;
  }

public:
  flow_graph() {}

  flow_graph(int v, int e, ZeroPredicator zero_predicator = is_zero<WeightT>())
          : v(v), e(e), zero_predicator(zero_predicator) {
    pool = new edge[e * 2];
    adj = new edge*[v];
    memset(pool, 0, sizeof(edge) * e * 2);
    memset(adj, 0, sizeof(edge*) * v);
    used = pool;
  }

  ~flow_graph() {
    delete[] pool;
    delete[] adj;
  }

  void link(int u, int v, const_weight_reference w) {
    edge *p = add_edge(u, v, w), *q = add_edge(v, u, 0);
    p->rev = q;
    q->rev = p;
  }

  void link(int u, int v, const_weight_reference w, const_weight_reference rw) {
    edge *p = add_edge(u, v, w), *q = add_edge(v, u, rw);
    p->rev = q;
    q->rev = p;
  }

  /**
   * Calculate the maximum flow from `s` to `t`. Using the dinic algorithm.
   * time complexity: O(n^2e)
   **/
  weight_type max_flow(int s, int t) {
    int* level = new int[v];
    edge** cadj = new edge*[v];
    weight_type flow = 0;
    while (bfs(s, t, level)) {
      memcpy(cadj, adj, sizeof(edge*) * v);
      flow += dfs(s, t, cadj, level);
    }
    delete[] level;
    delete[] cadj;
    return flow;
  }
};

int main() {
  int n;
  scanf("%d", &n);
  vector<int> p(n), q(n);
  int cnt = 0;
  vector<int> vp(n, -1), vq(n, -1);
  for (int i = 0; i < n; ++i)
    scanf("%d", &p[i]);
  for (int i = 0; i < n; ++i)
    scanf("%d", &q[i]);
  for (int i = 0; i < n; ++i) {
    int x = i;
    if (vp[x] != -1) continue;
    while (vp[x] == -1) {
      vp[x] = cnt;
      x = p[x];
    }
    ++cnt;
  }
  for (int i = 0; i < n; ++i) {
    int x = i;
    if (vq[x] != -1) continue;
    while (vq[x] == -1) {
      vq[x] = cnt;
      x = q[x];
    }
    ++cnt;
  }
  flow_graph<int> g(cnt + 2, n);
  int s = cnt, t = s + 1, pre = 0;
  for (int i = 0; i < n; ++i) {
    if (p[i] == i) {
      if (q[i] == i) {
        ++pre;
      } else {
        g.link(s, vq[i], 1);
      }
    } else {
      if (q[i] == i) {
        g.link(vp[i], t, 1);
      } else {
        g.link(vp[i], vq[i], 1, p[i] == q[i]);
      }
    }
  }
  printf("%d\n", n - pre - g.max_flow(s, t));
  return 0;
}

