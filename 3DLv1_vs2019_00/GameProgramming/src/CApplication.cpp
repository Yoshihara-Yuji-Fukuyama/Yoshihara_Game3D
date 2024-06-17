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
	//�v���C���[3D���f���t�@�C���ǂݍ���
	mPlayerModel.Load(MODEL_PLAYER);//0:�O����
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
	//�G3D���f���t�@�C���ǂݍ���
	mKnightModel.Load(MODEL_PLAYER);//0:�O����
	//�ǉ��A�j���[�V�����ǂݍ���
	mKnightModel.AddAnimationSet(BACKWARD); //1:������
	mKnightModel.AddAnimationSet(L_WALK);   //2:������
	mKnightModel.AddAnimationSet(R_WALK);   //3:�E����
	mKnightModel.AddAnimationSet(AIM_IDLE); //4:�\���ҋ@
	mKnightModel.AddAnimationSet(Fire);     //5:�ˌ�
	mKnightModel.AddAnimationSet(JUMP_UP);  //6:�W�����v
	mKnightModel.SeparateAnimationSet(6, 5, 16, "JumpUp");//7:�W�����v�㏸
	mKnightModel.AddAnimationSet(JUMP_DOWN);//8:�W�����v�~��
	mKnightModel.AddAnimationSet(RUN);//9:����
	mKnightModel.AddAnimationSet(IDLE_RELOAD);//10:�~�܂��ă����[�h
	mKnightModel.AddAnimationSet(WALK_RELOAD);//11:�����Ȃ��烊���[�h
	//�p���f�B���̃C���X�^���X�쐬
	mpPaladin = new CPaladin();
	
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

