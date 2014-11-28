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
  int x, y, next;
  Node(int x_=0, int y_=0) {
    x = x_; y = y_; next = 0;
  }
};

int mark[HEIGHT][WIDTH];
int route[SIZE];
bool visited[HEIGHT][WIDTH];

int dx[] = {1, 2, 2, 1, -1, -2, -2, -1};
int dy[] = {2, 1, -1, -2, -2, -1, 1, 2};

bool cmp(Node a, Node b) {
  return a.next < b.next;
}

bool valid(Node node) {
  return (node.x >= 0 && node.y >= 0
          && node.x < HEIGHT && node.y < WIDTH
          && !visited[node.x][node.y]);
}

bool dfs(Node node, int step) {
  visited[node.x][node.y] = true;
  route[step] = mark[node.x][node.y];

  if (step == SIZE - 1) {
    for (int i = 0; i < SIZE; i++)
        printf("%d%c", route[i], i == SIZE - 1 ? '\n' : ' ');
    return true;
  } else {
    std::vector<Node> v;
    for (int i = 0; i < NEIGHBOR; i++) {
      Node cur(node.x + dx[i], node.y + dy[i]);
      if (valid(cur)) {
        for (int j = 0; j < NEIGHBOR; j++) {
          Node ahead(cur.x + dx[j], cur.y + dy[j]);
          if (valid(ahead))
            cur.next++;
        }
        v.push_back(cur);
      }
    }

    std::sort(v.begin(), v.end(), cmp);

    for (int i = 0; i < v.size(); i++) {
      visited[v[i].x][v[i].y] = true;
      if (dfs(v[i], step + 1))
        return true;
      visited[v[i].x][v[i].y] = false;
    }
    return false;
  }
}

int main(int argc, char *argv[]) {
#ifdef JDEBUG
  freopen("1153.in", "r", stdin);
  freopen("1153.out", "w", stdout);
#endif
  int count = 1, n = 0;

  for (int i = 0; i < HEIGHT; i++)
    for (int j = 0; j < WIDTH; j++)
      mark[i][j] = count++;

  while (scanf("%d", &n) && n != -1) {
    Node node;

    for (int i = 0; i < HEIGHT; i++)
      for (int j = 0; j < WIDTH; j++)
        if (mark[i][j] == n) {
          node = Node(i, j);
        }

    memset(visited, false, sizeof(visited));
    memset(route, -1, sizeof(route));

    dfs(node, 0);
  }

  return 0;
}
