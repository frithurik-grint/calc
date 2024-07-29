#include "calc/base/stddef.h"

// +---- System Dependent Definitions --------------------------+

#pragma region System Dependent Definitions

#ifdef _WIN32

#include <windows.h>

unsigned int _CDECL _getpagesiz()
{
    static SYSTEM_INFO sys_info = { 0 };

    if (!sys_info.dwPageSize)
        GetSystemInfo(&sys_info);

    return sys_info.dwPageSize;
}

#endif // _WIN32

#pragma endregion

// +---- System Dependent Definitions -- End -------------------+
