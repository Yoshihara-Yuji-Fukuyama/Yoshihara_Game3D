#ifndef CENEMYAI_H
#define CENEMYAI_H

class CEnemyAi
{
	//AI�̃X�e�[�g
public:
	enum class EAiState
	{
		EWAIT,      //�ҋ@
		EWANDERING, //�p�j
		ECHASE,     //�ǐ�
		EATTACK,    //�U��
		EMOVEATTACK,//�ړ����Ȃ���U��
		ERELOAD,    //�����[�h
		EESCAPE,    //���S
		EDAMAGE,    //��e
		EDIE,       //���S
	};
	//�����ݒ�
	void Init();
	//�X�e�[�g�̕ύX
	void ChangeState(EAiState aiState);
protected:
	EAiState mAiState;
};


#endif