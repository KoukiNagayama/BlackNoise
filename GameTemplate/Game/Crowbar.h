#pragma once
class Item;
class GameCamera;
class UI;
class Crowbar : public IGameObject
{
public:
	Crowbar() {};
	~Crowbar() {};
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
	/// <param name="position">���W</param>
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
	/// <summary>
	/// ���W���擾
	/// </summary>
	/// <returns>���W</returns>
	const Vector3 GetPosition()
	{
		return m_position;
	}
	/// <summary>
	/// �o�[�����Ƃ�邩
	/// </summary>
	//void CanPickCrowbar();
private:
	Vector3 m_position;						//���W
	Vector3 m_scale;						//�g�嗦
	Quaternion m_rotation;					//��]

	ModelRender m_modelRender;				//���f��

	Item* m_item = nullptr;					//�A�C�e��
	GameCamera* m_gamecam = nullptr;		//�Q�[���J����
	UI* m_ui = nullptr;						//UI
};

