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
	mPlayerModel.Load(MODEL_PLAYER);//0:�O����
	mKnightModel.Load(MODEL_KNIGHT);
	//�ǉ��A�j���[�V�����ǂݍ���
	mPlayerModel.AddAnimationSet(BACKWARD); //1:������
	mPlayerModel.AddAnimationSet(L_WALK);   //2:������
	mPlayerModel.AddAnimationSet(R_WALK);   //3:�E����
	mPlayerModel.AddAnimationSet(AIM_IDLE); //4:�\���ҋ@
	mPlayerModel.AddAnimationSet(Fire);     //5:�ˌ�
	mPlayerModel.AddAnimationSet(JUMP_UP);  //6:�W�����v
	mPlayerModel.SeparateAnimationSet(6, 5, 16, "JumpUp");//7:�W�����v�㏸
	mPlayerModel.AddAnimationSet(JUMP_DOWN);//8:�W�����v�~��
	mPlayerModel.AddAnimationSet(RUN);//9:����
	mPlayerModel.AddAnimationSet(IDLE_RELOAD);//10:�~�܂��ă����[�h
	mPlayerModel.AddAnimationSet(WALK_RELOAD);//11:�����Ȃ��烊���[�h
	//�p���f�B���̃C���X�^���X�쐬
	mpPaladin = new CPaladin();
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
	
	//�w�i���f��
	mBackGround.Load(MODEL_BACKGROUND);
	//�v���C���[�̏����ݒ�
	mXPlayer.Init(&mPlayerModel);
	
	//�G�̏����ݒ�
	mXEnemy.Init(&mKnightModel);
	//�ҋ@�A�j���[�V�����ɕύX
	mXEnemy.ChangeAnimation(2, true, 200);
	//�U���A�j���[�V�����ɕύX
	mpPaladin->ChangeAnimation(1, true, 50);
	
	//�t�H���g�̃��[�h
	mFont.Load("FontG.png", 1, 4096 / 64);
}

void CApplication::Start()
{
	//�J�����̐ݒ�
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
	//CApplication��Init()
	Init();
	
	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
	/*mColliderTriangle.Set(nullptr, nullptr
		, CVector(-50.0f, 0.0f, -50.0f)
		, CVector(-50.0f, 0.0f, 50.0f)
		, CVector(50.0f, 0.0f, -50.0f));*/
	//TODO:�n�ʂ̓����蔻����o��
	//�G�̔z�u
	mXEnemy.SetPosition(CVector(7.0f, 0.0f, 0.0f));
	//�p���f�B���̔z�u
	mpPaladin->SetPosition(CVector(-1.0f, 0.0f, 5.0f));
	mpPaladin->SetScale(CVector(0.025f, 0.025f, 0.025f));
}

void CApplication::Update()
{	
	/*
	//�v���C���[�̍X�V
	mXPlayer.Update();
	mXPlayer.WeponUpdate();
	//�G�̍X�V
	mXEnemy.Update();
	//�p���f�B���̍X�V
	mpPaladin->Update();
	mpPaladin->WeponUpdate();
	*/
	CTaskManager::GetInstance()->Update();
	
	mActionCamera.SetPosition(CVector(mXPlayer.GetPosition().GetX(), mXPlayer.GetPosition().GetY() + 5.0f, mXPlayer.GetPosition().GetZ()));
	mActionCamera.Update();
	mActionCamera.Render();
	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.GetM());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.GetTranspose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);

	/*
	//�v���C���[�`��
	mXPlayer.Render();
	mXPlayer.WeponRender();//�v���C���[�̕���`��
	//�G�`��
	mXEnemy.Render();
	//�p���f�B���̕`��
	mpPaladin->Render();
	mpPaladin->WeponRender();
	*/
	mBackGround.Render();
	CTaskManager::GetInstance()->Render();
	CTaskManager::GetInstance()->Delete();

	//�R���C�_�̕`��
	CCollisionManager::GetInstance()->Render();
	//�Փˏ���
	CCollisionManager::GetInstance()->Collision();


	//2D�`��J�n
	CCamera::Start(0, 1920, 0, 1080);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");
	
	CVector screen;
	//Enemy�̍��W���X�N���[�����W�֕ϊ�����
	if (CActionCamera::GetInstance()->WorldToScreen(&screen, mXEnemy.GetPosition()))
	{
		//�ϊ���̍��W�ɕ�������o�͂���
		mFont.Draw(screen.GetX(), screen.GetY(), 7, 14, "ENEMY");
	}
	
	//2D�̕`��I��
	CCamera::End();
}

