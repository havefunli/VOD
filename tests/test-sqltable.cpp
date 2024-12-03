#include "../include/SQLTable.h"


int main()
{
    MYSQL* conn = SqlTable::SQLConnect();
    SqlTable::SQLDestroy(conn);
    return 0;
}