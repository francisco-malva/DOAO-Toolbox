#include <windows.h>

#include "WidescreenFix.h"

#include "Globals.h"

DWORD* ScreenFlags = reinterpret_cast<DWORD*>(0x02cb9574);

void WidescreenFix::Update()
{
	*ScreenFlags = Globals::IsWidescreen;
}
