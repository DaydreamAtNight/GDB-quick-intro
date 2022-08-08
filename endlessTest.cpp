#include <iostream>
using namespace std;

void nothingTest()
{

}

void iTest()
{
    int i = 0;
    i++;
}

int main()
{
    for(;;)
    {
        nothingTest();
        iTest();
    }
}