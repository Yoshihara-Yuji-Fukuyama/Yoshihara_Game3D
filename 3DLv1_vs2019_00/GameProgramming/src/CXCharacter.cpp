#include "CXCharacter.h"

CXCharacter::CXCharacter()
	: mpCombinedMatrix(nullptr)
{
	mScale = CVector(1.0f, 1.0f, 1.0f);
}

//�X�V����
void CXCharacter::Update()
{
	//�ϊ��s��̍X�V
	CTransform::Update();
	//�A�j���[�V�������X�V����
	Update(mMatrix);
}

//����������
void CXCharacter::Init(CModelX* model)
{
	mpModel = model;
	//�ŏ��̃A�j���[�V�����ɂ���
	mAnimationIndex = 0;
	//�J��Ԃ��Đ�����
	mAnimationLoopFlg = true;
	//1�A�j���[�V�����ڂ̍ő�t���[����
	mAnimationFrameSize = model->GetAnimationSet()[0]->GetMaxTime();
	//�A�j���[�V�����̃t���[�����ŏ��ɂ���
	mAnimationFrame = 0.0f;
	mpModel->GetAnimationSet()[mAnimationIndex]->SetTime(mAnimationFrame);
	//�A�j���[�V�����̏d�݂�1.0�i100%�j�ɂ���
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(1.0f);
	//�����s��ޔ��G���A�̊m��
	mpCombinedMatrix = new CMatrix[model->GetFrames().size()];
}

/*
�A�j���[�V������؂�ւ���
index:�A�j���[�V�����̔ԍ�
loop:true�@�J��Ԃ�
framesize:�Ō�܂ōĐ�����̂Ɏg�p�����t���[����
*/
void CXCharacter::ChangeAnimation(int index, bool loop, float framesize)
{
	//�����ꍇ�͐؂�ւ��Ȃ�
	if (mAnimationIndex == index)return;
	//���̃A�j���[�V�����̏d�݂�0.0(0%)�ɂ���
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(0.0f);
	//�ԍ��A�J��Ԃ��A�t���[������ݒ�
	mAnimationIndex = index % mpModel->GetAnimationSet().size();
	mAnimationLoopFlg = loop;
	mAnimationFrameSize = framesize;
	//�A�j���[�V�����̃t���[�����ŏ��ɂ���
	mAnimationFrame = 0.0f;
	mpModel->GetAnimationSet()[mAnimationIndex]->SetTime(mAnimationFrame);
	//�A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(1.0f);
}

/*
�X�V����
matrix:�ړ��A��]�A�g��k���̍s��
*/
void CXCharacter::Update(CMatrix& matrix)
{
	//�S�ẴE�F�C�g��0�ɂ���
	for (size_t i = 0; i < mpModel->GetAnimationSet().size(); i++)
	{
		mpModel->GetAnimationSet()[i]->SetWeight(0.0f);
	}
	//���̃A�j���[�V�����̃E�F�C�g����1�ɂ���
	mpModel->GetAnimationSet()[mAnimationIndex]->SetWeight(1.0f);

	//�Ō�܂ōĐ�����
	if (mAnimationFrame <= mAnimationFrameSize)
	{
		//�A�j���[�V�����̎��Ԃ��v�Z
		mpModel->GetAnimationSet()[mAnimationIndex]->
			SetTime(mpModel->GetAnimationSet()[mAnimationIndex]->
			GetMaxTime() * mAnimationFrame / mAnimationFrameSize);
		//�t���[����i�߂�
		mAnimationFrame++;
	}
	else
	{
		//�J��Ԃ��̏ꍇ�́A�t���[����0�ɖ߂�
		if (mAnimationLoopFlg)
		{
			//�A�j���[�V�����̃t���[�����ŏ��ɂ���
			mAnimationFrame = 0.0f;
			mpModel->GetAnimationSet()[mAnimationIndex]->
				SetTime(mAnimationFrame);
		}
		else
		{
			mpModel->GetAnimationSet()[mAnimationIndex]->
				SetTime(mpModel->GetAnimationSet()[mAnimationIndex]->
					GetMaxTime());
		}
	}
	//�t���[���̕ϊ��s����A�j���[�V�����ōX�V����
	mpModel->AnimateFrame();
	//�t���[���̍����s����v�Z����
	mpModel->GetFrames()[0]->SetAnimateCombined(&matrix);
	//�����s��̑ޔ�
	for (size_t i = 0; i < mpModel->GetFrames().size(); i++)
	{
		mpCombinedMatrix[i] =
			mpModel->GetFrames()[i]->GetCombinedMatrix();
	}	
}

//�`�悷��
void CXCharacter::Render()
{
	//�V�F�[�_�[���g���ĕ`��
	mpModel->RenderShader(mpCombinedMatrix);
}

//�A�j���[�V�����̏I������
bool CXCharacter::IsAnimationFinished()
{
	return mAnimationFrame >= mAnimationFrameSize;
}

//�A�j���[�V�����ԍ��̎擾
int CXCharacter::GetAnimationIndex()
{
	return mAnimationIndex;
}

//�L�����̕��������������������ɕς���
void CXCharacter::ChangeDirection(CVector charZ, CVector direction)
{
	//�V��
	const float MARGIN = 0.06f;
	//�����̌����ƌ��������������ŊO��
	float cross = charZ.Cross(direction).GetY();
	//�����̌����ƌ��������������œ���
	float dot = charZ.Dot(direction);
	//�O�ς��v���X�͉E��]
	if (cross > MARGIN)
	{
		mRotation.SetY(mRotation.GetY() - 5.0f);
	}
	//�O�ς��}�C�i�X�͍���]
	else if (cross < -MARGIN)
	{
		mRotation.SetY(mRotation.GetY() + 5.0f);
	}
	//�O��̌����������Ƃ����ς�1.0f
	//���������������������Ă����牽�����Ȃ�
	else if (dot < 1.0f - MARGIN)
	{

	}
}
