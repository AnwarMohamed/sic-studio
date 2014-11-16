#include "../sicasm/sicasm.h"
#include "../sicasm/cObjectFile.h"

INT main(INT argc, CHAR* argv[])
{
    //cObjectFile list("C:\\Users\\Anwar Mohamed\\Desktop\\SICXE Simulator\\systems\\Labs\\SICBOOT.ASM");
    //cListFile list("C:\\Users\\Anwar Mohamed\\Desktop\\SICXE Simulator\\systems\\Labs\\SICBOOT.ASM");
    cListFile list("D:\\College\\systems\\labs\\quiz1.sicasm");
    list.print_listfile();
    system("PAUSE");
    return 0;
}