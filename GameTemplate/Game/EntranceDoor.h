#pragma once
class GroundFloor;
class EntranceDoor : public IGameObject
{
public:
	EntranceDoor() {};
	~EntranceDoor() {};

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
	void OpenDoor();
private:
	// �A�j���[�V�����N���b�v
	enum EnAnimationClip {	
		enAnimationClip_Idle,
		enAnimationClip_Open,			// �J��
		enAnimationClip_Num				// ��
	};
	ModelRender				m_modelRender;						// ���f�������_�[
	Vector3					m_position;							// ���W
	Vector3					m_scale;							// �g�嗦
	Quaternion				m_rotation;							// ��]
	GroundFloor*			m_groundFloor;
	AnimationClip			m_animationClips[enAnimationClip_Num];
	bool					m_isOpened = false;
};

