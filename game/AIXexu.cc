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
    int files;
    int columnes;
    vector<vector<CType> > map;

    /**
     * Play method.
     *
     * This method will be invoked once per each round.
     * You have to read the board here to place your actions
     * for this round.
     *
     * In this example, each robot selects a random direction
     * and follows it as long as it does not hit a wall.
     */
    virtual void play () {

        // initialization
        if (round() == 0) {
            files = rows();
            columnes = cols();
            map = vector<vector<CType> >(files, vector<CType>(columnes));
            /*escanejar_mapa(map);
            escriure_mapa(map);
            */
            dirs = vector<Dir>(nb_ghosts() + 1);
            for (int i=0; i <= nb_ghosts(); ++i) dirs[i] = rand_dir(None);
        }
        // move pacman
        //cout << "!!!!!!!!!! RONDA " << round() << " !!!!!!!!!!!" <<endl;
        /*escanejar_mapa(map);
        escriure_mapa(map);*/
        Pos p = pacman(me()).pos;   // retorna la posicio del pacman
        Dir mov = None;
        mov = go(p);
        move_my_pacman(mov);
        // move ghosts
        for (int i=0; i < nb_ghosts(); ++i) {
            Pos q = ghost(me(), i).pos;
            if (not ghost_can_move(q, dirs[i+1]) or rand()%1000==0) dirs[i+1] = rand_dir( dirs[i+1]);
            move_my_ghost(i, dirs[i+1]);
        }
        //escanejar_mapa(map);
    }

    void escanejar_mapa(vector<vector<CType> >&map){
        for(int i = 0; i < files; ++i){
            for(int j = 0; j < columnes; ++j) map[i][j] = cell(i,j).type;
        }
    }

  //Escriure mapa, funcio x comprovar que llegim be el mapa
  void escriure_mapa(vector<vector<CType> >&map){
        for(int i = 0; i < files; ++i){
            for(int j = 0; j < columnes; ++j) cout << map[i][j];
            cout << endl;
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
                            if ((c == Pill or c == Bonus or c == Hammer or c == Mushroom) and not found) {
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
        /*for(int i = 0; i < n; ++i){
            for(int j = 0; j < m; ++j) cout << leng[i][j];
            cout << endl;
        }*/

        return siguiente;
    }

    
    inline bool pac_can_move (Pos p, Dir d) {       //RETORNA SI PODEM MOURE EL PACMAN
        CType t = cell(dest(p, d)).type;
        if (t == Wall or t == Gate) return false;
        if (pacman(me()).type == PacMan and cell(dest(p, d)).id != -1) return false;
        return true;
    }

    inline bool ghost_can_move (Pos p, Dir d) {
        CType t = cell(dest(p, d)).type;
        if (t == Wall) return false;        
        int r = cell(dest(p, d)).id;
        if (r == -1) return true;
        if (robot(r).type==Ghost) return false;
        return true;
    }

    Dir rand_dir (Dir notd) {
        Dir a[] = {Left, Right, Bottom, Top};
        while (true) {
            Dir d = a[rand() % 4];
            if (d != notd) return d;
        }
    }

};



/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);

