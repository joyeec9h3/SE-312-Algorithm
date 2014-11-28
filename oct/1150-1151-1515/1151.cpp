#define JDEBUG
#include <cstdio>
#include <queue>
#include <string>
#include <cstring>

using std::string;
using std::queue;
typedef unsigned int uint32_t;

#define ONE 4
#define THREE 12
#define FOUR 16

class Node {  // Node for search
 public:
  uint32_t state;  // binary encoded state, 4 bits for each square
  string path;  // path to get to this node
  Node(uint32_t s = 0, string p = ""): state(s), path(p) {}
};

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

// put A, B, C into an array so that they can be iterated
uint32_t (*OP[3])(uint32_t) = {A, B, C};
// the hash table, declared in global scope to put it on the heap
bool visited[50000];  //8! = 40320

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

// convert the board state to binary code
uint32_t toBinary(int board[]) {
  uint32_t state = 0;
  for (int i = 0; i < 8; ++i)  // e.g. (A8 = board[7] - 1) << (ONE * 7)
    state |= (board[i]) << (ONE * (7 - i)); // e.g. 8 = 0b111
  return  state;
}

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

int main(void) {
#ifdef JDEBUG
  freopen("test.in", "r", stdin);
  freopen("test.out", "w", stdout);
#endif
  int n;
  int board[8] = {1 , 2 , 3 , 4 ,
                  8 , 7 , 6 , 5};
  Node begin(toBinary(board));

  scanf("%d", &n);
  while (n != -1) {
    for (int i = 0; i < 8; ++i)
      scanf("%d", &(board[i]));
	int b = int(n); 
    Node end(toBinary(board));
    memset(visited, false, sizeof(visited));
    bfs(begin, end, n);
    scanf("%d", &n);
  }

  return 0;
}