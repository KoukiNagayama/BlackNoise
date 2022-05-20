#pragma once
#include "sound/SoundSource.h"
class Title;
class TitlePiano : public IGameObject
{
public:
	TitlePiano() {};
	~TitlePiano() {};

	/// <summary>
	/// ����������
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	/// <param name="position">���W</param>
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
private:
	/// <summary>
	/// ����炷
	/// </summary>
	void MakeSound();
	/// <summary>
	/// �e������ς���
	/// </summary>
	void ChangeRate();

private:
	Vector3			m_position;				// ���W
	SoundSource*	m_soundSource;			// ����
	float			m_rateByTime;			// ���Ԃɂ��e����
	int				m_count;				// �s�A�m��������
	Title*			m_title;				// �^�C�g��
};

