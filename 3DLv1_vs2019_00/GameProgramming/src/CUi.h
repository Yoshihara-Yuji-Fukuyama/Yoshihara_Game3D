#pragma once
#include "CFont.h"
/*
* CUi
* ユーザーインタフェースクラス
*/
class CUi
{
public:
	void PosY(float f);//高度の設定
	void RotX(float f);//ピッチの設定
	void RotY(float f);//Y軸の回転値
	//ゲームクリア表示
	void Clear();
	//ゲームオーバー表示
	void Over();
	//スタート表示
	void Start();
	//Enemy(敵の数)
	void Enemy(int enemy);
	CUi();
	void Hp(int hp);
	//SetTime(時間)
	void SetTime(int time);
	void Render();
private:
	float mPosY;//高度　Y座標
	float mRotX;//ピッチ　X軸回転値
	float mRotY;//Y軸回転値
	int mEnemy;	//敵の数
	int mHp;
	int mTime;	//経過時間
	CFont mFont;
};
