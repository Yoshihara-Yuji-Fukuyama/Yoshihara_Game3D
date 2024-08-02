#ifndef UITEXTUER_H
#define UITEXTURE_H

#include "CCharacter.h"

#define TEXTURE_HP_BAR "HpBar.png"//HPバー画像
#define TEX_HP_MAX 0,625,0,91 //HP最大の画像内座標
#define TEX_HP_1 0,625,104,194//HP-1の画像内座標
#define TEX_HP_2 0,625,208,298//HP-2の画像内座標
#define TEX_HP_3 0,625,312,402//HP-3の画像内座標
#define TEX_HP_4 0,625,415,505//HP-4の画像内座標
#define TEX_HP_5 0,625,519,609//HP-5の画像内座標
#define TEX_HP_6 0,625,623,713//HP-6の画像内座標
#define TEX_HP_7 0,625,726,816//HP-7の画像内座標
#define TEX_HP_8 0,625,830,920//HP-8の画像内座標
#define TEX_HP_9 0,625,934,1024//HP-9の画像内座標
#define TEX_HP_10 0,625,1037,1127//HP-10の画像内座標

#define HP_SIZE 312.5f,45.0f //HPバーの大きさ
#define HP_POS 312.5f,45.0f //HPバーの座標

class CUiTexture : public CCharacter
{
public:
	//UIの種類
	enum class EUiType
	{
		Hp,
	};
	//デフォルトコンストラクタ
	CUiTexture();
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	/// <param name="uiType">UIの種類</param>
	/// <param name="left">テクスチャの左</param>
	/// <param name="right">テクスチャの右</param>
	/// <param name="bottom">テクスチャの下</param>
	/// <param name="top">テクスチャの上</param>
	CUiTexture(float w, float h, EUiType uiType, int left = 0, int right = 0, int bottom = 0, int top = 0);

	//更新
	void Update();

	//HPの表示を更新
	void HpBarUpdate();

	//staticメソッドの宣言
	static CTexture* GetTextureHpBar();
private:
	//UIの種類
	EUiType mUiType;
	//現在のHP
	float mHp;
	//HPの最大値
	float mMaxHp;
	//幅の保存
	float mW;

	//static変数の宣言
	static CTexture mTextureHpBar;
};
#endif