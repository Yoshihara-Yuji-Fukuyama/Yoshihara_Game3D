#ifndef CENEMYAI_H
#define CENEMYAI_H

class CEnemyAi
{
	//AIÌXe[g
public:
	enum class EAiState
	{
		EWAIT,      //Ò@
		EWANDERING, //pj
		ECHASE,     //ÇÕ
		EATTACK,    //U
		EMOVEATTACK,//Ú®µÈªçU
		ERELOAD,    //[h
		EESCAPE,    //¦S
		EDAMAGE,    //íe
		EDIE,       //S
	};
	//úÝè
	void Init();
	//Xe[gÌÏX
	void ChangeState(EAiState aiState);
protected:
	EAiState mAiState;
};


#endif