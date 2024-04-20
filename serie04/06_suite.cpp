// ex 06

#include <iostream>
#include <cmath>

int main ()
{
using namespace std;
    cout << " \n06. Write a program that calculates the first 10 terms  of the sequence U_n  \nsuch that: U_0 = 1, U{n+1} = U_n/(n+1) \n" ;
    
    double u0(1);
    double uN(u0);  // uN = u0

    for (int n(1); n<=10; ++n)
    {
        u0=u0/n;
        uN=uN+u0; 
        
        cout <<  " \nU_" << n << " = " << u0 
        << "      V"<< n << " = " << uN << endl;
    }
    // à paufiner prochainement
    {
        cout << endl << " \nLa suite converge vers " << uN ;

    }

}
