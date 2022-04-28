#pragma once
class GameCamera;
class GroundFloor;
class Key : public IGameObject
{
public:
	Key() {};
	~Key() {};
	bool Start();
	void Update();
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
	/// �g�嗦�̐ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]�̐ݒ�
	/// </summary>
	/// <param name="rot">�N�H�[�^�j�I��</param>
	void SetRotation(Quaternion& rot)
	{
		m_rotation = rot;
	}
	/// <summary>
	/// �s�[�X���E������
	/// </summary>
	void PickPiece()
	{
		m_havePiece += 1;
	}
private:
	/// <summary>
	/// �����E������
	/// </summary>
	//void PickKey();

	GameCamera* m_gamecam = nullptr;	//�Q�[���J����
	GroundFloor* m_groundfloor;			//��K
	Vector3 m_position;					//���W
	Vector3 m_scale;					//�g�嗦
	Quaternion m_rotation;				//��]
	ModelRender m_modelRender;			//���f��
	int m_havePiece = 0;				//�s�[�X�������W�߂���
};

