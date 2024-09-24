#include "glew.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h> //isspace�֐��̐錾
#include "CModelX.h"
#include "CMaterial.h"//CMaterial���g����悤�ɂ��邽��

CModelX::CModelX()
	: mpPointer(nullptr)
	, mLoaded(false)
	, mpSkinningMatrix(nullptr)
{
	//mToken��������
	memset(mToken, 0, sizeof(mToken));
}

CModelX::~CModelX()
{
	//�t���[���̉��
	if (mFrame.size() > 0)
	{
		delete mFrame[0];
	}
	//�A�j���[�V�����Z�b�g�̉��
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		delete mAnimationSet[i];
	}
	//�}�e���A���̉��
	for (size_t i = 0; i < mMaterial.size(); i++)
	{
		delete mMaterial[i];
	}
	SAFE_DELETE_ARRAY(mpSkinningMatrix);
}
//������f�[�^����A�P���1�擾����
char* CModelX::GetToken()
{
	char* p = mpPointer;
	char* q = mToken;
	//�^�u(\t)��()���s(\r)(\n),;"�̋�؂蕶���ȊO�ɂȂ�܂œǂݔ�΂�
	while (*p != '\0' && IsDelimiter(*p))p++;
	if (*p == '{' || *p == '}')
	{
		//{�܂���}�Ȃ�mToken�ɑ�������̕�����
		*q++ = *p++;
	}
	else
	{
		//�^�u(\t)��()���s(\r)(\n),;"�̋�؂蕶���A
		//�܂��́A}�̕����ɂȂ�܂�mToken�ɑ������
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;
	}

	*q = '\0'; //mToken�̍Ō��\0����
	mpPointer = p; //���̓ǂݍ��ރ|�C���g���X�V����

	//����mToken��//�̏ꍇ�́A�R�����g�Ȃ̂ŉ��s�܂œǂݔ�΂�
	/*
	
	
	
	(������1,������2)
	������1�ƕ�����2���������ꍇ�A0��Ԃ��܂��B
	������1�ƕ�����2���������Ȃ��ꍇ�A0�ȊO��Ԃ��܂�
	*/

	if (!strcmp("//", mToken))
	{
		//���s�܂œǂݔ�΂�
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//�ǂݍ��݈ʒu�̍X�V
		mpPointer = p;
		//�P����擾����(�ċA�Ăяo��)
		return GetToken();
	}
	return mToken;
}
//mToken�̃|�C���^��Ԃ�
char* CModelX::GetOnlyToken()
{
	return mToken;
}


void CModelX::Load(char* file)
{
	//�t�@�C���T�C�Y���擾����

	FILE* fp; //�t�@�C���|�C���^�ϐ��̍쐬

	fp = fopen(file, "rb"); //�t�@�C�����I�[�v������
	if (fp == NULL)
	{
		//�G���[�`�F�b�N
		printf("fopen error:%s\n", file);
		return;
	}

	//�t�@�C���̍Ō�ֈړ�
	fseek(fp, 0L, SEEK_END);
	//�t�@�C���T�C�Y�̎擾
	int size = ftell(fp);
	//�t�@�C���T�C�Y�{1�o�C�g���̗̈���m��
	char* buf = mpPointer = new char[size + 1];
	
	//�t�@�C������3D���f���̃f�[�^��ǂݍ���

	//�t�@�C���̐擪�ֈړ�
	fseek(fp, 0L, SEEK_SET);
	//�m�ۂ����̈�Ƀt�@�C���T�C�Y���f�[�^��ǂݍ���
	fread(buf, size, 1, fp);
	//�Ō��\0��ݒ肷��(������̏I�[)
	buf[size] = '\0';
	fclose(fp); //�t�@�C�����N���[�Y����

	//�_�~�[���[�g�t���[���̍쐬
	CModelXFrame* p = new CModelXFrame();
	//���O�Ȃ�
	p->mpName = new char[1];
	p->mpName[0] = '\0';
	//�t���[���z��ɒǉ�
	mFrame.push_back(p);

	//������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0')
	{
		GetToken(); //�P��̎擾
		//template �ǂݔ�΂�
		if (strcmp(mToken, "template") == 0)
		{
			SkipNode();
		}
		//Material�̎�
		else if (strcmp(mToken, "Material") == 0)
		{
			new CMaterial(this);
		}
		//�P�ꂪFrame�̏ꍇ
		else if (strcmp(mToken, "Frame") == 0)
		{
			//�t���[�����擾
			GetToken();
			if (strchr(mToken, '{'))
			{
				//�t���[�����Ȃ��@�X�L�b�v
				SkipNode();
				GetToken(); // }
			}
			else
			{
				//�t���[�����Ȃ����
				if (FindFrame(mToken) == 0)
				{
					//�t���[�����쐬����
					p->mChild.push_back(new CModelXFrame(this));
				}
			}
		}
		//�P�ꂪAnimationSet�̏ꍇ
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);
		}
	}

	SAFE_DELETE_ARRAY(buf); //�m�ۂ����̈���J������
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	SetSkinWeightFrameIndex();

	mLoaded = true;//�ǂݍ��ݍς�

	//���_�o�b�t�@�̍쐬
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		if (mFrame[i]->mpMesh != nullptr)
		{
			mFrame[i]->mpMesh->CreateVertexBuffer();
		}
	}
	//�X�L���}�g���b�N�X�̃G���A�쐬
	mpSkinningMatrix = new CMatrix[mFrame.size()];
	//�V�F�[�_�[�ǂݍ���
	mShader.Load("res\\skinmesh.vert", "res\\skinmesh.flag");
}
//�ǂݍ��ݍς݂�����
bool CModelX::IsLoaded()
{
	return mLoaded;
}
//�m�[�h��ǂݔ�΂�
void CModelX::SkipNode()
{
	//�������I�������I��
	while (*mpPointer != '\0')
	{
		GetToken();//���̒P��擾
		//{�����������烋�[�v�I��
		if (strchr(mToken, '{')) break;
	}
	int count = 1;
	//�������I��邩�A�J�E���g��0�ɂȂ�����I��
	while (*mpPointer != '\0' && count > 0)
	{
		GetToken();//���̒P��擾
		//{��������ƃJ�E���g�A�b�v
		if (strchr(mToken, '{')) count++;
		//}��������ƃJ�E���g�_�E��
		else if (strchr(mToken, '}')) count--;
	}
}
//���ׂẴt���[���̕`�揈�����Ăяo��
void CModelX::Render()
{
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		mFrame[i]->Render();
	}
}
//�V�F�[�_�[���g�����`��
void CModelX::RenderShader(CMatrix* pCombinedMatrix)
{
	mShader.Render(this, pCombinedMatrix);
}
//�g�[�N�����Ȃ��Ȃ�����true
bool CModelX::EOT()
{
	if (*mpPointer == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
FindFrame(�t���[����)
�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
*/
CModelXFrame* CModelX::FindFrame(char* name)
{
	//�C�e���[�^�̍쐬
	std::vector<CModelXFrame*>::iterator itr;
	//�擪����Ō�܂ŌJ��Ԃ�
	for (itr = mFrame.begin(); itr != mFrame.end(); itr++)
	{
		//���O����v�������H
		if (strcmp(name, (*itr)->mpName) == 0)
		{
			//��v�����炻�̃A�h���X��Ԃ�
			return *itr;
		}
	}
	//��v����t���[�����Ȃ��ꍇ��nullptr��Ԃ�
	return nullptr;
}

/*
FindMaterial(�}�e���A����)
�}�e���A�����ɊO�Ƃ邷��}�e���A���̃A�h���X��Ԃ�*/
CMaterial* CModelX::FindMaterial(char* name)
{
	//�}�e���A���z��̃C�e���[�^�쐬
	std::vector<CMaterial*>::iterator itr;
	//�}�e���A���z���擪���珇�Ɍ���
	for (itr = mMaterial.begin(); itr != mMaterial.end(); itr++)
	{
		//���O����v����΃}�e���A���̃|�C���^��ԋp
		if (strcmp(name, (*itr)->Name()) == 0)
		{
			return *itr;
		}
	}
	//�Ȃ��Ƃ���nullptr��ԋp
	return nullptr;
}

/*
AnimateFrame
�t���[���̕ϊ��s����A�j���[�V�����f�[�^�ōX�V����
*/
void CModelX::AnimateFrame()
{
	//�A�j���[�V�����œK�p�����t���[����
	//�ϊ��s����[���N���A����
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//�d�݂�0�͔�΂�
		if (animSet->mWeight == 0)continue;

		//�t���[����(Animation��)�J��Ԃ�
		for (size_t j = 0; j < animSet->GetAnimation().size(); j++)
		{
			CAnimation* animation = animSet->GetAnimation()[j];
			//�Y������t���[���̕ϊ��s����[���N���A����
			memset(
				&mFrame[animation->mFrameIndex]
				->mTransformMatrix,
				0, sizeof(CMatrix));
		}
	}
	//�A�j���[�V�����ɊY������t���[���̕ϊ��s���
	//�A�j���[�V�����̃f�[�^�Őݒ肷��
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//�d�݂�0�͔�΂�
		if (animSet->mWeight == 0)continue;
		animSet->AnimateMatrix(this);
	}

#ifdef DEBUG
	//�S�t���[������mTransformMatrix�̐ݒ��\��
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		printf("Frame:%s\n", mFrame[i]->mpName);
		for (int j = 0; j < 4; j++)
		{
			printf("%10f %10f %10f %10f\n",
				mFrame[i]->mTransformMatrix.GetM(j, 0),
				mFrame[i]->mTransformMatrix.GetM(j, 1),
				mFrame[i]->mTransformMatrix.GetM(j, 2),
				mFrame[i]->mTransformMatrix.GetM(j, 3));
		}
	}
#endif
}

//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
void CModelX::SetSkinWeightFrameIndex()
{
	//�t���[�������J��Ԃ�
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//���b�V���������
		if (mFrame[i]->mpMesh != nullptr)
		{
			mFrame[i]->mpMesh->SetSkinWeightFrameIndex(this);
		}
	}
}

/*
AnimateVertex
���_�ɃA�j���[�V������K�p����
*/
void CModelX::AnimateVertex()
{
	//�t���[�������J��Ԃ�
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//���b�V���ɖʂ������
		if (mFrame[i]->mpMesh != nullptr)
		{
			//���_���A�j���[�V�����ōX�V����
			mFrame[i]->mpMesh->AnimateVertex(this);
		}
	}
}

//���_�v�Z���w�肵�������s��ōs��
void CModelX::AnimateVertex(CMatrix* mat)
{
	//�t���[�������J��Ԃ�
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//���b�V���������
		if (mFrame[i]->mpMesh)
		{
			//���_���A�j���[�V�����ōX�V����
			mFrame[i]->mpMesh->AnimateVertex(mat);
		}
	}
}

//�A�j���[�V�����𔲂��o��
void CModelX::SeparateAnimationSet(int idx, int start, int end, char* name)
{
	CAnimationSet* anim = mAnimationSet[idx];//��������A�j���[�V�����Z�b�g���m��
	CAnimationSet* as = new CAnimationSet(); //�A�j���[�V�����Z�b�g�̐���
	as->mpName = new char[strlen(name) + 1]; //���O�p�̗̈���m��
	strcpy(as->mpName, name);  //���O���R�s�[
	as->mMaxTime = end - start;//�Đ����Ԃ��v�Z
	//�����̃A�j���[�V�������J��Ԃ�
	for (size_t i = 0; i < anim->mAnimation.size(); i++)
	{
		CAnimation* animation = new CAnimation();//�A�j���[�V�����̐���
		animation->mpFrameName = 
			new char[strlen(anim->mAnimation[i]->mpFrameName) + 1];//�t���[�����p�̗̈���m��
		strcpy(animation->mpFrameName, anim->mAnimation[i]->mpFrameName);//�t���[�����̃R�s�[
		animation->mFrameIndex = anim->mAnimation[i]->mFrameIndex;//�t���[���ԍ�����
		animation->mKeyNum = end - start + 1;//���Ԑ�
		animation->mpKey = new CAnimationKey[animation->mKeyNum];//�A�j���[�V�����L�[�̐���
		animation->mKeyNum = 0;//���Ԑ����[���ɂ���
		//j���I�����Ԉȉ������Ԑ������̊ԌJ��Ԃ�
		//�A�j���[�V�����L�[�R�s�[
		for (int j = start; j <= end && j < anim->mAnimation[i]->mKeyNum; j++)
		{
			if (j < anim->mAnimation[i]->mKeyNum)
			{
				animation->mpKey[animation->mKeyNum] = anim->mAnimation[i]->mpKey[j];
			}
			else
			{
				animation->mpKey[animation->mKeyNum] =
					anim->mAnimation[i]->mpKey[anim->mAnimation[i]->mKeyNum - 1];
			}
			animation->mpKey[animation->mKeyNum].mTime = animation->mKeyNum++;
		}
		as->mAnimation.push_back(animation);//�A�j���[�V�����̒ǉ�
	}
	mAnimationSet.push_back(as);//�A�j���[�V�����Z�b�g�̒ǉ�
}
//�A�j���[�V�����Z�b�g�̒ǉ�
void CModelX::AddAnimationSet(const char* file)
{
	//�t�@�C���T�C�Y���擾
	FILE* fp;//�t�@�C���|�C���^�ϐ��̍쐬
	fp = fopen(file, "rb");//�t�@�C�����I�[�v������
	if (fp == NULL)//�G���[�`�F�b�N
	{
		printf("fopen error:%s\n", file);
		return;
	}
	//�t�@�C���̍Ō�ֈړ�
	fseek(fp, 0L, SEEK_END);
	//�t�@�C���T�C�Y�̎擾
	int size = ftell(fp);

	//�t�@�C���T�C�Y�{1�o�C�g���̗̈���m��
	char* buf = mpPointer = new char[size + 1];
	/*�t�@�C������3D���f���̃f�[�^��ǂݍ���*/
	//�t�@�C���̐擪�ֈړ�
	fseek(fp, 0L, SEEK_SET);
	//�m�ۂ����̈�Ƀt�@�C���T�C�Y���f�[�^��ǂݍ���
	fread(buf, size, 1, fp);
	//�Ō��\0��ݒ肷��(������̏I�[)
	buf[size] = '\0';
	fclose(fp);//�t�@�C�����N���[�Y����

	//������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0')
	{
		GetToken();//�P��̎擾
		//template �ǂݔ�΂�
		if (strcmp(mToken, "template") == 0)
		{
			SkipNode();
		}
		//�P�ꂪAnimationSet�̏ꍇ
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);//�A�j���[�V�����Z�b�g�̐���
		}
	}
	SAFE_DELETE_ARRAY(buf);//�m�ۂ����̈�����
}

//mFrame�z���Ԃ�
std::vector<CModelXFrame*>& CModelX::GetFrames()
{
	return mFrame;

}
//mAnimationSet�z���Ԃ�
std::vector<CAnimationSet*>& CModelX::GetAnimationSet()
{
	return mAnimationSet;
}
//mMaterial�z���Ԃ�
std::vector<CMaterial*>& CModelX::GetMaterial()
{
	return mMaterial;
}

//c����؂蕶���Ȃ�true��Ԃ�
bool CModelX::IsDelimiter(char c)
{
	//c��0��菬�������Afalse��Ԃ�
	if (c < 0)
	{
		return false;
	}
	//isspace(c)
	//c���󔒕����Ȃ�0�ȊO��Ԃ�
	if (isspace(c) != 0)
		return true;
	/*
	strchr(������,����)
	������ɕ������܂܂�Ă����
	�������������ւ̃|�C���^��Ԃ�
	������Ȃ�������NULL��Ԃ�
	*/
	if (strchr(",;\"", c) != NULL)
		return true;
	//��؂蕶���ł͂Ȃ�
	return false;
}



CModelXFrame::CModelXFrame()
	: mpMesh(nullptr)
	, mpName(nullptr)
	, mIndex(0)
{
}

/*
model:CModelX�C���X�^���X�ւ̃|�C���^
�t���[�����쐬����
�ǂݍ��ݒ���Frame��������΁A�t���[�����쐬���A
�q�t���[���ɒǉ�����*/
CModelXFrame::CModelXFrame(CModelX* model)
	: mpName(nullptr)
	, mIndex(0)
	, mpMesh(nullptr)
{
	//���݂̃t���[���z��̗v�f�����擾���ݒ肷��
	mIndex = model->mFrame.size();
	//CModelX�̃t���[���z��ɒǉ�����
	model->mFrame.push_back(this);
	//�ϊ��s���P�ʍs��ɂ���
	mTransformMatrix.SetIdentity();
	//�t���[�������G���A���m�ۂ���
	mpName = new char[strlen(model->mToken) + 1];
	//�t���[�������R�s�[����
	strcpy(mpName, model->mToken);
	//���̒P��i�o�̗\��j���擾����
	model->GetToken();//{
	//�������Ȃ��Ȃ�����I���
	while (*model->mpPointer != '\0')
	{
		//���̒P��擾
		model->GetToken();//Frame
		//}�������̏ꍇ�͏I��
		if (strchr(model->mToken, '}')) break;
		//�V���ȃt���[���̏ꍇ�́A�q�t���[���ɒǉ�
		if (strcmp(model->mToken, "Frame") == 0)
		{
			//�t���[�����擾
			model->GetToken();
			if (strchr(model->mToken, '{'))
			{
				//�t���[�����Ȃ� �X�L�b�v
				model->SkipNode();
				model->GetToken(); // }
			}
			else
			{
				//�t���[�����Ȃ����
				if (model->FindFrame(model->mToken) == 0)
				{
					//�t���[�����쐬���A�q�t���[���̔z��ɒǉ�
					mChild.push_back(new CModelXFrame(model));
				}
			}
		}
		//FrameTransformMatrix�̏ꍇ�A�s��ɒǉ�����
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0)
		{
			model->GetToken();// {
			for (int i = 0; i < mTransformMatrix.GetSize(); i++)
			{
				mTransformMatrix.GetM()[i] = atof(model->GetToken());
			}
			model->GetToken();// }
		}
		//Mesh�̏ꍇ�ACMesh���쐬
		else if (strcmp(model->mToken, "Mesh") == 0)
		{
			mpMesh = new CMesh();
			mpMesh->Init(model);
		}
		else
		{
			//��L�ȊO�̗v�f�͓ǂݔ�΂�
			model->SkipNode();
		}
	}
	//�f�o�b�O�o�[�W�����̂ݗL��
    #ifdef DEBUG
	    printf("%s\n", mpName);
		mTransformMatrix.Print();
    #endif
}

CModelXFrame::~CModelXFrame()
{
	//�q�t���[�������ׂĊJ������
	std::vector<CModelXFrame*>::iterator itr;
	for (itr = mChild.begin(); itr != mChild.end(); itr++)
	{
		delete* itr;
	}
	//���O�̃G���A�������
	SAFE_DELETE_ARRAY(mpName);

	//mpMesh��nullptr�ȊO�Ȃ�C���X�^���X���폜
	if (mpMesh != nullptr)
	{
		delete mpMesh;
	}
}

/*
Render
���b�V�������݂���Ε`�悷��
*/
void CModelXFrame::Render()
{
	if (mpMesh != nullptr)
	{
		mpMesh->Render();
	}
}

int CModelXFrame::GetIndex()
{
	return mIndex;
}

/*
AnimateCombined
�����s��̍쐬
*/
void CModelXFrame::SetAnimateCombined(CMatrix* parent)
{
	//�����̕ϊ��s��ɁA�e����̕ϊ��s����|����
	mCombinedMatrix = mTransformMatrix * (*parent);
	//�q�t���[���̍����s����쐬����
	for (size_t i = 0; i < mChild.size(); i++)
	{
		mChild[i]->SetAnimateCombined(&mCombinedMatrix);
	}

#ifdef DEBUG
	printf("Frame:%s\n", mpName);
	for (int i = 0; i < 4; i++)
	{
		printf("%10f % 10f % 10f % 10f\n",
			mCombinedMatrix.GetM(i, 0),
			mCombinedMatrix.GetM(i, 1),
			mCombinedMatrix.GetM(i, 2), 
			mCombinedMatrix.GetM(i, 3));
	}
#endif
}

const CMatrix& CModelXFrame::GetCombinedMatrix()
{
	return mCombinedMatrix;
}


//CMesh�̃R���X�g���N�^
CMesh::CMesh()
	: mVertexNum(0)
	, mpVertex(nullptr)
	, mFaceNum(0)
	, mpVertexIndex(nullptr)
	, mTempVertexIndex(0)
	, mNormalNum(0)
	, mpNormal(nullptr)
	, mMaterialNum(0)
	, mMaterialIndexNum(0)
	, mpMaterialIndex(nullptr)
	, mpAnimateVertex(nullptr)
	, mpAnimateNormal(nullptr)
	, mpTextureCoords(nullptr)
	, mMyVertexBufferId(0)
{

}

//CMesh�̃f�X�g���N�^
CMesh::~CMesh()
{
	SAFE_DELETE_ARRAY(mpVertex);
	SAFE_DELETE_ARRAY(mpVertexIndex);
	SAFE_DELETE_ARRAY(mpNormal);
	SAFE_DELETE_ARRAY(mpMaterialIndex);
	//�X�L���E�F�C�g�̍폜
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		delete mSkinWeights[i];
	}
	SAFE_DELETE_ARRAY(mpAnimateVertex);
	SAFE_DELETE_ARRAY(mpAnimateNormal);
	SAFE_DELETE_ARRAY(mpTextureCoords);
}

/*
Init
Mesh�̃f�[�^����荞��
*/
void CMesh::Init(CModelX* model)
{
	model->GetToken();// { or ���O
	if (!strchr(model->GetOnlyToken(), '{'))
	{
		//���O�̏ꍇ�A����{
		model->GetToken();// {
	}

	//���_���̎擾
	mVertexNum = atoi(model-> GetToken());
	//���_�����G���A�m��
	mpVertex = new CVector[mVertexNum];
	mpAnimateVertex = new CVector[mVertexNum];
	//���_�����f�[�^����荞��
	for (int i = 0; i < mVertexNum; i++)
	{
		mpVertex[i].SetX(atof(model->GetToken()));
		mpVertex[i].SetY(atof(model->GetToken()));
		mpVertex[i].SetZ(atof(model->GetToken()));
	}

	//�ʐ��ǂݍ���
	mFaceNum = atoi(model->GetToken());
	/*
	//���_����1�ʂ�3���_
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		model->GetToken(); //���_���ǂݔ�΂�
		mpVertexIndex[i] = atoi(model->GetToken());
		mpVertexIndex[i + 1] = atoi(model->GetToken());
		mpVertexIndex[i + 2] = atoi(model->GetToken());
	}
	*/
 
	//�ʐ����J��Ԃ�
	int faceVertexIndex = 0;//��ʓ�����̒��_��
	int allVertexIndex = 0;//�S�Ă̒��_��
	for (int i = 0; i < mFaceNum; i++)
	{
		//��ʓ�����̒��_���擾
		faceVertexIndex = atoi(model->GetToken());
		//���_�����J��Ԃ��ĕۑ�����
		for (int j = 0; j < faceVertexIndex; j++)
		{
			//������ꏊ�ɕۑ����Ă���
			mTempVertexIndex.push_back(atoi(model->GetToken()));
		}
		//�S�Ă̒��_���ۑ��p
		allVertexIndex += faceVertexIndex;
	}
	//���_�����̗̈���m��
	mpVertexIndex = new int[allVertexIndex];
	//�S�Ă̒��_�����J��Ԃ��A�S�Ă̒��_��z��ɒǉ�����
	for (int i = 0; i < allVertexIndex; i++)
	{
		mpVertexIndex[i] = mTempVertexIndex[i];
	}
	
	//�P�ꂪ����ԌJ��Ԃ�
	while (!model->EOT())
	{
		model->GetToken(); //MeshNormals
		// } �������̏ꍇ�͏I��
		if (strchr(model->GetOnlyToken(), '}'))
			break;

		//MeshNormals�̎�
		if (strcmp(model->GetOnlyToken(), "MeshNormals") == 0)
		{
			model->GetToken();// {
			//�@���f�[�^�����擾
			mNormalNum = atoi(model->GetToken());
			//�@���̃f�[�^��z��Ɏ�荞��
			CVector* pNormal = new CVector[mNormalNum];
			for (int i = 0; i < mNormalNum; i++)
			{
				pNormal[i].SetX(atof(model->GetToken()));
				pNormal[i].SetY(atof(model->GetToken()));
				pNormal[i].SetZ(atof(model->GetToken()));
			}

			//�ʐ��ǂݍ���
			mFaceNum = atoi(model->GetToken());
			//�@�������ʐ��~3
			//mNormalNum = atoi(model->GetToken()) * 3;//FaceNum
			//�@�������S�Ă̒��_�̐�
			mNormalNum = allVertexIndex;
			int ni;
			//���_���Ƃɖ@���f�[�^��ݒ肷��
			mpNormal = new CVector[mNormalNum];
			mpAnimateNormal = new CVector[mNormalNum];
			/*for (int i = 0; i < mNormalNum; i += 3)
			{
				faceVertexIndex = atoi(model->GetToken()); // 3 ���_��
				ni = atoi(model->GetToken());
				mpNormal[i] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 1] = pNormal[ni];

				ni = atoi(model->GetToken());
				mpNormal[i + 2] = pNormal[ni];
			}*/

			//�@�������ڂ�������
			int countVertexIndex = 0;
			//�ʐ����J��Ԃ�
			for (int i = 0; i < mFaceNum; i++)
			{
				//��ʓ�����̒��_���擾
				faceVertexIndex = atoi(model->GetToken());
				//���_�����J��Ԃ��ĕۑ�����
				for (int j = 0; j < faceVertexIndex; j++)
				{
					ni = atoi(model->GetToken());
					mpNormal[countVertexIndex] = pNormal[ni];
					countVertexIndex++;
				}
			}
			delete[] pNormal;
			model->GetToken(); // }
		}//End of MeshNormals

		//MeshMaterialList�̂Ƃ�
		else if (strcmp(model->GetOnlyToken(), "MeshMaterialList") == 0)
		{
			model->GetToken(); //{
			//Material�̐�
			mMaterialNum = atoi(model->GetToken());
			//FaceNum
			mMaterialIndexNum = atoi(model->GetToken());
			//�}�e���A���C���f�b�N�X�̍쐬
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++)
			{
				mpMaterialIndex[i] = atoi(model->GetToken());
			}
			//�}�e���A���f�[�^�̍쐬
			for (int i = 0; i < mMaterialNum; i++)
			{
				model->GetToken(); //Material
				if (strcmp(model->GetOnlyToken(), "Material") == 0)
				{
					mMaterial.push_back(new CMaterial(model));
				}
				else
				{
					// { ���o
					model->GetToken(); //MaterialName
					mMaterial.push_back(
						model->FindMaterial(model->GetOnlyToken()));
					model->GetToken(); // }
				}
			}
			model->GetToken(); // }
		}//End of MeshMaterialList

		//SkinWeights�̂Ƃ�
		else if (strcmp(model->GetOnlyToken(), "SkinWeights") == 0)
		{
			//CSkinWeights�N���X�̃C���X�^���X���쐬���A�z��ɒǉ�
			mSkinWeights.push_back(new CSkinWeights(model));
		}//End of SkinWeights

		//�e�N�X�`�����W�̎�
		else if (strcmp(model->GetOnlyToken(), "MeshTextureCoords") == 0)
		{
			model->GetToken(); // {
			//�e�N�X�`�����W�����擾
			int textureCoordsNum = atoi(model->GetToken()) * 2;
			//�e�N�X�`�����W�̃f�[�^��z��Ɏ�荞��
			mpTextureCoords = new float[textureCoordsNum];
			for (int i = 0; i < textureCoordsNum; i++)
			{
				mpTextureCoords[i] = atof(model->GetToken());
			}
			model->GetToken(); // }
		}//End of MeshTextureCoords
		
		else
		{
			//�ȊO�̃m�[�h�͓ǂݔ�΂�
			model->SkipNode();
		}
	}


	//�f�o�b�O�o�[�W�����̂ݗL��
    #ifdef DEBUG
	//���_��
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++)
	{
		printf("%10f %10f %10f\n", 
			mpVertex[i].GetX(), mpVertex[i].GetY(), mpVertex[i].GetZ());
	}
	//�ʐ�
	printf("FaceNum:%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		printf("%3d %3d %3d\n", 
			mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	}
	//�@����
	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i++)
	{
		printf("%12f %12f %12f\n", 
			mpNormal[i].GetX(), mpNormal[i].GetY(), mpNormal[i].GetZ());
	}

    #endif
}

/*
Render
��ʂɕ`�悷��
*/
void CMesh::Render()
{
	/* ���_�f�[�^�A�@���f�[�^�̔z���L���ɂ��� */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	//�e�N�X�`���}�b�s���O�̔z���L���ɂ���
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	/* ���_�f�[�^�A�@���f�[�^,�e�N�X�`�����W�̏ꏊ���w�肷�� */
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);
	glTexCoordPointer(2, GL_FLOAT, 0, mpTextureCoords);

	/* ���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷�� */
	for (int i = 0; i < mFaceNum; i++)
	{
		//�}�e���A����K�p����
		mMaterial[mpMaterialIndex[i]]->Enabled();
		glDrawElements(GL_TRIANGLES, 3,
			GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
		mMaterial[mpMaterialIndex[i]]->Disabled();
	}

	/* ���_�f�[�^�A�@���f�[�^�̔z��𖳌��ɂ��� */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

//�X�L���E�F�C�g�Ƀt���[���ԍ���ݒ�
void CMesh::SetSkinWeightFrameIndex(CModelX* model)
{
	//�X�L���E�F�C�g���J��Ԃ�
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//�t���[�����̃t���[�����擾����
		CModelXFrame* frame = model->FindFrame(mSkinWeights[i]->mpFrameName);
		//�t���[���ԍ���ݒ肷��
		mSkinWeights[i]->mFrameIndex = frame->GetIndex();
	}
}

//���_�ɃA�j���[�V������K�p
void CMesh::AnimateVertex(CModelX* model)
{
	//�A�j���[�V�����p�̒��_�G���A�N���A
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//�X�L���E�F�C�g���J��Ԃ�
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//�t���[���ԍ��擾
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//�I�t�Z�b�g�s��ƃt���[�������s�������
		CMatrix mSkinningMatrix = 
			mSkinWeights[i]->mOffset * model->GetFrames()[frameIndex]->GetCombinedMatrix();
		//���_�����J��Ԃ�
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//���_�ԍ��擾
			int index = mSkinWeights[i]->mpIndex[j];
			//�d�ݎ擾
			float weight = mSkinWeights[i]->mpWeight[j];
			//���_�Ɩ@�����X�V����
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//�@���𐳋K������
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}

void CMesh::AnimateVertex(CMatrix* mat)
{
	//�A�j���[�V�����p�̒��_�G���A�N���A
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//�X�L���E�F�C�g���J��Ԃ�
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//�t���[���ԍ��擾
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//�t���[�������s��ɃI�t�Z�b�g�s�������
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * mat[frameIndex];
		//���_�����J��Ԃ�
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//���_�ԍ��擾
			int index = mSkinWeights[i]->mpIndex[j];
			//�d�ݎ擾
			float weight = mSkinWeights[i]->mpWeight[j];
			//���_�Ɩ@�����X�V����
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}

	}
	//�@���𐳋K��
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}
//���_�o�b�t�@�̍쐬
void CMesh::CreateVertexBuffer()
{
	//���b�V�����ƂɈ��쐬����΂悢
	if (mMyVertexBufferId > 0)
		return;
	if (mVertexNum > 0)
	{
		//���_�C���f�b�N�X���g�킸�A���ׂĂ̖ʃf�[�^���쐬
		CVertex* pmyVertex, * vec;
		//���_���v�Z
		int myVertexNum = mFaceNum * 3;
		//���_�������_�z��쐬
		pmyVertex = new CVertex[myVertexNum];
		vec = new CVertex[mVertexNum];
		for (int j = 0; j < mVertexNum; j++)
		{
			//���_���W�ݒ�
			vec[j].mPosition = mpVertex[j];
			//�e�N�X�`���}�b�s���O�ݒ�
			if (mpTextureCoords != NULL)
			{
				vec[j].mTextureCoords.SetX(mpTextureCoords[j * 2]);
				vec[j].mTextureCoords.SetY(mpTextureCoords[j * 2 + 1]);
			}
			vec[j].mBoneWeight[0] = 1.0f;
		}
		int wi = 0;
		//�X�L���E�F�C�g�ݒ�
		for (size_t k = 0; k < mSkinWeights.size(); k++)
		{
			for (int l = 0; l < mSkinWeights[k]->mIndexNum; l++)
			{
				int idx = mSkinWeights[k]->mpIndex[l];
				for (int m = 0; m < 4; m++)
				{
					if (vec[idx].mBoneIndex[m] == 0)
					{
						vec[idx].mBoneIndex[m] = mSkinWeights[k]->mFrameIndex;
						vec[idx].mBoneWeight[m] = mSkinWeights[k]->mpWeight[l];
						break;
					}
				}
			}
		}
		int k = 0;
		//�}�e���A���ԍ��̏����ɖʂ̒��_��ݒ�
		for (size_t i = 0; i < mMaterial.size(); i++)
		{
			int w = k;
			for (int j = 0; j < mMaterialIndexNum; j++)
			{
				if (mpMaterialIndex[j] == i)
				{
					//���_�z��ɐݒ肵�A�@����ݒ肷��
					pmyVertex[k] = vec[mpVertexIndex[j * 3]];
					pmyVertex[k++].mNormal = mpNormal[j * 3];
					pmyVertex[k] = vec[mpVertexIndex[j * 3 + 1]];
					pmyVertex[k++].mNormal = mpNormal[j * 3 * 1];
					pmyVertex[k] = vec[mpVertexIndex[j * 3 + 2]];
					pmyVertex[k++].mNormal = mpNormal[j * 3 + 2];
				}
			}
			//�}�e���A�����Ƃ̒��_����ǉ�����
			mMaterialVertexCount.push_back(k - w);
		}
		//���_�o�b�t�@�̍쐬
		glGenBuffers(1, &mMyVertexBufferId);
		//���_�o�b�t�@���o�C���h
		glBindBuffer(GL_ARRAY_BUFFER, mMyVertexBufferId);
		//�o�C���h�����o�b�t�@�Ƀf�[�^��]��
		glBufferData(GL_ARRAY_BUFFER
			, sizeof(CVertex) * myVertexNum
			, pmyVertex, GL_STATIC_DRAW);
		//�o�C���h����
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//�z����
		delete[] pmyVertex;
		delete[] vec;
		pmyVertex = nullptr;
	}
}



/*
CSkinWeights
�X�L���E�F�C�g�̓ǂݍ���
*/
CSkinWeights::CSkinWeights(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mIndexNum(0)
	, mpIndex(nullptr)
	, mpWeight(nullptr)
{
	model->GetToken(); // {
	model->GetToken(); //FrameName
	//�t���[�����m�ہA�ݒ�
	mpFrameName = new char[strlen(model->GetOnlyToken()) + 1];
	strcpy(mpFrameName, model->GetOnlyToken());

	//���_�ԍ����擾
	mIndexNum = atoi(model->GetToken());
	//���_�ԍ�����0�𒴂���
	if (mIndexNum > 0)
	{
		//���_�ԍ��ƒ��_�E�F�C�g�̃G���A�m��
		mpIndex = new int[mIndexNum];
		mpWeight = new  float[mIndexNum];
		//���_�ԍ��擾
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = atoi(model->GetToken());
		//���_�E�F�C�g�擾
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = atof(model->GetToken());
	}
	//�I�t�Z�b�g�s��
	for (int i = 0; i < 16; i++)
	{
		mOffset.GetM()[i] = atof(model->GetToken());
	}
	model->GetToken(); // }

	//�f�o�b�O�o�[�W�����̂ݗL��
    #ifdef DEBUG
	printf("SkinWeights %s\n", mpFrameName);
	for (int i = 0; i < mIndexNum; i++)
	{
		printf("%d %f\n",
			mpIndex[i], mpWeight[i]);
	}
	for (int i = 0; i < 16; i += 4)
	{
		printf("%9f %9f %9f %9f\n",
			mOffset.GetM()[i], mOffset.GetM()[i + 1], mOffset.GetM()[i + 2], mOffset.GetM()[i + 3]);
	}
    #endif
}

//CSkinWeights�̃f�X�g���N�^
CSkinWeights::~CSkinWeights()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpIndex);
	SAFE_DELETE_ARRAY(mpWeight);
}

//CAnimationSet�̃f�t�H���g�R���X�g���N�^
CAnimationSet::CAnimationSet()
	: mpName(nullptr)
	, mTime(0)
	, mWeight(0)
	, mMaxTime(0)
{
}
//CAnimationSet�̃R���X�g���N�^
CAnimationSet::CAnimationSet(CModelX* model)
	: mpName(nullptr)
	, mTime(0)
	, mWeight(0)
	, mMaxTime(0)
{
	model->mAnimationSet.push_back(this);
	model->GetToken(); //Animation Name
	//�A�j���[�V�����Z�b�g����ޔ�
	mpName = new char[strlen(model->GetOnlyToken()) + 1];
	strcpy(mpName, model->GetOnlyToken());
	model->GetToken(); // {
	while (!model->EOT())
	{
		model->GetToken(); // } or Animation
		if (strchr(model->GetOnlyToken(), '}'))break;
		if (strcmp(model->GetOnlyToken(), "Animation") == 0)
		{
			//Animation�v�f�ǂݍ���
			mAnimation.push_back(new CAnimation(model));
		}
	}

	//�I������
	mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTime;

	//�f�o�b�O�o�[�W�����̂ݗL��
    #ifdef DEBUG
	printf("AnimationSet:%s\n", mpName);
    #endif
}
//CAnimationSet�̃f�X�g���N�^
CAnimationSet::~CAnimationSet()
{
	SAFE_DELETE_ARRAY(mpName);
	//�A�j���[�V�����v�f�̍폜
	for (size_t i = 0; i < mAnimation.size(); i++)
	{
		delete mAnimation[i];
	}
}
//���Ԃ̐ݒ�
void CAnimationSet::SetTime(float time)
{
	mTime = time;
}
//mTime��Ԃ�
float CAnimationSet::GetTime()
{
	return mTime;
}
//mMaxTime��Ԃ�
float CAnimationSet::GetMaxTime()
{
	return mMaxTime;
}
//�d�݂̐ݒ�
void CAnimationSet::SetWeight(float weight)
{
	mWeight = weight;
}
//�A�j���[�V�����ϊ��s����v�Z����
void CAnimationSet::AnimateMatrix(CModelX* model)
{
	//�d�݂�0�͔�΂�
	if (mWeight == 0)return;
	//�t���[����(Animation��)�J��Ԃ�
	for (size_t j = 0; j < mAnimation.size(); j++)
	{
		//�t���[�����擾����
		CAnimation* animation = mAnimation[j];
		//�L�[���Ȃ��ꍇ���c�̃A�j���[�V������
		if (animation->mpKey == nullptr)continue;
		//�Y������t���[���̎擾
		CModelXFrame* frame = model->mFrame[animation->mFrameIndex];
		//�ŏ��̎��Ԃ�菬�����ꍇ
		if (mTime < animation->mpKey[0].mTime)
		{
			//�ϊ��s���0�R�}�ڂ̍s��ōX�V
			frame->mTransformMatrix += animation->mpKey[0].mMatrix * mWeight;
		}
		//�Ō�̎��Ԃ��傫���ꍇ
		else if (mTime >= animation->mpKey[animation->mKeyNum - 1].mTime)
		{
			//�ϊ��s����Ō�̃R�}�̍s��ōX�V
			frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * mWeight;
		}
		else
		{
			//���Ԃ̓r���̏ꍇ
			for (int k = 1; k < animation->mKeyNum; k++)
			{
				//�ϊ��s����A���`��ԂɂčX�V
				if (mTime < animation->mpKey[k].mTime &&
					animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime)
				{
					float r = (animation->mpKey[k].mTime - mTime) /
						(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);

					frame->mTransformMatrix += (animation->mpKey[k - 1].mMatrix * r +
						animation->mpKey[k].mMatrix * (1 - r)) * mWeight;
					break;
				}
			}
		}
	}
}
//mAnimation��Ԃ�
std::vector<CAnimation*>& CAnimationSet::GetAnimation()
{
	return mAnimation;
}

//CAnimation�̃f�t�H���g�R���X�g���N�^
CAnimation::CAnimation()
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
}
//CAnimation�̃R���X�g���N�^
CAnimation::CAnimation(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
	model->GetToken(); // { or Animation Name
	if (strchr(model->GetOnlyToken(), '{'))
	{
		model->GetToken(); // {
	}
	else
	{
		model->GetToken(); // {
		model->GetToken(); // {
	}

	model->GetToken(); //FrameName
	mpFrameName = new char[strlen(model->GetOnlyToken()) + 1];
	strcpy(mpFrameName, model->GetOnlyToken());
	mFrameIndex =
		model->FindFrame(model->GetOnlyToken())->GetIndex();
	model->GetToken(); // }

	//�L�[�̔z���ۑ����Ă����z��
	CMatrix* key[4] = { nullptr,nullptr,nullptr,nullptr };
	//���Ԃ̔z���ۑ����Ă����z��
	float* time[4] = { nullptr,nullptr,nullptr,nullptr };

	while (!model->EOT())
	{
		model->GetToken();// } or AnimationKey
		if (strchr(model->GetOnlyToken(), '}'))break;
		if (strcmp(model->GetOnlyToken(), "AnimationKey") == 0)
		{
			model->GetToken(); // {
			//�f�[�^�̃^�C�v�擾
			int type = atoi(model->GetToken());
			//���Ԑ��擾
			mKeyNum = atoi(model->GetToken());

			switch (type)
			{
			case 0: //Rotation Quaternion
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for (int i = 0; i < mKeyNum; i++)
				{
					//���Ԏ擾
					time[type][i] = atof(model->GetToken());
					model->GetToken(); //4��ǂݔ�΂�
					//w,x,y,z���擾
					float w = atof(model->GetToken());
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//�N�H�[�^�j�I�������]�s��ɕϊ�
					key[type][i].Quaternion(x, y, z, w);
				}
				break;
			case 1: //�g��E�k���̍s��쐬
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for (int i = 0; i < mKeyNum; i++)
				{
					//���Ԏ擾
					time[type][i] = atof(model->GetToken());
					model->GetToken(); //3��ǂݔ�΂�
					//x,y,z���擾
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//�g��E�k���s��ɕϊ�
					key[type][i].SetScale(x, y, z);
				}
				break;
			case 2: //�ړ��̍s��쐬
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for (int i = 0; i < mKeyNum; i++)
				{
					//���Ԏ擾
					time[type][i] = atof(model->GetToken());
					model->GetToken(); //3��ǂݔ�΂�
					//x,y,z���擾
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//�ړ��s��ɕϊ�
					key[type][i].SetTranlate(x, y, z);
				}
				break;
			case 4: //�s��f�[�^���擾
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					mpKey[i].mTime = atof(model->GetToken());//Time
					model->GetToken(); //16
					for (int j = 0; j < 16; j++)
					{
						mpKey[i].mMatrix.GetM()[j] = atof(model->GetToken());
					}
				}
				break;
			}
			model->GetToken(); // }
		}
		else 
		{
			model->SkipNode();
		}
	} //while�̏I���

	//�s��f�[�^�ł͂Ȃ��Ƃ�
	if (mpKey == nullptr)
	{
		//���Ԑ����L�[���쐬
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++)
		{
			//���Ԑݒ�
			mpKey[i].mTime = time[2][i];//Time
			//�s��쐬 Scale * Rotation * Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
		}
	}
	//�m�ۂ����G���A�J��
	for (int i = 0; i < ARRAY_SIZE(key); i++)
	{
		SAFE_DELETE_ARRAY(time[i]);
		SAFE_DELETE_ARRAY(key[i]);
	}

	//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef DEBUG
	printf("Animation:%s\n", mpFrameName);
	for (int i = 0; i < 16; i += 4)
	{
		printf("%9f %9f %9f %9f\n",
			mpKey[1].mMatrix.GetM()[i], mpKey[1].mMatrix.GetM()[i + 1], mpKey[1].mMatrix.GetM()[i + 2], mpKey[1].mMatrix.GetM()[i + 3]);
	}
#endif
}
//CAnimation�̃f�X�g���N�^
CAnimation::~CAnimation()
{
	SAFE_DELETE_ARRAY(mpFrameName);
	SAFE_DELETE_ARRAY(mpKey);
}