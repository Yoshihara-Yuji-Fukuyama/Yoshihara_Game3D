#include "CEnemyAi.h"

//�����ݒ�
void CEnemyAi::Init()
{
	//�ҋ@�ɐݒ�
	mAiState = EAiState::EWAIT;
}

//�X�e�[�g�̕ύX
void CEnemyAi::ChangeState(EAiState aiState)
{
	mAiState = aiState;
}
