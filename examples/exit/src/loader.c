#include "loader.h"

void _start()
{
    unsigned int coreinit_handle;
    OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
    void(*_Exit)();
    OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
    _Exit();
}