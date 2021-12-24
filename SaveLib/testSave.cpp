#include "DataSave.h"
#include <stdio.h>

//-----------------------------------------------------------------------------

int main()
{
    Data data;

    int values[4] = {124, 25, 0, 0};

    data.Save("Val", values, 4);

    char name[10] = "Vlad";
    data.Save("Name", name, 10);
}

//-----------------------------------------------------------------------------

