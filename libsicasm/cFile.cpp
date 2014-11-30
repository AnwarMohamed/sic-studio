#include "cFile.h"

cFile::cFile(char* Filename)
{
    IsReassembled = FALSE;
    Attributes = GetFileAttributes(Filename);
    hFile = CreateFileA(Filename,
                        GENERIC_READ,
                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        0);
    
    if (hFile == INVALID_HANDLE_VALUE)
    {
        BaseAddress = NULL;
        FileLength = NULL;
        return;
    }
    
    hMapping = CreateFileMappingW(hFile,
                                  NULL,
                                  PAGE_READONLY,
                                  0,
                                  0,
                                  NULL);
    if (hMapping == 0)
    {
        CloseHandle(hFile);
        BaseAddress = NULL;
        FileLength = NULL;
        return;
    }

    BaseAddress = (unsigned long) MapViewOfFile(hMapping,
                                                FILE_MAP_READ,
                                                0,
                                                0,
                                                0);
    if (hMapping == 0)
    {
        UnmapViewOfFile(hMapping);
        CloseHandle(hFile);
        BaseAddress = NULL;
        FileLength = NULL;
        return;
    }
    
    FileLength  = (DWORD) GetFileSize(hFile,NULL);
    this->Filename = new CHAR[strlen(Filename)+1];
    strcpy_s(this->Filename, strlen(Filename)+1, Filename);

    IsFile = TRUE;
    return;
}

cFile::cFile(char* Buffer, DWORD Size)
{
    BaseAddress = (DWORD)Buffer;
    FileLength = Size;
    Attributes = NULL;
    Filename = NULL;
    IsFile = FALSE;
    IsReassembled = TRUE;
}

cFile::~cFile()
{
    if (BaseAddress != NULL && IsFile)
    {
        UnmapViewOfFile((LPVOID)BaseAddress);
        CloseHandle(hMapping);
        CloseHandle(hFile);
    }

    if (!IsFile && IsReassembled && BaseAddress != NULL)
    {
        try {
            //delete (DWORD*)BaseAddress;
        }
        catch (int){
            //free((DWORD*)BaseAddress);
        }
    }
}