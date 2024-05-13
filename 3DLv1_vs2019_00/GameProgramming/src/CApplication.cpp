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
const CMatrix& CApplication::GetModelViewInverse()
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

//�����ݒ�
void CApplication::Init()
{
	//3D���f���t�@�C���ǂݍ���
	mPlayerModel.Load(MODEL_PLAYER);
	mKnightModel.Load(MODEL_KNIGHT);
	//�G�̃A�j���[�V�����𔲂��o��
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//1:�ړ�
	mKnightModel.SeparateAnimationSet(0, 1530, 1830, "idle1");//2:�ҋ@
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//3:�_�~�[
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//4:�_�~�[
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//5:�_�~�[
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//6:�_�~�[
	mKnightModel.SeparateAnimationSet(0, 440, 520, "attack1");//7:Attack1
	mKnightModel.SeparateAnimationSet(0, 520, 615, "attack2");//8:Attack2
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//9:�_�~�[
	mKnightModel.SeparateAnimationSet(0, 10, 80, "walk");//10:�_�~�[
	mKnightModel.SeparateAnimationSet(0, 1160, 1260, "death1");//11:�_�E��
	//�v���C���[�̏����ݒ�
	mXPlayer.Init(&mPlayerModel);
	//�G�̏����ݒ�
	mXEnemy.Init(&mKnightModel);
	//�ҋ@�A�j���[�V�����ɕύX
	mXEnemy.ChangeAnimation(2, true, 200);
}

void CApplication::Start()
{
	//CApplication��Init()
	Init();

	//�G�̔z�u
	mXEnemy.SetPosition(CVector(7.0f, 0.0f, 0.0f));
	
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Update()
{	
	//�v���C���[�̍X�V
	mXPlayer.Update();
	//�G�̍X�V
	mXEnemy.Update();

	//�J�����̃p�����[�^���쐬����
	CVector  e, c, u;//���_�A�����_�A�����
	//���_�����߂�
	e = CVector(1.0f, 2.0f, 10.0f);
	//�����_�����߂�
	c = CVector();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f);
	//�J�����̐ݒ�
	gluLookAt(e.GetX(), e.GetY(), e.GetZ(), c.GetX(), c.GetY(), c.GetZ(), u.GetX(), u.GetY(), u.GetZ());
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	//X���{��]
	if (mInput.Key('K'))
	{
		mMatrix = mMatrix * CMatrix().SetRotateX(1);
	}
	//X���|��]
	if (mInput.Key('I'))
	{
		mMatrix = mMatrix * CMatrix().SetRotateX(-1);
	}
	//Y���{��]
	if (mInput.Key('L'))
	{
		mMatrix = mMatrix * CMatrix().SetRotateY(1);
	}
	//Y���|��]
	if (mInput.Key('J'))
	{
		mMatrix = mMatrix * CMatrix().SetRotateY(-1);
	}
	//�s��ݒ�
	glMultMatrixf(mMatrix.GetM());

	//���_�ɃA�j���[�V������K�p����
	mPlayerModel.AnimateVertex();
	//�v���C���[�`��
	mXPlayer.Render();
	//�R���C�_�̕`��
	CCollisionManager::Instance()->Render();
	//�Փˏ���
	CCollisionManager::Instance()->Collision();
	//�G�`��
	mXEnemy.Render();

	//2D�`��J�n
	CCamera::Start(0, 800, 0, 600);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");

	//2D�̕`��I��
	CCamera::End();
}

