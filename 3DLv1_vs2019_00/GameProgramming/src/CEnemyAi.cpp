#include "CEnemyAi.h"

void CEnemyAi::Init()
{
	//待機に設定
	mAiState = EAiState::EWAIT;
}