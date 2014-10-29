//#define JDEBUG
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


// convert the board state to binary code
uint32_t toBinary(int board[]) {
  uint32_t state = 0;
  for (int i = 0; i < 8; ++i)  // e.g. (A8 = board[7] - 1) << (ONE * 7)
    state |= (board[i]) << (ONE * (7 - i)); // e.g. 8 = 0b111
  return  state;
}

void print(uint32_t x) {
	for (int i = 1; i <= 8; ++i)
		printf("%d%c", (x >> (ONE * (8 - i))) & 0xf, i % 4 == 0 ? '\n' : ' ');
}

// search from begin to the end, until the path is longer than n
// or the end is found

int main(void) {
#ifdef JDEBUG
  freopen("test.in", "r", stdin);
  freopen("test.out", "w", stdout);
#endif
  int n;
  int board[8] = {1 , 2 , 3 , 4 ,
                  8 , 7 , 6 , 5};
  Node begin(toBinary(board));
  char buf[100];
  scanf("%d", &n);
  while (n != -1) {
  	int x = toBinary(board);
    scanf("%s", buf);
    string ops(buf);
    for (int i = 0; i < ops.length(); ++i)
    	x = OP[ops[i] - 'A'](x);
   	printf("%d\n", ops.length());
    print(x);
    scanf("%d", &n);
  }

  return 0;
}