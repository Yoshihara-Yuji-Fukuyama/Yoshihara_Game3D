#include "CXPlayer.h"

#define PLAYER_ROTATION CVector(0.0f,2.0f,0.0f)//��]���x
#define PLAYER_VELOCITY CVector(0.0f,0.0f,0.1f)//�ړ����x

CXPlayer::CXPlayer()
	: mColSphereHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSphereBody(this, nullptr, CVector(), 0.5f)
	, mColSphereSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f, CCollider::ETag::ESWORD)
{
	//�^�O�Ƀv���C���[��ݒ�
	mCharaTag = ECharaTag::EPLAYER;
}

void CXPlayer::Update()
{
	//�U�����[�V�����̂Ƃ�
	if (GetAnimationIndex() == 3)
	{
		//�A�j���[�V�������I��������
		//�U���I���A�j���[�V�����ɕύX
		if (IsAnimationFinished() == true)
		{		
			ChangeAnimation(4, false, 30);
		}
	}
	//�U���I�����[�V�����̂Ƃ�
	else if (GetAnimationIndex() == 4)
	{
		//�A�j���[�V�������I��������
		//�ҋ@�A�j���[�V�����ɕύX
		if (IsAnimationFinished() == true)
		{	
			ChangeAnimation(0, true, 60);
		}
	}
	//��L�ȊO�̂Ƃ�
	else
	{
		//W�L�[��������Ă���΁A�������[�V�����ɕύX
        //�O�����Ɉړ�
		if (mInput.Key('W'))
		{
			ChangeAnimation(1, true, 60);
			mPosition = mPosition + PLAYER_VELOCITY * mMatrixRotate;
		}
		//W�L�[��������ĂȂ��Ȃ�ҋ@���[�V�����ɕύX
		else
		{
			ChangeAnimation(0, true, 60);
		}
		//A�L�[�������ꂽ�獶�ɉ�]
		if (mInput.Key('A'))
		{
			mRotation = mRotation + PLAYER_ROTATION;
		}
		//D�L�[�������ꂽ��E�ɉ�]
		else if (mInput.Key('D'))
		{
			mRotation = mRotation - PLAYER_ROTATION;
		}

		//���N���b�N�������ꂽ��A�U�����[�V�����ɕύX
		if (mInput.Key(VK_LBUTTON))
		{
			ChangeAnimation(3, false, 30);
		}
	}

	//�ϊ��s��A�A�j���[�V�����̍X�V
	CXCharacter::Update();
}

//�����ݒ�
//���R���C�_�̐����ƁA�{�[���Ƃ̘A��
void CXPlayer::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSphereHead.SetMatrix(&mpCombinedMatrix[12]);
	//��
	mColSphereBody.SetMatrix(&mpCombinedMatrix[9]);
	//��
	mColSphereSword.SetMatrix(&mpCombinedMatrix[22]);
}
