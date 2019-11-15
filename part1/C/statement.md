# agc020f Arcs on a Circle

时间限制：$5\texttt{s}$，空间限制：$512\texttt{MB}$

### 题目描述

你有一个周长为 $C$ 的圆，接下来你要在这个圆上放 $N$ 条弧，第 $i$ 个弧的弧长为 $L_i$。

每个弧 $i$ 都是在圆上均匀放置的：在圆上随机均匀选取一个位置，并将第 $i$ 个圆弧以其为中点放置。

注意每个弧的放置放置是互相独立的。例如他们可以任意相交或者一个盖住另一个。

请问圆上每个点都被至少一个圆弧覆盖的概率是多少？我们认为圆弧包含其端点。

### 限制

 - $2\le N \le 6$
 - $2\le K \le 50$
 - $1\le L_i < C$

### 输入格式

输入由以下形式给出：

> $N$ $C$

> $L_1\ L_2\ \dots\ L_N$

### 输出格式

输出一个实数表示圆上每个点都被至少一个圆弧覆盖的概率，你的答案被是为正确当且仅当与答案的差不超过 $10^{-11}$。

### 样例一

#### input

```plain
2 3
2 2
```

#### output

```plain
0.3333333333333333
```

#### explanation

两个圆弧的中点距离至少应该是 $1$，因此在周长为 $3$ 的圆上其概率为 $1/3$。

### 样例二

#### input

```plain
4 10
1 2 3 4
```

#### output

```plain
0.0000000000000000
```

#### explanation

即使所有弧的总长恰好为 $C$，即圆上每个点都被至少一个圆弧覆盖这一事件可能发生，其概率也可以是 $0$。

### 样例三

#### input

```plain
4 2
1 1 1 1
```

#### output

```plain
0.5000000000000000
```

### 样例四

#### input

```plain
3 5
2 2 4
```

#### output

```plain
0.4000000000000000
```

### 样例五

#### input

```plain
4 6
4 1 3 2
```

#### output

```plain
0.3148148148148148
```

### 样例六

#### input

```plain
6 49
22 13 27 8 2 19
```

#### output

```plain
0.2832340720702695
```