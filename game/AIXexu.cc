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
        if(s == "pacman") next = bfs (p, found, t);
        else next = ghost_bfs (p, found);
        return next;
    }
    
    Dir bfs (const Pos& pac, bool& found, CType s) {
        int n = rows();
        int m = cols();
        Dir siguiente = None;
        vector<vector<int> > leng(n, vector<int> (m, -1));
        queue<Pos> q;
        q.push(pac);
        leng[pac.i][pac.j] = 5;
        Pos fin;
        fin.i = -1;
        fin.j = -1;

        Dir d[4] = {Top, Bottom, Left, Right};

        while (not q.empty() and not found) {
            Pos p = q.front();
            q.pop();
            for (int i = 0; i < 4 and not found; ++i) {
                Pos destino = dest(p, d[i]);
                if (pos_ok(destino)) {
                    CType c = cell(destino.i,destino.j).type;
                    int id = cell(destino.i,destino.j).id;

                    if (c != Wall and c != Gate and leng[destino.i][destino.j] == -1  and not comprovapos(destino.i,destino.j)) {
                        if (c == s and not found) {
                            found = true;
                            fin = destino;
                        }
                        leng[destino.i][destino.j] = i;
                        q.push(destino);
                    }
                }
            }
        }
        siguiente = way(leng, found, fin);
        return siguiente;
    }


    Dir ghost_bfs (const Pos& gh, bool& found) {     /* BFS GHOST per buscar el pacman més proper */
        /* Initialitzations */
        int n = rows();
        int m = cols();
        Dir siguiente = None;
        vector<vector<int> > leng(n, vector<int> (m, -1));
        queue<Pos> q;
        q.push(gh);
        leng[gh.i][gh.j] = 5;
        Pos fin;
        fin.i = -1;
        fin.j = -1;

        /* Top,Bottom,Left,Right */
        Dir d[4] = {Top, Bottom, Left, Right};

        while (not q.empty() and not found) {
            Pos p = q.front();
            q.pop();
            for (int i = 0; i < 4 and not found; ++i) {
               Pos destino = dest(p, d[i]);
                if (pos_ok(destino)) {
                    int id = cell(destino.i, destino.j).id;
                    CType c = cell(destino.i, destino.j).type;
                    if (c != Wall and leng[destino.i][destino.j] == -1) {
                        if (id != -1){
                            if(robot(id).type == PacMan and  (pacman(me()).pos != destino) ) {
                                found = true;
                                fin = destino;
                            }
                        }
                        leng[destino.i][destino.j] = i;
                        q.push(destino);
                    }
                }
            }
        }
        siguiente = way(leng, found, fin);
        return siguiente;
    }

    Dir way (const vector<vector<int> >& leng, const bool& found, const Pos& fin){        /* Troba el cami pel que ha d'anar */
        Dir siguiente = None;
        if(fin.i != -1 and fin.j != -1){
            int row = fin.i;
            int col  = fin.j;
            while(leng[row][col] != 5 and found){
                int sig = leng[row][col];
                tractar_pos(row, col, sig);

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

    void tractar_pos(int& row, int& col, const int& i){
        if(i == 0) ++row;
        else if(i == 1) --row;
        else if(i == 2) ++col;
        else if(i == 3) --col;

        if(row == -1) row = rows()-1;
        else if(row == rows()) row = 0;
        if(col == -1) col = cols()-1;
        else if (col == cols()) col = 0;

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