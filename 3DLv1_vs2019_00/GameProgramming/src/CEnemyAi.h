#ifndef CENEMYAI_H
#define CENEMYAI_H

class CEnemyAi
{
	//TODO:AI�̍쐬
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
protected:
	EAiState mAiState;
};


#endif