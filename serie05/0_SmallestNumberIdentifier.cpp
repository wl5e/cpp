#include <iostream>
#include <cmath>
using namespace std;

int main()
{
   double n1(0);
   double n2(0);
   double n3(0);

    cout << "\nEntrez vos chiffres" << endl << "1er chiffre : ";
    cin >> n1;
    cout << "\n2ème chiffre : ";
    cin >> n2;
    cout << "\n3ème chiffre : ";
    cin >> n3; 
    cout << endl << "Parmi la liste ( " << n1 << " " << n2 << " " << n3 << ")" << endl << endl;
   
   if (n1<=n2) 
   {
       if (n1<=n3)
       {
           cout << "Le plus petit chiffre de la liste est : "<< n1 << endl;
           }
        else
        {
            cout << "Le plus petit chiffre de la liste est : "<< n3 << endl;
        }    
   }else
        {
        if (n2<=n1)
        {
            cout << "Le plus petit chiffre de la liste est : "<< n2 << endl;
        }
        else
        {
            cout << "Le plus petit chiffre de la liste est : "<< n3 << endl;
        }
        }
}
