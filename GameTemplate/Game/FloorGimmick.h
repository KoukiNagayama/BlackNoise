#pragma once
class Item;
class GameCamera;
class FloorGimmick : public IGameObject
{
public:
	FloorGimmick() {};
	~FloorGimmick() {};
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
	/// �h�A���󂹂鋗���ɂ��邩
	/// </summary>
	void NearGimmick();
	void SetPosition(const Vector3& position)
	{
		m_position = position;
	}
	/// <summary>
	/// ��]��ݒ肷��B
	/// </summary>
	/// <param name="rotation">��]�B</param>
	void SetRotation(const Quaternion& rotation)
	{
		m_rotation = rotation;
	}
	/// <summary>
	/// �傫����ݒ肷��B
	/// </summary>
	/// <param name="scale">�傫���B</param>
	void SetScale(const Vector3& scale)
	{
		m_scale = scale;
	}

private:
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̐���
	/// </summary>
	void CreatePhysicsObject();
	/// <summary>
	/// �t�B�W�b�N�X�I�u�W�F�N�g�̊J��
	/// </summary>
	void ReleasePhysicsObject();
	Vector3 m_position;	//���W
	Vector3 m_scale;	//�g�嗦
	Quaternion m_rotation;	//��]
	ModelRender m_modelRender;	//���f��

	PhysicsStaticObject	m_physicsStaticObject;	//�t�B�W�N�X�X�^�e�B�b�N�I�u�W�F�N�g�B	
	int m_attack = 2;	//����܂łɕK�v�ȍU����

	Item* m_item = nullptr;	//�A�C�e���N���X
	GameCamera* m_gamecam = nullptr;	//�Q�[���J����
};

