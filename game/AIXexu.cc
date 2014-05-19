#include "Player.hh"

#include <cmath>

using namespace std;



/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME xexuv3



struct PLAYER_NAME : public Player {


    /**
     * Factory: returns a new instance of this class.
     * Do not modify this function.
     */
    static Player* factory () {
        return new PLAYER_NAME;
    }


    /**
     * Attributes for your player can be defined here.
     */
    vector<Dir> dirs;
    int powertime;

    /**
     * Play method.
     *
     * This method will be invoked once per each round.
     * You have to read the board here to place your actions
     * for this round.
     **/
    virtual void play () {

        /* << INITIALITZATION >> */
        if (round() == 0) {
            dirs = vector<Dir>(nb_ghosts() + 1);
            powertime = 0;
        }
        else{
            /* << MOVE PACMAN >> */
            string s = "pacman";
            Pos p = pacman(me()).pos;   // retorna la posicio del pacman
            CType sdot = Dot;
            CType spill = Pill;
            CType sham = Hammer;
            CType sbon = Bonus;
            CType smus = Mushroom;
            Dir mov_dot, mov_pill, mov_ham, mov_mus, mov_bon;
            mov_dot = mov_pill = mov_ham = mov_mus = mov_bon = None;
            mov_dot = go(p, sdot, s);
            mov_pill = go(p, spill, s);
            mov_ham = go(p, sham, s);
            mov_bon = go(p, sbon, s);
            mov_mus = go(p, smus, s);

            if((pacman(me()).type == PowerPacMan)){
                if(mov_ham != None) move_my_pacman(mov_ham);
                else if(mov_pill != None and powertime < (power_time()/2)) move_my_pacman(mov_pill);
                else if(mov_mus != None) move_my_pacman(mov_mus);
                else if(mov_bon != None) move_my_pacman(mov_bon);
                else move_my_pacman(mov_dot);
                ++powertime;
            }
            else if(mov_pill != None) move_my_pacman(mov_pill);
            else move_my_pacman(mov_dot);
            powertime = 0;

            /* << MOVE GHOSTS >> */
            s  = "ghost";
            for (int i=0; i < nb_ghosts(); ++i) {
                Pos q = ghost(me(), i).pos;
                Dir res = go(q, sdot, s);
                move_my_ghost(i, res);
            }
        }
    }

    Dir go(const Pos& p, const CType t, const string s){  /* Devuelve la posicion a la que nos movemos */
        Dir next = None;
        bool found = false;
        int x  = p.i;
        int y = p.j;
        if(s == "pacman") next = bfs (x, y, found, t);
        else next = ghost_bfs (x, y, found);
        return next;
    }
    
    Dir bfs (int x, int y, bool& found, CType s) {
        int n = rows();
        int m = cols();
        Dir siguiente = None;
        vector<vector<int> > leng(n, vector<int> (m, -1));
        queue<pair<int, int> > q;
        q.push(make_pair(x, y));
        leng[x][y] = 5;
        pair<int,int> fin;
        fin.first = -1;
        fin.second = -1;

        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        while (not q.empty() and not found) {
            pair<int, int> p = q.front();
            q.pop();
            for (int i = 0; i < 4 and not found; ++i) {
                int xx = p.first + dx[i];
                int yy = p.second + dy[i];
                if (xx >= 0 and xx < n) {
                    if (yy >= 0 and yy < m) {
                        CType c = cell(xx,yy).type;
                        int id = cell(xx,yy).id;

                        if (c != Wall and c != Gate and leng[xx][yy] == -1  and not comprovapos(xx,yy)) {
                            if (c == s and not found) {
                                found = true;
                                fin = make_pair(xx,yy);
                            }
                            leng[xx][yy] = i;
                            q.push(make_pair(xx, yy));
                        }
                    }
                }
            }
        }
        siguiente = way(leng, found, fin);
        return siguiente;
    }


    Dir ghost_bfs (int x, int y, bool& found) {     /* BFS GHOST per buscar el pacman més proper */
        /* Initialitzations */
        int n = rows();
        int m = cols();
        Dir siguiente = None;
        vector<vector<int> > leng(n, vector<int> (m, -1));
        queue<pair<int, int> > q;
        q.push(make_pair(x, y));
        leng[x][y] = 5;
        pair<int,int> fin;
        fin.first = -1;
        fin.second = -1;

        /* Top,Bottom,Left,Right */
        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        while (not q.empty() and not found) {
            pair<int, int> p = q.front();
            q.pop();
            for (int i = 0; i < 4 and not found; ++i) {
                int xx = p.first + dx[i];
                int yy = p.second + dy[i];
                if (xx >= 0 and xx < n) {
                    if (yy >= 0 and yy < m) {
                        int id = cell(xx,yy).id;
                        CType c = cell(xx,yy).type;
                        if (c != Wall and leng[xx][yy] == -1) {
                            if (id != -1){
                                if(robot(id).type == PacMan and  (pacman(me()).pos.i != xx and pacman(me()).pos.j != yy) ) {
                                    found = true;
                                    fin = make_pair(xx,yy);
                                }
                            }
                            leng[xx][yy] = i;
                            q.push(make_pair(xx, yy));
                        }
                    }
                }
            }
        }
        siguiente = way(leng, found, fin);
        return siguiente;
    }

    Dir way (const vector<vector<int> >& leng, const bool& found, const pair<int,int>& fin){        /* Troba el cami pel que ha d'anar */
        Dir siguiente = None;
        if(fin.first != -1 and fin.first != -1){
            int row = fin.first;
            int col  = fin.second;

            while(leng[row][col] != 5 and found){
                int sig = leng[row][col];
                if(sig == 0) ++row;
                else if(sig == 1) --row;
                else if(sig == 2) ++col;
                else if(sig == 3) --col;

                if(leng[row][col] == 5){
                    if(sig == 0) siguiente = Top;
                    else if(sig == 1) siguiente = Bottom;
                    else if(sig == 2) siguiente = Left;
                    else if(sig == 3) siguiente = Right;
                }
            }
        }
        return siguiente;
    }

    bool comprovapos(const int x, const int y){
        bool problem = false;
        Cell c = cell(x,y);
        if(c.id != -1){
            for(int i = 0; i < nb_ghosts() and not problem; ++i){   /* Comprovo si es un ghost meu */
                Pos q = ghost(me(), i).pos;
                if(q.i == x and q.j == y) return true;
            }
            if((pacman(me()).type == PowerPacMan)){
                if(robot(c.id).type == Ghost) return false;
                else if(robot(c.id).type == PacMan or robot(c.id).type == PowerPacMan) return true;
            }
            else return true;
        }
        return problem;
    }

    bool come(const int x, const int y){
        bool problem = false;
        Cell c = cell(x,y);
        if(c.id != -1){
           for(int i = 0; i < nb_ghosts() and not problem; ++i){
                Pos q = ghost(me(), i).pos;
                if(q.i == x and q.j == y) return true;
            }
            if(robot(c.id).type == Ghost) return true;
            else if(robot(c.id).type == PowerPacMan) return true;
        }
        return problem;
    }

    void print_m(vector<vector<int> > matrix){
        int n = matrix.size();
        int m = matrix[0].size();
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j) cout << matrix[i][j];
            cout << endl;
        }
    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

