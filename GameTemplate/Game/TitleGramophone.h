#pragma once
#include "sound/SoundSource.h"

class Title;
class TitleCamera;
class TitleGramophone : public IGameObject
{
public:

	enum enGramophoneState
	{
		enGramophoneState_Idle,
		enGramophoneState_Play
	};

	TitleGramophone() {};
	~TitleGramophone();
	bool Start();
	void Update();
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
	/// <summary>
	/// �I�u�W�F�N�g�̔ԍ���ݒ�
	/// </summary>
	/// <param name="number">�ԍ�</param>
	void SetNumber(const int& number)
	{
		m_number = number;
	}
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
private:
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// ����炷
	/// </summary>
	void MakeSound();
	/// <summary>
	/// �v���C���[�Ɖ����̋����ɂ�艹�̑傫���𒲐�����
	/// </summary>
	/// <param name="range">�����̉e���͈�</param>
	float SoundLevelByDistance(float range);
	/// <summary>
	/// ���ʒ���
	/// </summary>
	void VolumeControl();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManegeState();
	/// <summary>
	/// �ҋ@���̓���
	/// </summary>
	void IdleState();
	/// <summary>
	/// ���t���̓���
	/// </summary>
	void PlayState();
	/// <summary>
	/// �e����
	/// </summary>
	void ChangeRate();

private:
	enGramophoneState		m_gramophoneState = enGramophoneState_Idle;	// �X�e�[�g
	Vector3					m_position;									// ���W
	Vector3					m_scale = Vector3::One;						// �g�嗦
	Quaternion				m_rotation = Quaternion::Identity;			// ��]
	ModelRender				m_modelRender;								// ���f�������_�[
	float					m_rateByTime;								// ���Ԃɂ��e����
	SoundSource*			m_soundSource1 = nullptr;					// ����1
	int						m_number;									// �I�u�W�F�N�g�̔ԍ�
	int						m_recordNum;
	TitleCamera*			m_titleCamera;								// �^�C�g���J����
	Title*					m_title;
};

