#ifndef CMATERIAL_H
#define CMATERIAL_H
#define MATERIAL_NAME_LEN 64//���O�̒���

#include "CTexture.h"
#include "CModelX.h"
class CModelX;  //CModelX�N���X�̐錾
class CMyShader;//CMyShader�N���X�̐錾

/*
�}�e���A���N���X
�}�e���A���̃f�[�^������
*/
class CMaterial
{
	friend CMyShader;
public:
	CMaterial();
	CMaterial(CModelX* model);
	~CMaterial();

	//���_���̐ݒ�
	//VertexNum(���_��)
	void VertexNum(int num);
	//���_���̎擾
	int VertexNum();
	//�}�e���A����L���ɂ���
	void Enabled();
	//�}�e���A���̖��O�̎擾
	char* Name();
	//�}�e���A���̖��O��ݒ肷��
	//Name(�}�e���A���̖��O)
	void Name(char* name);
	//mDiffuse�z��̎擾
	float* Diffuse();
	//�}�e���A���𖳌��ɂ���
	void Disabled();
	//�e�N�X�`���̎擾
	CTexture* GetTexture();
private:
	//�}�e���A�����̒��_��
	int mVertexNum;
	//�}�e���A����
	char mName[MATERIAL_NAME_LEN + 1];
	//�g�U���̐FRGBA
	float mDiffuse[4];
	//�e�N�X�`��
	CTexture mTexture;
	float mPower;
	float mSpecular[3];
	float mEmissive[3];
	//�e�N�X�`���t�@�C����
	char* mpTextureFilename;
};

#endif 