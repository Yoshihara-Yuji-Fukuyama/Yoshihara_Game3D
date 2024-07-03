#ifndef CENEMYAI_H
#define CENEMYAI_H

class CEnemyAi
{
	//TODO:AI‚Ìì¬
public:
	enum class EAiState
	{
		EWAIT,      //‘Ò‹@
		EWANDERING, //œpœj
		ECHASE,     //’ÇÕ
		EATTACK,    //UŒ‚
		EMOVEATTACK,//ˆÚ“®‚µ‚È‚ª‚çUŒ‚
		ERELOAD,    //ƒŠƒ[ƒh
		EESCAPE,    //“¦–S
		EDAMAGE,    //”í’e
		EDIE,       //€–S
	};
	//‰Šúİ’è
	void Init();
protected:
	EAiState mAiState;
};


#endif