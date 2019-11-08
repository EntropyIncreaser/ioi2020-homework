# agc037e Reversing and Concatenating

时间限制：$2\texttt{s}$，空间限制：$1024\texttt{MB}$

### 题目描述

Takahashi 有一个长为 $N$ 的小写英文字符串 $S$。他会在这个字符串上进行 $K$ 次操作：

 - 记 $T$ 为 $S$ 这个字符串翻转后的结构，$U$ 为将 $S$ 和 $T$ 拼接得到的字符串。
 - 选取 $S'$ 是 $U$ 的一个长度为 $N$ 的子串，然后将 $S$ 替换掉 $S'$。

请你找出将字符串 $S$ 执行 $K$ 次操作后能得到的所有字符串中，字典序最小的那个。

### 限制

 - $1\le N \le 5000$
 - $1\le K \le 10^9$
 - $|S| = N$
 - $|S|$ 只包含英文小写字母。

### 输入格式

输入由以下形式给出：

> $N$ $K$

> $S$

### 输出格式

输出一个字符串，表示将字符串 $S$ 执行 $K$ 次操作后能得到的所有字符串中，字典序最小的那个。

### 样例一

#### input

```plain
5 1
bacba
```

#### output

```plain
aabca
```

### 样例二

#### input

```plain
10 2
bbaabbbaab
```

#### output

```plain
aaaabbaabb
```
