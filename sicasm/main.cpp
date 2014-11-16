#include "../sicasm/sicasm.h"
#include "../sicasm/cObjectFile.h"

INT main(INT argc, CHAR* argv[])
{
    cObjectFile list("D:\\College\\systems\\labs\\lab2a.sicasm");
    list.print_listfile();
    system("PAUSE");
    return 0;
}