#include "../sicasm/sicasm.h"
#include "../sicasm/cObjectFile.h"

int main(int argc, char* argv[])
{
	printf(
		"\n       ___                       ___           ___           ___     \n"
		"      /  /\\        ___          /  /\\         /__/|         /  /\\    \n"
		"     /  /:/_      /  /\\        /  /:/        |  |:|        /  /:/_   \n"
		"    /  /:/ /\\    /  /:/       /  /:/         |  |:|       /  /:/ /\\  \n"
		"   /  /:/ /::\\  /__/::\\      /  /:/  ___   __|__|:|      /  /:/ /:/_ \n"
		"  /__/:/ /:/\\:\\ \\__\\/\\:\\__  /__/:/  /  /\\ /__/::::\\____ /__/:/ /:/ /\\\n"
		"  \\  \\:\\/:/~/:/    \\  \\:\\/\\ \\  \\:\\ /  /:/    ~\\~~\\::::/ \\  \\:\\/:/ /:/\n"
		"   \\  \\::/ /:/      \\__\\::/  \\  \\:\\  /:/      |~~|:|~~   \\  \\::/ /:/ \n"
		"    \\__\\/ /:/       /__/:/    \\  \\:\\/:/       |  |:|      \\  \\:\\/:/  \n"
		"      /__/:/        \\__\\/      \\  \\::/        |  |:|       \\  \\::/   \n"
		"      \\__\\/                     \\__\\/         |__|/         \\__\\/    \n\n\n"
		"[*] SIC/XE Assembler\n"
		"[*] By Anwar Mohamed ~ 2491\n"
		"[*] anwarelmakrahy@gmail.com\n\n"
		);

	cObjectFile* object_file;
	if (argc > 1) {
		object_file = new cObjectFile(argv[1]);
		printf("[+] Opening \'%s\'\n", argv[1]);
	}
	else {
		object_file = new cObjectFile("SRCFILE");
		printf("[+] Opening \'SRCFILE\'\n");
	}


	if (object_file->BaseAddress) {
		FILE* file;
		fopen_s(&file, "LISTFILE", "w");
		printf("[+] Generating List File\n");
		object_file->print_listfile(file);
		fclose(file);

		fopen_s(&file, "OBJFILE", "w");
		printf("[+] Generating Object Code\n");
		object_file->print_objectfile(file);
		fclose(file);

	}
	else {
		printf("[x] Error processing source file\n");
		printf("[x] Check if the specified file exists\n");
	}

	printf("\n[+] Exiting\n");

	system("PAUSE");
	return 0;
}