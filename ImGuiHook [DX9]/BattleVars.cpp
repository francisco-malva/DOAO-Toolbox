#include "BattleVars.h"

unsigned char* BattleVars::GameStatePtr = reinterpret_cast<unsigned char*>(0x02d584a0);

bool BattleVars::InBattle()
{
	return *GameStatePtr == 2;
}
