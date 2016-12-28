#include <windows.h>

DWORD GetFileSize(char* filepath)
{
  DWORD size = 0, tmp;
  HANDLE hFile = CreateFile(filepath,
                            GENERIC_READ,
                            FILE_SHARE_READ,
                            NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL,
                            NULL);

  if(hFile != INVALID_HANDLE_VALUE)
  {
    size = GetFileSize(hFile, &tmp);
    CloseHandle(hFile);
  }
  return size;
}
 