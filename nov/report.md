# 1153 马的周游问题 解题报告

12330402 · 张秋怡 · 12 级计应 2 班

## 原题大意

在一个 8 &times; 8 的国际象棋棋盘中的某个位置有一只马，如果它能够按照“日”字形走法走遍整个棋盘的每一个方格，而且每格只经过一次，这样一种走法则称马的周游路线，试设计一个算法，从给定的起点出发，找出它的一条周游路线。

     1   2   3   4   5   6   7   8
    9   10  11  12  13  14  15  16
    17  18  19  20  21  22  23  24
    25  26  27  28  29  30  31  32
    33  34  35  36  37  38  39  40
    41  42  43  44  45  46  47  48
    49  50  51  52  53  54  55  56
    57  58  59  60  61  62  63  64

输入有若干行。每行一个整数N(1 &#8804; N &#8804; 30)，表示马的起点。最后一行用 -1 表示结束，不用处理。

## 算法及数据结构

本题即骑士巡逻([Knight's tour](http://en.wikipedia.org/wiki/Knight%27s_tour)）问题，为汉密尔顿路径问题（[Hamiltonian path problem](http://en.wikipedia.org/wiki/Hamiltonian_path_problem)）的一个特例。由于这是一个 8 &times; 8 的棋盘，本题一定有解 [1]。

骑士巡逻的常见解决方案有：

1. 暴力搜索（对于 8 &times; 8 的棋盘，状态空间大小约为 4&times;10<sup>51</sup>）
2. 分治（多项式时间 [2]）
3. 使用 Warnsdorff's rule 的启发式搜索（通常情况下可在线性时间内得解 [3]）

对于本题，我们可以使用 Warnsdorff's rule 进行启发式搜索（此处按照题意须为DFS）求解。Warnsdorff's rule 内容如下：

1. 对所有能从当前节点访问到且尚未被访问过的节点，统计能从它们继续访问的节点的数目。
2. 按照后续可访问节点数目，从大到小进行下一步搜索。

题目需要用到的数据结构：

* `struct Node`：包括表示坐标的`x`, `y` 和表示可访问后续节点数的 `reachability`
* `int board[HEIGHT][WIDTH]`：棋盘各格编号
* `int from[SIZE]`：记录路径，`from[i]` 是第 `i` 格的上一步
* `bool visited[HEIGHT][WIDTH]` 用于搜索时判断该节点是否已经访问

## 解题思路

每一个节点的下一步的搜索过程可以这样实现：

1. 如果已经周游完毕，则输出路径并返回 true
2. 否则，对于能从该节点继续访问到的每一个节点，统计它们的后续节点的数目
3. 将能够访问的下一个节点按照后续节点的数目排序
4. 按后续节点数目，从大到小继续进行 DFS 搜索（搜索前将其标记为已访问）。如果已经无后续节点，返回 false。
5. 如果被搜索的节点的 DFS 返回 false，表明无法从此节点求解，将其重新标记为未访问以备其他节点继续搜索。

## 注释清单

```cpp
// 12330402 Qiuyi Zhang
 
#include<cstdio>
#include<cstring>
#include<algorithm>
#include<vector>

#define WIDTH 8
#define HEIGHT 8
#define SIZE WIDTH * HEIGHT
#define NEIGHBOR 8

struct Node {
  int x, y, reachability;
  Node(int x_ = 0, int y_ = 0) {
    x = x_; y = y_; reachability = 0;
  }
};

int board[HEIGHT][WIDTH];  // layout of the board
int from[SIZE];  // record the route
bool visited[HEIGHT][WIDTH];

int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};


// for sorting
bool cmp(Node a, Node b) {
  return a.reachability < b.reachability;
}


// test if a node is in the board
// and not visited
bool valid(Node node) {
  return (node.x >= 0 && node.y >= 0
          && node.x < HEIGHT && node.y < WIDTH
          && !visited[node.x][node.y]);
}


bool dfs(Node start, int step) {
  from[step] = board[start.x][start.y];

  if (step == SIZE - 1) {  // the tour is finished
    for (int i = 0; i < SIZE; i++)
      printf("%d%c", from[i], i == SIZE - 1 ? '\n' : ' ');
    return true;
  } else {
    // Warnsdorff's rule:
    // nodes with better reachability will be checked first

    // calculate the reachability of reachable nodes
    std::vector<Node> v;
    for (int i = 0; i < NEIGHBOR; i++) {
      Node probe(start.x + dx[i], start.y + dy[i]);
      if (valid(probe)) {  // for each valid neighbor `probe`
        // calculate the reachability for `probe`
        for (int j = 0; j < NEIGHBOR; j++) {
          Node ahead(probe.x + dx[j], probe.y + dy[j]);
          if (valid(ahead))
            probe.reachability++;
        }

        v.push_back(probe);
      }
    }
    // sort the reachable nodes by their own reachability
    std::sort(v.begin(), v.end(), cmp);

    // DFS, note `v` will be empty now if `start` is a dead end
    for (int i = 0; i < v.size(); i++) {
      visited[v[i].x][v[i].y] = true;  // check this one out
      if (dfs(v[i], step + 1)) return true;  // go deeper, found a route
      visited[v[i].x][v[i].y] = false;  // dead end, restore
    }

    return false;  // not returned before = all dead ends from here
  }
}

int main(int argc, char *argv[]) {
#ifdef JDEBUG
  freopen("1153.in", "r", stdin);
  freopen("1153.out", "w", stdout);
#endif
  int square = 1, n;

  // initialize the board
  for (int i = 0; i < HEIGHT; i++)
    for (int j = 0; j < WIDTH; j++)
      board[i][j] = square++;

  while (scanf("%d", &n) && n != -1) {
    Node start((n - 1) / WIDTH, (n - 1) % WIDTH);

    // initialize
    memset(visited, false, sizeof(visited));
    memset(from, -1, sizeof(from));

    visited[start.x][start.y] = true;
    dfs(start, 0);
  }

  return 0;
}
```

## 测试数据

测试数据
```
1
2
3
4
5
6
7
8
9
10
-1

```

预期输出
```
1 11 17 2 12 6 16 31 48 63 53 59 49 34 51 57 42 25 10 4 14 8 23 40 55 61 46 56 62 52 58 41 26 9 19 36 21 38 32 15 5 22 7 24 39 29 44 27 33 50 60 43 28 45 35 18 3 13 30 20 37 47 64 54
2 17 34 49 59 42 57 51 61 55 40 23 8 14 24 7 13 3 9 19 25 10 4 21 6 16 31 48 63 53 36 46 56 62 52 58 41 26 11 1 18 33 43 60 50 35 45 28 38 44 27 12 29 39 54 64 47 37 20 30 15 32 22 5
3 9 26 41 58 52 62 56 39 24 7 13 23 8 14 4 10 25 42 57 51 61 55 40 46 63 48 31 16 6 21 36 19 2 17 34 49 59 53 38 32 15 30 47 64 54 60 45 35 50 44 29 12 27 33 43 37 20 5 22 28 18 1 11
4 10 25 42 57 51 61 55 40 23 8 14 24 7 13 3 9 19 2 17 34 49 59 44 50 60 54 64 47 62 56 39 45 30 15 32 38 48 63 53 36 46 29 35 52 58 41 26 20 5 11 1 18 28 43 33 27 37 22 12 6 16 31 21
5 15 32 47 64 54 60 50 33 18 1 11 17 2 12 6 16 22 7 24 39 56 62 52 58 41 26 9 3 13 23 8 14 4 10 20 30 40 55 61 51 57 42 25 35 45 28 43 49 59 53 63 48 38 44 34 19 29 46 36 21 31 37 27
6 16 31 48 63 53 59 49 34 17 2 12 18 1 11 5 15 32 47 64 54 60 50 33 43 58 41 26 9 3 13 7 24 39 56 62 52 37 22 28 38 55 61 44 27 21 4 10 20 14 8 23 40 46 29 35 45 30 36 51 57 42 25 19
7 24 39 56 62 52 58 41 51 57 42 59 49 34 17 2 12 6 16 22 32 15 5 11 1 18 33 50 60 43 26 9 3 13 28 45 55 61 44 54 64 47 53 63 48 38 23 8 14 31 37 27 21 4 10 20 30 40 46 36 19 29 35 25
8 23 40 55 61 51 57 42 59 49 34 17 2 12 6 16 31 48 63 53 38 32 15 5 22 7 24 14 4 21 11 1 18 28 13 3 9 19 25 10 27 33 50 44 29 39 56 46 36 26 41 58 43 60 54 64 47 62 52 37 20 30 45 35
9 3 13 7 24 39 56 62 52 58 41 26 11 1 18 33 50 60 43 49 59 42 57 51 61 55 45 35 25 10 4 14 8 23 40 46 63 53 36 19 29 44 34 17 2 12 27 21 6 16 31 48 54 64 47 37 20 30 15 5 22 32 38 28
10 25 42 57 51 61 55 40 23 8 14 4 19 9 3 13 7 24 39 56 62 52 58 41 26 36 46 63 48 31 16 6 21 15 32 38 53 59 49 34 17 2 12 29 44 27 33 50 60 43 28 45 35 18 1 11 5 22 37 54 64 47 30 20

```
## 复杂度分析，优化与改进

如果使用暴力搜索，此题几乎不可能在正常时间内求解。使用Warnsdorff's rule 进行启发式搜索则可以在（相对于边数的）线性时间内求解 [3]。在 sicily 上耗时 0.0s，内存 308KB。

## 参考文献
1. Conrad, A.; Hindrichs, T.; Morsy, H. &amp; Wegener, I. (1994). "Solution of the Knight's Hamiltonian Path Problem on Chessboards". Discrete Applied Mathematics 50 (2): 125–134. doi:10.1016/0166-218X(92)00170-Q.
2. Parberry, Ian (1997). "An Efficient Algorithm for the Knight's Tour Problem". Discrete Applied Mathematics 73: 251–260. doi:10.1016/S0166-218X(96)00010-8.
3. Pohl, Ira (July 1967). "A method for finding Hamilton paths and Knight's tours". Communications of the ACM 10 (7): 446–449. doi:10.1145/363427.363463.