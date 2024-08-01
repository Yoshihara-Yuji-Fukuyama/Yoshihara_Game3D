#ifndef CRECTANGLE_H
#define CRECTANGLE_H

class CRectangle
{
public:
	//�R���X�g���N�^
	CRectangle();
	//�f�X�g���N�^
	~CRectangle();
	//�`��
	void Render();

	/// <summary>
	/// �l�p�`�̕`��
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	void Render(float x, float y, float w, float h);

	/// <summary>
	/// �傫���̐ݒ�
	/// </summary>
	/// <param name="x">X���W</param>
	/// <param name="y">Y���W</param>
	/// <param name="w">��</param>
	/// <param name="h">����</param>
	void Set(float x, float y, float w, float h);

	//X���W�̎擾
	float GetX();
	//Y���W�̎擾
	float GetY();
	//Z���W�̎擾
	float GetZ();
	//���̎擾
	float GetW();
	//�����̎擾
	float GetH();

	/// <summary>
	/// //Y���W�̐ݒ�
	/// </summary>
	/// <param name="y">Y���W�̕ύX�l</param>
	void SetY(float y);
	/// <summary>
	/// //X���W�̐ݒ�
	/// </summary>
	/// <param name="x">X���W�̕ύX�l</param>
	void SetX(float x);
	/// <summary>
	/// //Z���W�̐ݒ�
	/// </summary>
	/// <param name="x">Z���W�̕ύX�l</param>
	void SetZ(float z);

	/// <summary>
	/// ���̐ݒ�
	/// </summary>
	/// <param name="w">��</param>
	void SetW(float w);
	/// <summary>
	/// �����̐ݒ�
	/// </summary>
	/// <param name="h">����</param>
	void SetH(float h);

protected:
	float mX; //X���W
	float mY; //Y���W
	float mZ; //����

private:
	float mW; //��
	float mH; //����

};

#endif