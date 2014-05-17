#include "Player.hh"

#include <cmath>

using namespace std;



/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME xexu



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
        }
        else{
            /* << MOVE PACMAN >> */
            Pos p = pacman(me()).pos;   // retorna la posicio del pacman
            Dir mov = None;
            mov = go(p);
            move_my_pacman(mov);

            /* << MOVE GHOSTS >> */
            for (int i=0; i < nb_ghosts(); ++i) {
                Pos q = ghost(me(), i).pos;
                cout << "Anem a entrar al go"<<endl;
                Dir res = ghost_go(q);
                cout << "<< Hem acabat el go"<<endl;
                move_my_ghost(i, res);
            }
        }
    }

    Dir go(Pos& p){  //Devuelve la posicion a la que nos movemos
        Dir next = None;
        bool found = false;
        int x  = p.i;
        int y = p.j;
        next = bfs (x, y, found);
        //cout << "FINAL: "<< next << endl;
        //if(pacman(me()).type == PowerPacMan) cout << "SUPERPODERES :] !!!" << endl;
        return next;

    }
    
    Dir bfs (int x, int y, bool& found) {
        int n = rows();
        int m = cols();
        Dir siguiente = None;
        vector<vector<int> > leng(n, vector<int> (m, -1));
        queue<pair<int, int> > q;
        q.push(make_pair(x, y));
        leng[x][y] = 5;
        //cout << "INITIAL POSITION: " << x << " " << y << endl;
        //cout << leng[x][y] << endl;
        pair<int,int> fin;

        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        while (not q.empty() and not found) {
            pair<int, int> p = q.front();
            q.pop();
            //cout << "Posicion inicial: " << p.first << ", " << p.second <<endl;
            for (int i = 0; i < 4 and not found; ++i) {
                int xx = p.first + dx[i];
                int yy = p.second + dy[i];
                if (xx >= 0 and xx < n) {
                    if (yy >= 0 and yy < m) {
                        CType c = cell(xx,yy).type;
                        if (c != Wall and c != Gate and leng[xx][yy] == -1) {
                            //cout << "Posicion: " << xx << "," << yy << " i: " << i << " casilla: " << c << endl;
                            if ((c == Pill or c == Bonus or c == Hammer or c == Mushroom or c == Dot) and not found) {
                                //cout << "encontrado" << endl;
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
        int row = fin.first;
        int col  = fin.second;
        while(leng[row][col] != 5){
            int sig = leng[row][col];
            //cout << sig << endl;
            if(sig == 0) ++row;
            else if(sig == 1) --row;
            else if(sig == 2) ++col;
            else if(sig == 3) --col;
            if(leng[row][col] == 5){
                //cout << "hemos llegado al inicio siguiente: " << sig << endl;
                if(sig == 0) siguiente = Top;
                else if(sig == 1) siguiente = Bottom;
                else if(sig == 2) siguiente = Left;
                else if(sig == 3) siguiente = Right;
                //cout << siguiente << endl;
            }
        }
        return siguiente;
    }

    Dir ghost_go(Pos& p){
        Dir next = None;
        bool found = false;
        int x  = p.i;
        int y = p.j;
        next = ghost_bfs (x, y, found);
        return next;
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

        /* Top,Bottom,Left,Right */
        int dx[4] = {-1, 1, 0, 0};
        int dy[4] = {0, 0, -1, 1};

        while (not q.empty() and not found) {
            pair<int, int> p = q.front();
            q.pop();
            //cout << "Posicion inicial: " << p.first << ", " << p.second <<endl;
            for (int i = 0; i < 4 and not found; ++i) {
                int xx = p.first + dx[i];
                int yy = p.second + dy[i];
                if (xx >= 0 and xx < n) {
                    if (yy >= 0 and yy < m) {
                        int id = cell(xx,yy).id;
                        CType c = cell(xx,yy).type;
                        if (c != Wall and leng[xx][yy] == -1) {
                            //cout << "Posicion: " << xx << "," << yy << " i: " << i << " casilla: " << c << endl;
                            if (id != -1){
                                if(robot(id).type == PacMan and  (pacman(me()).pos.i != xx and pacman(me()).pos.j != yy) ) {
                                    cout << "El fantasma esta en: " << x << " " << y << " ";
                                    cout << "Hemos encontrado un robot: " << xx << " " << yy <<endl;
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
        int row = fin.first;
        int col  = fin.second;
        /*cout << leng[row][col];
        for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j) cout << leng[i][j] << " ";
            cout <<endl;
        }
        cout << endl << endl;
        cout << "Tot retornat" << endl;*/
        while(leng[row][col] != 5 and found){
            //cout << "trobant cami" << endl;
            int sig = leng[row][col];
            //cout << sig << endl;
            if(sig == 0) ++row;
            else if(sig == 1) --row;
            else if(sig == 2) ++col;
            else if(sig == 3) --col;
            if(leng[row][col] == 5){
                //cout << "hemos llegado al inicio siguiente: " << sig << endl;
                if(sig == 0) siguiente = Top;
                else if(sig == 1) siguiente = Bottom;
                else if(sig == 2) siguiente = Left;
                else if(sig == 3) siguiente = Right;
                //cout << siguiente << endl;
            }
        }
        return siguiente;
    }

    inline bool ghost_can_move (Pos p, Dir d) {
        CType t = cell(dest(p, d)).type;
        if (t == Wall) return false;        
        int r = cell(dest(p, d)).id;
        if (r == -1) return true;
        if (robot(r).type==Ghost) return false;
        return true;
    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

