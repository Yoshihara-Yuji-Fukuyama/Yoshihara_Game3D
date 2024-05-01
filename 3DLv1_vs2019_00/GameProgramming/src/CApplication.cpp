#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"


//�N���X��static�ϐ�
CMatrix CApplication::mModelViewInverse;
CTexture CApplication::mTexture;

//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"//�w�i���f��
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"       //�v���C���[���f��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"          //�G�A���@���f��
#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

//���f���r���[�s��̎擾
const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

//Texture�N���X�̃C���X�^���X���擾
CTexture* CApplication::GetTexture()
{
	return &mTexture;
}

CUi* CApplication::spUi = nullptr;

CUi* CApplication::GetUi()
{
	return spUi;//�C���X�^���X�̃|�C���^��Ԃ�
}

//�f�X�g���N�^
CApplication::~CApplication()
{
	delete spUi;//�C���X�^���XUi�̍폜
}

void CApplication::Start()
{
	//3D���f���t�@�C���̓ǂݍ���
	mModelX.Load(MODEL_FILE);
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{	
	//�ŏ��̃A�j���[�V�����̌��ݎ��Ԃ�ݒ肷��
	mModelX.GetAnimationSet()[0]->SetTime(
		mModelX.GetAnimationSet()[0]->GetTime() + 1.0f);
	mModelX.GetAnimationSet()[0]->SetTime(
		(int)mModelX.GetAnimationSet()[0]->GetTime() %
		(int)(mModelX.GetAnimationSet()[0]->GetMaxTime() + 1));
	//�ŏ��̃A�j���[�V�����̏d�݂�1.0(100%)�ɂ���
	mModelX.GetAnimationSet()[0]->SetWeight(1.0f);
	//�t���[���̕ϊ��s����A�j���[�V�����ōX�V����
	mModelX.AnimateFrame();
	//�t���[���̍����s����v�Z����
	mModelX.GetFrames()[0]->SetAnimateCombined(&mMatrix);

	//�J�����̃p�����[�^���쐬����
	CVector  e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(1.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);
	//�J�����̐ݒ�
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//X���{��]
	if (mInput.Key('K'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(1);
	}
	//X���|��]
	if (mInput.Key('I'))
	{
		mMatrix = mMatrix * CMatrix().RotateX(-1);
	}
	//Y���{��]
	if (mInput.Key('L'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(1);
	}
	//Y���|��]
	if (mInput.Key('J'))
	{
		mMatrix = mMatrix * CMatrix().RotateY(-1);
	}
	//�s��ݒ�
	glMultMatrixf(mMatrix.M());

	//���_�ɃA�j���[�V������K�p����
	mModelX.AnimateVertex();
	//���f���`��
	mModelX.Render();

	//2D�`��J�n
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");

	//2D�̕`��I��
	CCamera::End();
}

