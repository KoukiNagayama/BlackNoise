#pragma once
#include "sound/SoundSource.h"
class GameCamera;
class Gramophone : public IGameObject
{
public:

	enum enGramophoneState
	{
		enGramophoneState_Idle,
		enGramophoneState_Play
	};

	Gramophone() {};
	~Gramophone() {};
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
	const int GetState() const
	{

	}
private:
	/// <summary>
	/// ����炷
	/// </summary>
	void MakeSound();
	/// <summary>
	/// �v���C���[�Ɖ����̋����ɂ�艹�̑傫���𒲐�����
	/// </summary>
	float SoundLevelByDistance();
	/// <summary>
	/// ���ʒ���
	/// </summary>
	void VolumeControl();
	/// <summary>
	/// �X�e�[�g�Ǘ�
	/// </summary>
	void ManegeState();

	void IdleState();
	void PlayState();

	void Font();
private:
	enGramophoneState		m_gramophoneState = enGramophoneState_Idle;	// �X�e�[�g
	Vector3					m_position;									// ���W
	Vector3					m_scale;									// �g�嗦
	Quaternion				m_rotation = Quaternion::Identity;			// ��]
	ModelRender				m_modelRender;								// ���f�������_�[
	float					m_rateByTime1 = 1.00f;						// ���Ԃɂ��e����
	SoundSource*			m_soundSource = nullptr;					// ����
	GameCamera*				m_gameCamera;								// �Q�[���J����
	FontRender				m_font;
};

