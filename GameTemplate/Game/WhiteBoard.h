#pragma once
class WhiteBoard : public IGameObject
{
public:
	WhiteBoard() {};
	~WhiteBoard() {};
	/// <summary>
	/// ����������
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// �`�揈��
	/// </summary>
	void Render(RenderContext& rc);
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// �g�嗦��ݒ�
	/// </summary>
	/// <param name="scale">�g�嗦</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	/// <param name="rotation">��]</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
private:
	ModelRender					m_modelRender;					// ���f�������_�[
	Vector3						m_position;						// ���W
	Quaternion					m_rotation;						// ��]
	Vector3						m_scale;						// �g�嗦
};

