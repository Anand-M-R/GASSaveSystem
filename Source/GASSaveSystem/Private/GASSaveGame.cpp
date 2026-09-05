#include "GASSaveGame.h"

UGASSaveGame::UGASSaveGame()
{
	SaveSlotName = TEXT("GASSaveSlot");
	UserIndex = 0;
	SaveTimestamp = FDateTime::Now();
}
