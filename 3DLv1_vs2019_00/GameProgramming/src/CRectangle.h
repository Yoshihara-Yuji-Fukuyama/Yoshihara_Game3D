#ifndef CRECTANGLE_H
#define CRECTANGLE_H

class CRectangle
{
public:
	//コンストラクタ
	CRectangle();
	//デストラクタ
	~CRectangle();
	//描画
	void Render();

	/// <summary>
	/// 四角形の描写
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	void Render(float x, float y, float w, float h);

	/// <summary>
	/// 大きさの設定
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="w">幅</param>
	/// <param name="h">高さ</param>
	void Set(float x, float y, float w, float h);

	//X座標の取得
	float GetX();
	//Y座標の取得
	float GetY();
	//Z座標の取得
	float GetZ();
	//幅の取得
	float GetW();
	//高さの取得
	float GetH();

	/// <summary>
	/// //Y座標の設定
	/// </summary>
	/// <param name="y">Y座標の変更値</param>
	void SetY(float y);
	/// <summary>
	/// //X座標の設定
	/// </summary>
	/// <param name="x">X座標の変更値</param>
	void SetX(float x);
	/// <summary>
	/// //Z座標の設定
	/// </summary>
	/// <param name="x">Z座標の変更値</param>
	void SetZ(float z);

	/// <summary>
	/// 幅の設定
	/// </summary>
	/// <param name="w">幅</param>
	void SetW(float w);
	/// <summary>
	/// 高さの設定
	/// </summary>
	/// <param name="h">高さ</param>
	void SetH(float h);

protected:
	float mX; //X座標
	float mY; //Y座標
	float mZ; //足元

private:
	float mW; //幅
	float mH; //高さ

};

#endif