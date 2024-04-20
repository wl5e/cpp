#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    double i(0);
    double j(0);
    double a(0); // variable temporaire

    cout << endl << "Attention ... je vais vous faire un tour de passe-passe ! Pour ceci, il nous faut deux variables (i & j)" << endl << "i= ";
    cin >> i;
    cout << endl << "Bien ! Maintenant la deuxième ... meh eh eh. " << endl << "j=";
    cin >> j;
    cout << endl << "Vous avez mentionné les valeurs : i = " << i << " et j = " << j << " dans cet ordre !" << endl;
    a = i; // nécéssaire à la permutation des variables
    i = j;
    j = a;
    cout << endl << "Désormais ... je les ai inversé : i = " << i << " et j = " << j << " \nUn autre ordre !!! Je pète un câble O_O" << endl;
}