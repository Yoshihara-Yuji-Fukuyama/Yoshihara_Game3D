#ifndef CMODELX_H //�C���N���[�h�K�[�h
#define CMODELX_H

#include "glut.h"
#include <vector>//vector�N���X�̃C���N���[�h�i���I�z��j
#include "CVector.h"
#include "CMatrix.h"
#include "CMyShader.h"
#include "CVertex.h"
class CModelX;      //CModelX�N���X�̐錾
class CModelXFrame; //CModelXFrame�N���X�̐錾
class CMesh;        //CMesh�N���X�̐錾
class CSkinWeights; //CSkinWeights�N���X�̐錾
class CAnimationSet;//CAnimationSet�N���X�̐錾
class CAnimation;   //CAnimation�N���X�̐錾
class CAnimationKey;//CAnimationKey�N���X�̐錾
class CMaterial;    //CMaterial�̐錾
class CMyShader;    //CMyShader�N���X�̐錾


#define MODEL_PLAYER "res\\Ch44_nonPBR.x"  //�v���C���[���f��
#define BACKWARD "res\\RunBackward.x"   //������
#define L_WALK "res\\WalkLeft.x"           //������
#define R_WALK "res\\WalkRight.x"          //�E����
#define AIM_IDLE "res\\RifleAimingIdle.x"  //�\���ҋ@
#define Fire "res\\FiringRifle.x"          //�ˌ�
#define JUMP_UP "res\\JumpUp.x"            //�W�����v�㏸
#define JUMP_DOWN "res\\JumpDown.x"        //�W�����v�~��
#define RUN "res\\RifleRun.x"              //����
#define MODEL_KNIGHT "res\\knight\\knight_low.x"//�G���f��

//�̈������}�N����
#define SAFE_DELETE_ARRAY(a) {if(a) delete[] a; a = nullptr;}
//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

/*
CModelX
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX 
{
	friend CModelXFrame;
	friend CAnimationSet;
	friend CAnimation;
	friend CMyShader;
public:
	CModelX();
	~CModelX();

	//�m�[�h�̓ǂݔ�΂�
	void SkipNode();
	//�P��̎��o��
	char* GetToken();
	//mToken�̃|�C���^��Ԃ��̂�
	char* GetOnlyToken();
	//�t�@�C���ǂݍ���
	void Load(char* file);
	//�ǂݍ��ݍς݂�����
	bool IsLoaded();
	//�`��
	void Render();
	//�V�F�[�_�[���g�����`��
	void RenderShader(CMatrix* m);
	//�g�[�N�����Ȃ��Ȃ�����true
	bool EOT();

	//�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
	CModelXFrame* FindFrame(char* name);
	//�}�e���A�����ɊY������}�e���A���̃A�h���X��Ԃ�
	CMaterial* FindMaterial(char* name);

	//�t���[���̕ϊ��s����X�V
	void AnimateFrame();
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	void SetSkinWeightFrameIndex();
	//���_�ɃA�j���[�V������K�p
	void AnimateVertex();
	//���_�v�Z���w�肵�������s��ōs��
	void AnimateVertex(CMatrix*);
	/// <summary>
	/// �A�j���[�V�����𔲂��o��
	/// </summary>
	/// <param name="idx">�����������A�j���[�V�����̔ԍ�</param>
	/// <param name="start">�����������A�j���[�V�����̊J�n����</param>
	/// <param name="end">�����������A�j���[�V�����̏I������</param>
	/// <param name="name">�ǉ�����A�j���[�V�����Z�b�g�̖��O</param>
	void SeparateAnimationSet(int idx, int start, int end, char* name);
	//�A�j���[�V�����Z�b�g�̒ǉ�
	void AddAnimationSet(const char* file);

	//mFrame�z���Ԃ�
	std::vector<CModelXFrame*>& GetFrames();
	//mAnimationSet�z���Ԃ�
	std::vector<CAnimationSet*>& GetAnimationSet();
	//mMaterial�z��̎擾
	std::vector<CMaterial*>& GetMaterial();

private:
	std::vector<CModelXFrame*> mFrame;       //�t���[���̔z��
	std::vector<CAnimationSet*>mAnimationSet;//�A�j���[�V�����Z�b�g�̔z��
	std::vector<CMaterial*>mMaterial;        //�}�e���A���z��
	bool IsDelimiter(char c);//c����؂蕶���Ȃ�true��Ԃ�
	char* mpPointer;  //�ǂݍ��݈ʒu
	char mToken[1024];//���o�����P��̗̈�
	bool mLoaded;//�ǂݍ��ݍς݃t���O
	CMatrix* mpSkinningMatrix;//�V�F�[�_�[�p�X�L���}�g���b�N�X
	CMyShader mShader;//�V�F�[�_�[�̃C���X�^���X
};

//Frame�N���X
class CModelXFrame 
{
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
	friend CMyShader;
public:
	CModelXFrame();
	/*
	model:CModelX�̃|�C���^
	�t���[�����쐬����
	�ǂݍ��ݒ���Frame��������΁A
	�t���[�����쐬���A�q�t���[���ɒǉ�����*/
	CModelXFrame(CModelX* model);
	~CModelXFrame();

	//�`��
	void Render();
	//mIndex�̒l��Ԃ�
	int GetIndex();
	//�����s��̍쐬
	void SetAnimateCombined(CMatrix* parent);
	//mCombinedMatrix(�����s��)��Ԃ�
	const CMatrix& GetCombinedMatrix();
private:
	std::vector<CModelXFrame*> mChild;//�q�t���[���̔z��
	CMatrix mTransformMatrix;//�ϊ��s��
	CMatrix mCombinedMatrix; //�����s��
	char* mpName;//�t���[�����O
	int mIndex;  //�t���[���ԍ�
	CMesh* mpMesh;//Mesh�f�[�^
};

//Mesh�N���X
class CMesh
{
	friend CMyShader;
public:
	CMesh();
	~CMesh();

	//�ǂݍ��ݏ���
	void Init(CModelX* model);
	//�`��
	void Render();
	//�X�L���E�F�C�g�Ƀt���[���ԍ���ݒ肷��
	void SetSkinWeightFrameIndex(CModelX* model);
	//���_�ɃA�j���[�V������K�p
	void AnimateVertex(CModelX* model);
	//���_�v�Z���w�肵�������s��ōs��
	void AnimateVertex(CMatrix*);
	//���_�o�b�t�@�̍쐬
	void CreateVertexBuffer();
private:
	int mVertexNum;    //���_��
	CVector* mpVertex; //���_�f�[�^
	int mFaceNum;      //�ʐ�
	int* mpVertexIndex;//�ʂ��\�����钸�_�C���f�b�N�X
	std::vector<int> mTempVertexIndex;//���Œ��_�C���f�b�N�X�����Ă����ꏊ
	int mNormalNum;    //�@����
	CVector* mpNormal; //�@���x�N�g��
	int mMaterialNum;     //�}�e���A����
	int mMaterialIndexNum;//�}�e���A���ԍ����i�ʐ��j
	int* mpMaterialIndex; //�}�e���A���ԍ�
	std::vector<CMaterial*> mMaterial;     //�}�e���A���f�[�^�z��
	std::vector<CSkinWeights*>mSkinWeights;//�X�L���E�F�C�g�z��
	CVector* mpAnimateVertex;//�A�j���[�V�����p���_
	CVector* mpAnimateNormal;//�A�j���[�V�����p�@��
	float* mpTextureCoords;//�e�N�X�`�����W�f�[�^
	std::vector<int> mMaterialVertexCount;//�}�e���A�����Ƃ̖ʐ�
	GLuint mMyVertexBufferId;//���_�o�b�t�@���ʎq
};

/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights 
{
	friend CModelX;
	friend CMesh;
	friend CMyShader;
public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();

	const int& FrameIndex();
	const CMatrix& Offset();
private:
	char* mpFrameName;//�t���[����
	int mFrameIndex;  //�t���[���ԍ�
	int mIndexNum;    //���_�ԍ���
	int* mpIndex;     //���_�ԍ��z��
	float* mpWeight;  //���_�E�F�C�g�z��
	CMatrix mOffset;  //�I�t�Z�b�g�}�g���b�N�X
};

/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet 
{
	friend CModelX;
public:
	CAnimationSet();
	CAnimationSet(CModelX* model);
	~CAnimationSet();

	//���Ԃ̐ݒ�
	void SetTime(float time);
	//mTime��Ԃ�
	float GetTime();
	//mMaxTime��Ԃ�
	float GetMaxTime();
	//�d�݂̐ݒ�
	void SetWeight(float weight);
	//�A�j���[�V�����ϊ��s����v�Z����
	void AnimateMatrix(CModelX* model);
	//mAnimation��Ԃ�
	std::vector<CAnimation*>& GetAnimation();
private:
	float mTime;   //���ݎ���
	float mWeight; //�d��
	float mMaxTime;//�ő厞��
	char* mpName;  //�A�j���[�V�����Z�b�g��
	std::vector<CAnimation*> mAnimation;//�A�j���[�V����
};

/*
CAnimation
�A�j���[�V�����N���X
*/
class CAnimation
{
	friend CModelX;
	friend CAnimationSet;
public:
	CAnimation();
	CAnimation(CModelX* model);
	~CAnimation();
private:
	char* mpFrameName;   //�t���[����
	int mFrameIndex;     //�t���[���ԍ�
	int mKeyNum;         //�L�[���i���Ԑ��j
	CAnimationKey* mpKey;//�L�[�̔z��
};

/*
CAnimationKey
�A�j���[�V�����L�[
*/
class CAnimationKey 
{
	friend CModelX;
	friend CAnimation;
	friend CAnimationSet;
private:
	float mTime;    //����
	CMatrix mMatrix;//�s��
};
#endif

