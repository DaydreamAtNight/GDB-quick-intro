#include <iostream>
using namespace std;

void printHello()
{
    cout << "hello" << endl;
}

int main()
{
    int arr[] = {1,2,3,4,5};
    for(int i = 0; i < 5; i++)
    {
        cout << arr[i] << endl;
    }

    printHello();

    return 0;
}