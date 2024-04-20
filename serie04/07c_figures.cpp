#include <iostream>
#include <cmath>
using namespace std;

int main()

{
    int n(0);
    cout << "\nc) How to modify the program so that it displays a triangle?";
    cout << endl << "\nEntrez le nombre de lignes du triangle : ";
    
    cin >> n;
    cout << endl;
    {
        for (int a(0); a<n;++a)
        {
            for (int j(1); j<(n-a);++j) //nécessaire aux espaces avant le triangle
            {
                cout << " ";
            }
            for(int j(0); j < 2*a+1; ++j) //nécessaire pour afficher le bon nombre de caractère(s)
            {
                cout << "｡◕‿◕｡";
            }
        cout << endl;
        }
    }
}