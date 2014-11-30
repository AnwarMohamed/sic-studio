#include "../libsicasm/sicasm.h"
#include "../libsicasm/cObjectFile.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"kernel32.lib")

void send_command(char *s) {
	int i = mciSendString(s, NULL, 0, 0);
	if (i) {
		fprintf(stderr, "Error %d when sending %s\n", i, s);
	}
}

int main(int argc, char* argv[])
{

	char shortBuffer[MAX_PATH];
	char cmdBuff[MAX_PATH + 64];

	GetShortPathName("mario.mp3", shortBuffer, sizeof(shortBuffer));
	
	if (!*shortBuffer) {
		fprintf(stderr, "Cannot shorten filename \"%s\"\n", "mario.mp3");
	}
	else {
		send_command("Close All");
		sprintf_s(cmdBuff, "Open %s Type MPEGVideo Alias sicMP3", shortBuffer);
		send_command(cmdBuff);
		send_command("Play sicMP3");
	}

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

	printf("\n");
	system("PAUSE");
	printf("\n[+] Exiting\n");
	send_command("Close All");
	return 0;
}