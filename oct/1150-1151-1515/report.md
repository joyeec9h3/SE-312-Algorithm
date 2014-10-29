##原题中文大意
有一个 2 &times; 4 的魔板（实际上可以用1 &times; 8 的一维数组表示），初始状态

	1 2 3 4
	8 7 6 5

有 A, B, C 三种操作，分别为

* A：上下两列交换

		8 7 6 5
		1 2 3 4

* B：每行循环右移

		4 1 2 3
		5 8 7 6

* C： 中间 4 格逆时针旋转

		1 7 2 4
		8 6 3 5

输入每三行一组：

* 第一行一个数字 n ，表示为达到目标状态允许的最大操作次数
* 后两行为目标状态

当 n 为 -1 时，输入结束

##算法及数据结构
本题最明显的解法是通过在状态空间中进行搜索求解，初步考虑使用 BFS 或者 DFS，而根据题意，使用 DFS 可能会陷入环路，且很可能在无解的分支中大量浪费时间。而使用 BFS 可以得到达到目标状态的最短路径，因为 BFS 中短的路径一定先于长的路径被搜索，若最短路径长度为 n ，那么所有长度为 m > n 的路径一定在找到最短路径前后才能被搜索，而只要在找到最短路后便退出，则不会浪费时间继续搜索更长的路径。因此当使用 BFS 的时候，我们可以保证找到的是最短路径，且中间不会有多余的步骤（即环路，因为包含环路的路径去掉环路后长度必然更小，因此更早被搜索到），需要搜索的状态数目也不会太多。

为了记录状态是否已经被搜索，需要一个支持快速查找的数据结构，初步考虑，可以使用：

1. 二维数组，每一行为搜索过的状态的一位数组表示
2. 树状结构
3. 哈希表

其中 1 的空间要求明显太高，并且不容易进行查找（数组中的快速查找一般是排序 + 二分，但这个数据结构需要频繁插入，很难维护有序）。2 的空间和时间要求都比较合适，可接受频繁插入，但是实现起来较为复杂，虽然使用 STL 可以一定程度上简化实现。3 的空间要求介于 1 和 2 之间，查找速度是最快的，由于题目本身的特性很容易找到合适的哈希函数，且由于题目本身的状态空间并不大，所以最适合这道题目。

综上考虑，我们选择使用哈希来保存搜索过的状态。本题可以直接从题目里衍生出哈希函数，因为状态其实是 1 ~ 8 这 8 个数字的组合，我们可以将每个状态，即这 8 个数字的排法，用一个整数表示出来。这个整数可以用：

1. 以 10 进制将各行链接起来的一位数组表示成一个整数，如初始状态可表示为 `12348765`
2. 以 8 进制表示，初始状态为 `012348765`
3. 以 16 进制表示，初始状态为 `0x12348765`

其中如果用 1 表示，实现 A, B, C 三种操作的变换比较麻烦。而 2,3 可以使用掩码和位运算快速实现操作。2 所需整数的范围应当是 `0 ~ (2^24)-1`，可用 `int` 装下。3 所需整数的范围是  `0 ~ (2^32)-1`，可用 `unsigned int` 装下。由于 3 的掩码使用 `f` 来过滤各个数字，而 2 的掩码使用 `7` 来过滤各个数字，而且 3 的字面值表示开头为 `0x`，2 的字面值表示开头为 `0`，所以 3 的掩码字面值可读性更高一些，因此这里选择 3。

此外，由于状态空间其实里是 1 ~ 8 的组合而不是 1 ~ 8 的排列，因此可以使用康托展开将哈希进行压缩，不为不可能出现的状态在哈希表内预留位置，如

	1 1 1 1
    2 2 2 2

综上所述，这题我们使用 BFS 进行搜索，使用哈希表存储搜索过的状态，用每个状态各数字排列的 16 进制表示做哈希函数，用康托展开压缩哈希表的大小。

##解题思路
从初始状态开始进行 BFS，先将初始状态入列，接着对列中的每一个状态，分别进行 A, B, C 操作，将每个操作的结果及其操作路径依次入列，一直到找到目标状态，或者操作路径长度已经超过 n 为止。

这里有两个需要注意的地方：

1. 极端情况为目标状态即初始状态，因此在初始状态入列后，需要先判断一次其是否为目标状态，若是，直接输出并返回
2. 可以在操作产生新状态后，立刻判断新状态是否为目标状态，如果是，输出并返回。此时还需要注意额外判断新状态的路径长度是否超过 n。

##逐步求精
为了方便阅读，我们按照 POSIX 标准定义

	typedef unsigned int uint32_t;

* 需要一个状态节点数据结构`Node`，其中包括
	* `uint32_t state` 表示状态的哈希
	* `string path` 表示从目标状态到达当前状态所需的操作步骤
* 需要 `A`, `B`, `C` 三个函数，接收状态哈希，进行相应的位操作，返回操作后的状态的哈希
	* `uint32_t A(uint32_t n)`
	* `uint32_t B(uint32_t n)`
	* `uint32_t C(uint32_t n)`
* 需要康托展开函数 `cantor`，将状态的哈希唯一地转换为一个小于 8! 的数字
	* `uint32_t cantor(uint32_t  state)`
* 需要一个函数 `toBinary`，将以数组表示的状态转化为哈希
	* `uint32_t toBinary(int board[])`
* 需要一个哈希表数组 `visited` 保存访问过的状态，由于这个哈希表长度为 8!，这里将它开为全局变量，放在堆上，避免超过栈的大小。为了方便写长度设为 50000，略大于 8!
	* `bool visited[50000];`
* 需要 BFS 函数 `bfs`，接收初始状态，目标状态，最大路径长度，进行搜索并输出对应结果
	* `void bfs(Node begin, Node end, int n)`
* `main`函数进行输入，调用 bfs，初始化变量等操作
##注释清单


定义状态节点数据结构：

	class Node {  // Node for search
	 public:
	  uint32_t state;  // binary encoded state, 4 bits for each square
	  string path;  // path to get to this node
	  Node(uint32_t s = 0, string p = ""): state(s), path(p) {}
	};

三个操作

	// switch two rows
	uint32_t A(uint32_t n) {
	  return  ((n >> FOUR)      // shift the upper 4 groups down
	                |            // merge
	           ((n & 0x0000ffff) << FOUR));  // shift the lower 4 squares up
	}
	
	// circularly shift each row to the left
	uint32_t B(uint32_t n) {
	  // shift last three square in each row to the left
	  return  (((n & 0xfff0fff0) >> ONE)
	                   |   // merge
	           ((n & 0x000f000f) << THREE));
	  // shift first squares in each row to the beginning
	}
	
	// rotate the four squares in the center clockwise
	uint32_t C(uint32_t n) {
	  return  (n & 0xf00ff00f)  // mask out corners
	          | ((n & 0x000000f0) << ONE)  // shift A7 left
	          | (n & 0x00000f00) << FOUR  // shift A6 UP
	          | (n & 0x0f000000) >> ONE   // shift A2 right
	          | (n & 0x00f00000) >> FOUR;  // shift A3 down
	}

康托展开

	// convert a 32-bit state to an integer no larger than 8!
	uint32_t cantor(uint32_t  state) {
	  uint32_t a[8], x = 0,
	           fac[8] = {0, 1, 2, 6, 24, 120, 720, 5040};  // fac[i] = i!
	
	  for (int i = 0; i < 8; ++i)
	    a[7 - i] = ( state >> (4 * i)) & 0xf;  // a[i] = board[i]
	
	  for (int i = 0; i < 8; ++i) {
	    int count = 0; // number of digits smaller than a[i]
	    for (int j = i + 1; j < 8; ++j)
	      if (a[j] < a[i])
	        ++count;
	    x += fac[7 - i] * count;
	  }
	  return x;
	}

数组到哈希的转换函数

	// convert the board state to binary code
	uint32_t toBinary(int board[]) {
	  uint32_t state = 0;
	  for (int i = 0; i < 8; ++i)  // e.g. (A8 = board[7] - 1) << (ONE * 7)
	    state |= (board[i]) << (ONE * (7 - i)); // e.g. 8 = 0b111
	  return  state;
	}

为了方便，将三个操作放在一个函数指针数组里，易于遍历

	// put A, B, C into an array so that they can be iterated
	uint32_t (*OP[3])(uint32_t) = {A, B, C};

哈希表

	// the hash table, declared in global scope to put it on the heap
	bool visited[50000];  //8! = 40320

BFS 函数

	// search from begin to the end, until the path is longer than n
	// or the end is found
	void bfs(Node begin, Node end, int n) {
	  // initial state is the target state
	  if (begin.state == end.state) {
	    printf("0 \n");
	    return;
	  }
	
	  queue<Node> q;
	  q.push(begin);
	  visited[cantor(begin.state)] = true;
	
	  while (!q.empty()) {
	    Node head = q.front();
	    q.pop();
	
	    if (head.path.length() > n) {
	      printf("-1\n");
	      return;
	    }
	
	    for (int i = 0; i < 3; ++i) {
	      uint32_t new_state = OP[i](head.state);
	      int hash = cantor(new_state);
	      if (!visited[hash]) {
	        visited[hash] = true;
	        string new_path = head.path + (char)('A' + i);
	        Node new_node(new_state, new_path);
	
	        if (new_state == end.state && new_path.length() <= n) {  // found
	          printf("%d %s\n", new_path.length(), new_path.c_str());
	          return;
	        }
	
	        q.push(new_node);
	      }
	    }
	  }
	}

##测试数据
使用如下main函数生成可到达的状态

	int main(void) {
	  int n;
	  int board[8] = {1 , 2 , 3 , 4 ,
	                  8 , 7 , 6 , 5};
	  char buf[100];
	
	  scanf("%d", &n);
	  while (n != -1) {
	  	int x = toBinary(board);

	    scanf("%s", buf);
	    string ops(buf);
	    for (int i = 0; i < ops.length(); ++i)
	    	x = OP[ops[i] - 'A'](x);

	   	printf("%d\n", ops.length());
		for (int i = 1; i <= 8; ++i)
			printf("%d%c", (x >> (ONE * (8 - i))) & 0xf, i % 4 == 0 ? '\n' : ' ');

	    scanf("%d", &n);
	  }
	
	  return 0;
	}

测试数据

	4
	1 2 3 4
	8 7 6 5
	4
	5 8 7 6
	4 1 2 3
	3
	8 7 6 5
	1 2 3 4
	1
	4 1 2 3
	5 8 7 6
	2
	1 7 2 4
	8 6 3 5
	3
	5 1 8 6
	4 2 7 3
	25
	5 8 2 3
	4 1 6 7
	3
	5 8 2 3
	4 1 6 7
	6
	3 5 4 7
	6 1 2 8
	11
	2 5 6 1
	7 4 3 8
	10
	4 1 5 3
	2 8 7 6
	19
	7 3 6 1
	2 5 8 4
	3
	1 3 6 4
	8 2 7 5
	10
	6 5 7 2
	3 4 8 1
	29
	2 3 5 1
	8 6 7 4
	-1

预期输出

	0 
	2 AB
	1 A
	1 B
	1 C
	3 ABC
	20 BBCBBCABCBCBBBCBCBCB
	-1
	6 ABCABC
	5 BBBCC
	-1
	6 BBCABC
	3 ACA
	4 BCAB
	12 BCABCBCCBCBB

##复杂度分析，优化与改进
