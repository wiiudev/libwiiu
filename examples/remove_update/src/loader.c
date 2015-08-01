#include "loader.h"

void _start()
{
	unsigned int nn_ccr_handle, coreinit_handle;
	OSDynLoad_Acquire("nn_ccr.rpl", &nn_ccr_handle);
	OSDynLoad_Acquire("coreinit.rpl", &coreinit_handle);
	void(*CCRSysDRCFWUpdateForward)();
	void(*_Exit)();
	OSDynLoad_FindExport(nn_ccr_handle, 0, "CCRSysDRCFWUpdateForward", &CCRSysDRCFWUpdateForward);
	OSDynLoad_FindExport(coreinit_handle, 0, "_Exit", &_Exit);
	CCRSysDRCFWUpdateForward();
	_Exit();
}