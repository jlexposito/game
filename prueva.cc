#include <iostream>
#include <vector>
using namespace std;

int min_aux(bool b1, bool b2, bool b3, bool b4, int dist1, int dist2, int dist3, int dist4){
    int minimo1, minimo2;
    minimo1 = minimo2 = 0;
    if(b1){
        if(b2){
            if(b3){
                if(b4){
                    minimo1 = min(dist1,dist2);
                    minimo2 = min(dist3,dist4);
                    minimo1 = min(minimo1,minimo2)
                }
                minimo1 = min(dist1,dist2);
                minimo1 = min(minimo1,dist3)
            }
            minimo1 = min(dist1,dist2);
        }
        minimo1 = dist1; 
    }
    return minimo1;
}

int main(){
    bool b1, b2, b3, b4;
    int dist1,dist2,dist3,dist4;
    cout << "b1: "; cin >> b1; cout << endl;
    cout << "b2: "; cin >> b2; cout << endl;
    cout << "b3: "; cin >> b3; cout << endl;
    cout << "b4: "; cin >> b4; cout << endl;

    cout << b1 << " " << b2 << " " << b3 << " " << b4 << endl;

    cout << "dist1: "; cin >> dist1; cout << endl;
    cout << "dist2: "; cin >> dist2; cout << endl;
    cout << "dist3: "; cin >> dist3; cout << endl;
    cout << "dist4: "; cin >> dist4; cout << endl;

    cout << dist1 << " " << dist2 << " " << dist3 << " " << dist4 << endl;

    cout << min_aux(b1,b2,b3,b4,dist1,dist2,dist3,dist4) << endl;

}
