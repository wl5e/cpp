#include <iostream>
#include <cmath>

using namespace std;
int main()
{

    int n;              // le nombre à tester
    bool premier(true); // résultat du test de primal
    int diviseur(1);

    cout << "1. Vérifier si le nombre n est pair (si oui, il n'est pas premier sauf si c'est 2).\n" << endl << "2. Pour tous les nombres impairs inférieurs ou égaux à la racine carrée de n,\nvérifier s'ils divisent n.\n" << endl << "Si ce n'est pas le cas, alors n est premier."<< endl;

    // Saisie du nombre à tester
    do {
        cout << "\nEntrez un nombre entier > 1 : ";
        cin >> n;
    }   while (n <= 1);
    
    if (n%2==0)  {
        // le nombre est pair
        if (n != 2 ) {
        premier = false;
        diviseur = 2;
        }

    // ça joue ! Mais oui c'est clair !
    } else {
        const double borne_max(sqrt(n));
        for (int i=3; premier && i<= borne_max; i += 2)
            if (n%i == 0) {
                premier = false;
                diviseur = i;
            }
    }
    cout << endl << n;
    if (premier) 
    { 
        cout << " est un nombre premier" << endl;
    } else {
        cout << " n'est pas pas premier, car il est divisible par : " 
        << diviseur << endl;
    }
}     
