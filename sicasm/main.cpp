#include "../sicasm/sicasm.h"
#include "../sicasm/cObjectFile.h"

INT main(INT argc, CHAR* argv[])
{
    //cObjectFile list("C:\\Users\\Anwar Mohamed\\Desktop\\SICXE Simulator\\systems\\Labs\\SICBOOT.ASM");
    //cListFile list("C:\\Users\\Anwar Mohamed\\Desktop\\SICXE Simulator\\systems\\Labs\\SICLDR.ASM");
    cObjectFile list("D:\\College\\systems\\labs\\quiz1.sicasm");

    FILE* file;
    fopen_s(&file, "LISTFILE", "w");
    list.print_listfile(file);
    //list.print_listfile();
    fclose(file);

    fopen_s(&file, "OBJFILE", "w");
    list.print_objectfile(file);
    list.print_objectfile();
    fclose(file);

    system("PAUSE");
    return 0;
}