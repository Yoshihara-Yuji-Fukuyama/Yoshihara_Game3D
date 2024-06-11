#ifndef CWEPON_H
#define CWEPON_H

#include "CCharacter3.h"


class CWepon : public CCharacter3
{
public:
	//����̃^�C�v�̎��
	enum class EWeponType
	{
		EAR, //0
		ESG, //1
		ESR, //2
		EHG, //3
		ESMG,//4
	};
	CWepon();
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="parent">�e</param>
	/// <param name="parent">�e�s��</param>
	/// <param name="position">�ʒu</param>
	/// <param name="rotation">��]</param>
	CWepon(CCharacter3* parent, CMatrix* matrix,
		const CVector& position, CVector* rotation);
	~CWepon();
	void Update();
	//�e�s��̐ݒ�
	void SetMatrix(CMatrix* m);
	//�e�𔭎˂���֐�
	void ShotBullet();
	//�����[�h������֐�
	void Reload();

	//IsRun��ݒ�
	void SetRun(bool isRun);
	//IsJump��ݒ�
	void SetJump(bool isJump);
private:
	static CModel sModel;//���f���f�[�^
	EWeponType mWeponType;//����̎��
	CMatrix* mpMatrix;    //�e�s��
	CVector* mpRotation;  //�e��]
	CVector mAdjustPosition;//�ʒu����
	bool IsRun;//�����Ă��邩�ǂ�����ۑ�����
	bool IsJump;//�W�����v�����Ă��邩�ǂ�����ۑ�����
	int mAmmo;//�c�e��
};
#endif
