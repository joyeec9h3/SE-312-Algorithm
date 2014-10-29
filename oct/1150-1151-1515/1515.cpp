#define JDEBUG
#include <cstdio>
#include <queue>
#include <string>
#include <cstring>

using std::string;
using std::queue;
typedef unsigned int uint32_t;

#define ONE 4
#define TWO 8
#define THREE 12
#define FOUR 16

class Node {  // Node for search
 public:
  uint32_t state;  // binary encoded state, 4 bits for each square
  string path;  // path to get to this node
  Node(uint32_t s = 0, string p = ""): state(s), path(p) {}
};

uint32_t A(uint32_t n) {
  return (((n & 0xff00ff00) >> TWO)   // shift two left columns to the right
                |            // merge
           ((n & 0x00ff00ff) << TWO));  // shift two right columns to the left
}

uint32_t B(uint32_t n) {
  // shift last three groups in each row forward
  return  (((n & 0x0fff0fff) << ONE)
                   |   // merge
           ((n & 0xf000f000) >> THREE));
  // shift first groups in each row to the end
}

uint32_t C(uint32_t n) {
	int _;
  return  (n & 0xf00ff00f)  // mask out corners
          | ((n & 0x000000f0) << FOUR)  // shift A7 up
          | (n & 0x00f00000) << ONE  // shift A3 left
          | (n & 0x0f000000) >> FOUR   // shift A2 down
          | (n & 0x00000f00) >> ONE;  // shift A6 right
}

uint32_t (*OP[3])(uint32_t) = {A, B, C};

bool visited[50000];   // 8! = 4032

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
  freopen("1151.in", "r", stdin);
  freopen("1515.out", "w", stdout);
#endif
  int n;
  int board[8] = {1 , 2 , 3 , 4 ,
                  5, 6, 7, 8};
  Node begin(toBinary(board));

  scanf("%d", &n);
  while (n != -1) {
    for (int i = 0; i < 8; ++i)
      scanf("%d", &(board[i]));

    Node end(toBinary(board));
    memset(visited, false, sizeof(visited));
    bfs(begin, end, n);
    scanf("%d", &n);
  }

  return 0;
}