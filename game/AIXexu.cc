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
            escanejar_mapa(map);
            dirs = vector<Dir>(nb_ghosts() + 1);
            for (int i=0; i <= nb_ghosts(); ++i) dirs[i] = rand_dir(None);
        }
        // move pacman
        cout << "!!!!!!!!!! RONDA " << round() << " !!!!!!!!!!!" <<endl;
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
        escanejar_mapa(map);
    }

    void escanejar_mapa(vector<vector<CType> >&map){
        for(int i = 0; i < files; ++i){
            for(int j = 0; j < columnes; ++j) map[i][j] = cell(i,j).type;
        }
    }

/*  Escriure mapa, funcio x comprovar que llegim be el mapa
  void escriure_mapa(vector<vector<CType> >&map){
        for(int i = 0; i < files; ++i){
            for(int j = 0; j < columnes; ++j) cout << map[i][j];
            cout << endl;
        }
    }*/

    Dir go(Pos& p){  //Devuelve la posicion a la que nos movemos
        Dir x;
        int dis = 0;
        bool b = false;
        vector<vector<bool> > vis(files,vector<bool>(columnes, false));
        x = bfs_rec (b, map, p, dis, vis);
        if(x == None){
            if(pac_can_move (p, Top)) x = Top;
            if(pac_can_move (p, Bottom)) x = Bottom;
            if(pac_can_move (p, Left)) x = Left;
            if(pac_can_move (p, Right)) x = Right;
        }
        cout << "resultado final: "<< x << endl;
        return x;

    }
    

    Dir bfs_rec(bool& b, vector<vector<CType> > matrix, Pos& p, int& dis, vector<vector<bool> >& vis) {
        if (p.i >= 0 and p.j >= 0 and p.i <= files and p.j <= columnes and matrix[p.i][p.j] != Wall and matrix[p.i][p.j] != Gate and not vis[p.i][p.j]) {
            int disl, disr, disb, dist;
            disl = disr = disb = dist = dis;
            Pos posl, posr, posb, post;
            bool bl, br, bb, bt;
            bl = br = bb = bt = false;
            vis[p.i][p.j] = true;
            if(matrix[p.i][p.j] == Dot or matrix[p.i][p.j] == Pill or matrix[p.i][p.j] == Bonus) b = true;
            else{
                disl = disr = disb = dist = dis+1;
                posb = posl = posr = post = p;
                posl.j -= 1;
                posr.j += 1;
                post.i -= 1;
                posb.i += 1;
                bl = br = bb = bt = false;
                bfs_rec(bb, matrix, posb, disb, vis);
                bfs_rec(bt, matrix, post, dist, vis);
                bfs_rec(br, matrix, posr, disr, vis);
                bfs_rec(bl, matrix, posl, disl, vis);
                cout << "booleans" << "bb: " << bb  << " bt: " << bt << " br: " << br << " bl: " << bl << endl;
                return min_aux(bt,bb,bl,br,dist,disb,disl,disr);
            }
            return None;
        }
        return None;
    }
    
    Dir min_aux(bool b1, bool b2, bool b3, bool b4, int dis1, int dis2, int dis3, int dis4){
        int minimo1, minimo2;
        minimo1 = minimo2 = 0;
        cout << "dis1: " << dis1 << " dis2: " << dis2 << " dis3: " << dis3 << " dis4: " << dis4 << endl;
        if(b1){
            if(b2){
                if(b3){
                    if(b4){
                        minimo1 = min(dis1,dis2);
                        minimo2 = min(dis3,dis4);
                        minimo1 = min(minimo1,minimo2);
                        if(minimo1 == dis1) return Top;
                        else if(minimo1 == dis2) return Bottom;
                        else if(minimo1 == dis3) return Left;
                        else return Right;
                    }
                    else {
                        minimo1 = min(dis1,dis2);
                        minimo1 = min(minimo1,dis3);
                        if(minimo1 == dis1) return Top;
                        else if(minimo1 == dis2) return Bottom;
                        else return Left;
                    }
                }
                else{
                    minimo1 = min(dis1,dis2);
                    if(minimo1 == dis1) return Top;
                    else if(minimo1 == dis2) return Bottom;
                }
            }
            else return Top; 
        }
        else if(b2){
            if(b3){
                if(b4){
                    minimo2 = min(dis3,dis4);
                    minimo1 = min(dis2,minimo2);
                    if(minimo1 == dis2) return Bottom;
                    else if(minimo1 == dis3) return Left;
                    else return Right;
                }
                else {
                    minimo1 = min(dis2,dis3);
                    if(minimo1 == dis2) return Bottom;
                    else if(minimo1 == dis3) return Left;
                }
            }
            else return Bottom;
        }
        else if(b3){
            if(b4){
                minimo1 = min(dis3,dis4);
                if(minimo1 == dis3) return Left;
                else return Right;
            }
            else return Left;
        }
        else if(b4) return Right;
        return None;
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


