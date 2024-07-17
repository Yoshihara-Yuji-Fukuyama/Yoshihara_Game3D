#include "CEnemyAi.h"

//初期設定
void CEnemyAi::Init()
{
	//待機に設定
	mAiState = EAiState::EWAIT;
}

//ステートの変更
void CEnemyAi::ChangeState(EAiState aiState)
{
	mAiState = aiState;
}
