#pragma once

class GameCamera;
class BrokenDoor;

class Hammer:public IGameObject
{
public:
	Hammer() {};
	~Hammer() {};
	/// <summary>
	/// ����������
	/// </summary>
	/// <returns>true</returns>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="rc">�����_�[�R���e�L�X�g</param>
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W�̐ݒ�
	/// </summary>
	/// <param name="pos">���W</param>
	void SetPosition(Vector3& pos)
	{
		m_position = pos;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rot">�N�H�[�^�j�I��</param>
	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}

private:
	Vector3			m_disToPlayer;			//�v���C���[�Ƃ̋���
	Vector3			m_position;				//���W
	Quaternion		m_rotation;				//��]
	ModelRender		m_modelRender;			//���f��
	GameCamera*		m_gamecam;				//�Q�[���J����
	BrokenDoor*		m_b_door;				//�u���[�N���h�A
};

