#ifndef CMODEL_H
#define CMODEL_H
#include <vector>
#include "CTriangle.h"
#include "CMaterial.h"
#include "CVertex.h"
/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
public:
	const std::vector<CTriangle>& Triangles() const;
	//�`��
	//Render(�s��)
	void Render(const CMatrix& m);
	~CModel();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����,�}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	//�`��
	void Render();

private:
	//���_���̔z��
	CVertex* mpVertexes;
	void CreateVertexBuffer();
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle>mTriangles;
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
};

#endif
