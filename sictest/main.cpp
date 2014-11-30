#include "../libsicasm/sicasm.h"
#include "../libsicasm/cObjectFile.h"

INT main(INT argc, CHAR* argv[])
{
    //cObjectFile list("C:\\sicxe\\SICBOOT.ASM");
    //cObjectFile list("C:\\sicxe\\SICLDR.ASM");
    cObjectFile list("D:\\College\\systems\\labs\\quiz1.sicasm");

    FILE* file;
    fopen_s(&file, "LISTFILE", "w");
    list.print_listfile(file);
    list.print_listfile();
    fclose(file);

	fopen_s(&file, "OBJFILE", "w");
    list.print_objectfile(file);
    //list.print_objectfile();
    fclose(file);

    system("PAUSE");
    return 0;
}