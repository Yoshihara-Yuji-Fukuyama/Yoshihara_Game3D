#include "CApplication.h"
#include "CCamera.h"
//OpenGL
#include "glut.h"


//�N���X��static�ϐ�
CMatrix CApplication::mModelViewInverse;
CTexture CApplication::mTexture;
CUi* CApplication::spUi = nullptr;
CApplication* CApplication::spInstance = nullptr;

//�w�i���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj","res\\sky.mtl"//�w�i���f��
#define MODEL_STAGE "res\\StageWall.obj","res\\StageWall.mtl" //�X�e�[�W���f��
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"       //�v���C���[���f��
#define MODEL_C5 "res\\c5.obj","res\\c5.mtl"          //�G�A���@���f��
#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��

#define MAX_ENEMY 1//�G�̍ő吔

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

//UI�N���X�̃C���X�^���X���擾
CUi* CApplication::GetUi()
{
	return spUi;//�C���X�^���X�̃|�C���^��Ԃ�
}

//�C���X�^���X�̎擾
CApplication* CApplication::GetInstance()
{
	return spInstance;
}

//�f�X�g���N�^
CApplication::~CApplication()
{
	delete spUi;//�C���X�^���XUi�̍폜
}

//�����ݒ�
void CApplication::Init()
{
	//�v���C���[�̏����ݒ�
	mXPlayer.Init(&mPlayerModel);
	
	mEnemyNum = 0;//�G�̐���0
}

void CApplication::Start()
{
	//�C���X�^���X�ɐݒ�
	spInstance = this;
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
	mPlayerModel.AddAnimationSet(HIT);//12:��e
	mPlayerModel.AddAnimationSet(WALK_HIT);//13:�����Ȃ����e
	mPlayerModel.AddAnimationSet(DEATH);//14:���S
	mPlayerModel.AddAnimationSet(ROLL);//15:���[�����O
	//�w�i���f��
	mBackGround.Load(MODEL_BACKGROUND);
	//�t�H���g�̃��[�h
	mFont.Load("FontG.png", 1, 4096 / 64);
	//�J�����̐ݒ�
	mActionCamera.Set(5.0f, -15.0f, 180.0f);
	//CApplication��Init()
	Init();
	//�G���o��������
	SpawnEnemy();
	
	CUiTexture::GetTextureHpBar()->Load(TEXTURE_HP_BAR);//HP�o�[�摜�̓ǂݍ���
	mpHpBar = new CUiTexture(CXPlayer::GetInstance()->GetHp(),HP_SIZE, CUiTexture::EUiType::Hp);//HP�o�[�̐���
	mpEnemyHpBar = new CUiTexture(mpXEnemy[0]->GetHp(),ENEMY_HP_SIZE, CUiTexture::EUiType::Hp);//�GHP�o�[�̐���

	//�w�i���f������O�p�R���C�_�𐶐�
	//�e�C���X�^���X�Ɛe�s��͂Ȃ�
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

void CApplication::Update()
{	
	SpawnEnemy();

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


	mBackGround.Render();

	CTaskManager::GetInstance()->Render();
	CTaskManager::GetInstance()->Delete();

	//�R���C�_�̕`��
	//CCollisionManager::GetInstance()->Render();
	//�Փˏ���
	CCollisionManager::GetInstance()->Collision();


	//2D�`��J�n
	CCamera::Start(0, 1920, 0, 1080);

	mFont.Draw(20, 20, 10, 12, "3D PROGRAMING");

	mpHpBar->SetHp(CXPlayer::GetInstance()->GetHp());//�v���C���[�̌���HP��ݒ�
	mpHpBar->Update();
	mpHpBar->Render();
	
	CVector screen;
	//Enemy�̍��W���X�N���[�����W�֕ϊ�����
	if (CActionCamera::GetInstance()->WorldToScreen(&screen, mpXEnemy[0]->GetPosition()))
	{
		//�ϊ���̍��W��HP�o�[���o�͂���
		mpEnemyHpBar->Set(screen.GetX(), screen.GetY(),
			mpEnemyHpBar->GetW(), mpEnemyHpBar->GetH());
		mpEnemyHpBar->SetHp(mpXEnemy[0]->GetHp());//�G�̌���HP��ݒ�
		mpEnemyHpBar->Update();
		mpEnemyHpBar->Render();
	}
	
	//2D�̕`��I��
	CCamera::End();
}

void CApplication::SpawnEnemy()
{
	//������t����
	//�G�̍ő吔�܂Ő�������
	while (mEnemyNum < MAX_ENEMY)
	{
		//X���W-100����100�AY���W0�AZ���W-100����100�Ƀ����_���ɐ���
		mpXEnemy.push_back(new CXEnemy(CVector(rand() % 200 - 100, 0.0f, rand() % 200 - 100)));
		mEnemyNum++;
	}
}

void CApplication::DeathEnemy()
{
	mEnemyNum--;
}

