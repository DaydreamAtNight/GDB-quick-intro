#include <iostream>
using namespace std;

int main()
{
    int * temp = NULL;
    *temp = 10;     // can't overwrite address of NULL as 10
    cout << *temp << endl;
    return 0;
}