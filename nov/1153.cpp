// 12330402 Qiuyi Zhang
#define JDEBUG
 
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