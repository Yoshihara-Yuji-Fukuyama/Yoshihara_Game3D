#ifndef CENEMYAI_H
#define CENEMYAI_H

class CEnemyAi
{
	//AIのステート
public:
	enum class EAiState
	{
		EWAIT,      //待機
		EWANDERING, //徘徊
		ECHASE,     //追跡
		EATTACK,    //攻撃
		EMOVEATTACK,//移動しながら攻撃
		ERELOAD,    //リロード
		EESCAPE,    //逃亡
		EDAMAGE,    //被弾
		EDIE,       //死亡
	};
	//初期設定
	void Init();
	//ステートの変更
	void ChangeState(EAiState aiState);
protected:
	EAiState mAiState;
};


#endif