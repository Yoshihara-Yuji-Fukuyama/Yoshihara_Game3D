#include "CCollider.h"
#include "CCollisionManager.h"
#include "CColliderLine.h"

//mTypeの値を返す
CCollider::EType CCollider::GetType()
{
	return mType;
}

//mTagの値を返す
CCollider::ETag CCollider::GetTag()
{
	return mTag;
}

CCollider::CCollider()
	: mpMatrix(&mMatrix)
	, mType(EType::ESPHERE)
	, mRadius(0)
{
	//コリジョンマネージャに追加
	CCollisionManager::GetInstance()->Add(this);
}

CCollider::~CCollider()
{
	//コリジョンリストから削除
	CCollisionManager::GetInstance()->Remove(this);
}

CCollider::CCollider(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, float radius, ETag tag)
	: CCollider()
{
	//タグの設定
	mTag = tag;
	//親決定
	mpParent = parent;
	//親行列設定
	mpMatrix = matrix;
	//CTransform設定
	mPosition = position;//位置
	//半径設定
	mRadius = radius;
	//コリジョンマネージャに追加
	//CCollisionManager::GetInstance()->Add(this);
}

void CCollider::Render()
{
	glPushMatrix();
	//コライダの中心座標を計算
	//自分の座標×親の変換行列
	CVector pos = mPosition * *mpMatrix;
	//中心座標へ移動
	glMultMatrixf(CMatrix().SetTranlate(pos.GetX(), pos.GetY(), pos.GetZ()).GetM());
	//DIFFUSE赤色設定
	float c[] = { 1.0f,0.0f,0.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

void CCollider::ChangePriority()
{
	//自分の座標×親の変換行列をかけてワールド座標を求める
	CVector pos = mPosition * *mpMatrix;
	//ベクトルの長さが優先度
	CCollider::ChangePriority(pos.Length());
}

void CCollider::ChangePriority(int priority)
{
	mPriority = priority;
	CCollisionManager::GetInstance()->Remove(this);//一旦削除
	CCollisionManager::GetInstance()->Add(this);//追加
}

//コライダ1と2の衝突判定
bool CCollider::Collision(CCollider* m, CCollider* o)
{
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//中心から中心へのベクトルを求める
	mpos = mpos - opos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + o->mRadius > mpos.Length())
	{
		//衝突している
		return true;
	}
	//衝突していない
	return false;
}
//三角コライダと球コライダの衝突判定
bool CCollider::CollisionTriangleSphere(CCollider* t, CCollider* s, CVector* a)
{
	CVector v[3], sv, ev;
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//線コライダをワールド座標で作成
	sv = s->mPosition * *s->mpMatrix + normal * s->mRadius;
	ev = s->mPosition * *s->mpMatrix - normal * s->mRadius;
	CColliderLine line(nullptr, nullptr, sv, ev);
	//三角コライダと線コライダの衝突処理
	return CollisionTriangleLine(t, &line, a);
}
//三角コライダと線分コライダの衝突判定
bool CCollider::CollisionTriangleLine(CCollider* t, CCollider* l, CVector* a)
{
	CVector v[3], sv, ev;
	//各コライダの頂点をワールド座標へ変換
	v[0] = t->mV[0] * *t->mpMatrix;
	v[1] = t->mV[1] * *t->mpMatrix;
	v[2] = t->mV[2] * *t->mpMatrix;
	sv = l->mV[0] * *l->mpMatrix;
	ev = l->mV[1] * *l->mpMatrix;
	//面の法線を、外積を正規化して求める
	CVector normal = (v[1] - v[0]).Cross(v[2] - v[0]).Normalize();
	//三角の頂点から線分始点へのベクトルを求める
	CVector v0sv = sv - v[0];
	//三角の頂点から線分終点へのベクトルを求める
	CVector v0ev = ev - v[0];
	//線分が面と交差しているか内積で確認する
	float dots = v0sv.Dot(normal);
	float dote = v0ev.Dot(normal);
	//プラスは交差してない
	if (dots * dote >= 0.0f)
	{
		//衝突してない(調整不要)
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//線分は面と交差している
	
	//面と線分の交点を求める
	//交点の計算
	CVector cross = sv + (ev - sv) * (abs(dots) / (abs(dots) + abs(dote)));

	//交点が三角形内なら衝突している
	//頂点1頂点2ベクトルと頂点1交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[1] - v[0]).Cross(cross - v[0]).Dot(normal) < 0.0f)
	{
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点2頂点3ベクトルと頂点2交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[2] - v[1]).Cross(cross - v[1]).Dot(normal) < 0.0f)
	{
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}
	//頂点3頂点1ベクトルと頂点3交点ベクトルとの外積を求め、
	//法線との内積がマイナスなら、三角形の外
	if ((v[0] - v[2]).Cross(cross - v[2]).Dot(normal) < 0.0f)
	{
		//衝突してない
		*a = CVector(0.0f, 0.0f, 0.0f);
		return false;
	}

	//調整値計算(衝突しない位置まで戻す)
	if (dots < 0.0f)
	{
		//始点が裏面
		*a = normal * -dots;
	}
	else
	{
		//終点が裏面
		*a = normal * -dote;
	}
	return true;
}

//カプセルコライダとカプセルコライダの衝突判定
bool CCollider::CollisionCapsuleCapseule(CCollider* m, CCollider* o, CVector* adjust)
{
	CVector mp1, mp2;
	float radius = m->mRadius + o->mRadius;

	*adjust = CVector();
	if (adjust->CalcSegmentSegmentDist(m->GetV(0), m->GetV(1), o->GetV(0), o->GetV(1), &mp1, &mp2) < radius)
	{
		*adjust = mp1 - mp2;
		float len = radius - adjust->Length();
		*adjust = adjust->Normalize() * len;
		return true;
	}
	return false;
}

//カプセルコライダと球コライダの衝突判定
bool CCollider::CollisionCapsuleSphere(CCollider* m, CCollider* o)
{
	//球コライダの中心座標を求める
	CVector sphereCenter = o->mPosition * *o->mpMatrix;
	//カプセルの軸を表すベクトル
	CVector capsuleLine = m->GetV(1) - m->GetV(0);//終点－始点
	//カプセルの開始点から球の中心までのベクトル
	CVector capsuleStartToSphere = sphereCenter - m->GetV(0);
	//球の中心からカプセルの軸への最も近い点を求めるためのパラメータｔを計算
	float t = capsuleStartToSphere.Dot(capsuleLine) / capsuleLine.Dot(capsuleLine);
	//tを0から1の範囲にクランプ
	t = std::max(0.0f, std::min(1.0f, t));
	//カプセルの軸上の最も近い点を計算
	CVector closestPointOnCapsule = m->GetV(0) + capsuleLine * t;
	//最も近い点から球の中心までのベクトルを計算
	CVector closestToSphereCenter = sphereCenter - closestPointOnCapsule;
	//最も近い点から球の中心までの距離を計算
	float distance = closestToSphereCenter.Length();
	//距離がカプセルの半径と球の半径の合計より小さいかどうかで衝突を判定
	return distance <= (m->mRadius + o->mRadius);
}


//配列mV[i]の要素を返す
const CVector& CCollider::GetV(int i)
{
	return mV[i];
}


//親ポインタの取得
CCharacter3* CCollider::GetParent()
{
	return mpParent;
}

//親行列の設定
void CCollider::SetMatrix(CMatrix* m)
{
	mpMatrix = m;
}
