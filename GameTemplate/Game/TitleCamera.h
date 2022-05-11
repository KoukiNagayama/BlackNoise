#pragma once
class TitleText;
class TitleCamera : public IGameObject
{
public:
	TitleCamera() {};
	~TitleCamera() {};

	bool Start();
	void Update();
	/// <summary>
	/// ���_��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_cameraPos = position;
	}
private:
	Vector3				m_cameraPos;			// ���W(���_)	
	Vector3				m_toCameraPos;			// �����_���王�_�Ɍ������x�N�g��
	TitleText*			m_tText;
};

