#include <iostream>
#include <vector>
using namespace std;

int bfs (const vector<vector<char> >& map, int x, int y, bool& found) {
    /* Initialitzations */
    int n = map.size();
    int m = map[0].size();

    vector<vector<int> > leng(n, vector<int> (m, -1));
    queue<pair<int, int> > q;
    q.push(make_pair(x, y));
    leng[x][y] = 0;
    int len;

    /* Top,Bottom,Left,Right */
    int dx[4] = {-1, 1, 0, 0};
    int dy[4] = {0, 0, -1, 1};

    while (not q.empty() and not found) {
    pair<int, int> p = q.front();
    q.pop();
    for (int i = 0; i < 4; ++i) {
      int xx = p.first + dx[i];
     int yy = p.second + dy[i];
   
    if (xx >= 0 and xx < n) {
        if (yy >= 0 and yy < m) {
            if (map[xx][yy] != 'X' and leng[xx][yy] == -1) {
              if (map[xx][yy] == 't') found = true;
                leng[xx][yy] = leng[p.first][p.second] + 1;
                len = leng[xx][yy];
                q.push(make_pair(xx, yy));
              }
            }
        }
      }
    }
  return len;
}

int main(){
    int n, m, f, c;
    int t = 0;
    cin >> n >> m;
    vector<vector<char> > map(n+1, vector<char>(m+1));
    vector<vector<bool> > visited(n+1, vector<bool>(m+1,false));
    for(int i = 1; i <= n; ++i)
        for(int j = 1; j <= m; ++j)
            cin >> map[i][j];

    cin >> f >> c;
    bool found = false;
    bfs(map, found);
    cout << t << endl;
}