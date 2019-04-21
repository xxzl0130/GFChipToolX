#include <iostream>
#include <string>
#include "split.h"
#include "GFChip.h"
using namespace std;

int main()
{
    string str;
    while((bool)(cin >> str))
    {
        cout << GFChip::createFromSaveCode(str).toExcelLine() << endl;
    }
}