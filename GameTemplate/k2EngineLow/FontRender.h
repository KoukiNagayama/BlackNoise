#pragma once

namespace nsK2EngineLow {

	// �������T�C�Y
	const int MAX_TEXT_SIZE = 256;


	class FontRender
	{
	public:
		// �t�H���g�\����
		struct SFont {
			wchar_t		s_text[MAX_TEXT_SIZE];
			Vector2		s_position;
			float		s_scale = 1.0f;
			Vector4		s_color = g_vec4White;;
			float		s_rotation = 0.0f;
			Vector2		s_pivot = Sprite::DEFAULT_PIVOT;
		};


		~FontRender() {};
		/// <summary>
		/// �\�����镶����ݒ�
		/// </summary>
		/// <param name="text">�\�����镶��</param>
		void SetText(wchar_t* text)
		{
			swprintf_s(m_text, text);
		}
		/// <summary>
		/// �\�����镶�����擾
		/// </summary>
		/// <returns>�\�����镶��</returns>
		const wchar_t* GetText() const
		{
			return m_text;
		}
		/// <summary>
		/// ���W��ݒ�Bz��0.0f
		/// </summary>
		/// <param name="x">X���W</param>
		/// <param name="y">Y���W</param>
		/// <param name="z">Z���W</param>
		void SetPosition(float x, float y, float z)
		{
			SetPosition({ x,y,z });
		}
		/// <summary>
		/// ���W��ݒ�
		/// </summary>
		/// <param name="position">���W</param>
		void SetPosition(const Vector3& position)
		{
			m_position = position;
		}
		/// <summary>
		/// ���W���擾
		/// </summary>
		/// <returns>���W</returns>
		const Vector3& GetPosition() const
		{
			return m_position;
		}
		/// <summary>
		/// �傫����ݒ�
		/// </summary>
		/// <param name="scale">�傫��</param>
		void SetScale(const float scale)
		{
			m_scale = scale;
		}
		/// <summary>
		/// �傫�����擾
		/// </summary>
		/// <returns>�傫��</returns>
		const float GetScale() const
		{
			return m_scale;
		}
		/// <summary>
		/// �F��ݒ�
		/// </summary>
		/// <param name="r">��</param>
		/// <param name="g">��</param>
		/// <param name="b">��</param>
		/// <param name="a">�����x</param>
		void SetColor(float r, float g, float b, float a)
		{
			SetColor({ r, g, b, a });
		}
		/// <summary>
		/// �F��ݒ�
		/// </summary>
		/// <param name="color">�F</param>
		void SetColor(const Vector4& color)
		{
			m_color = color;
		}
		/// <summary>
		/// �F���擾
		/// </summary>
		/// <returns>�F</returns>
		const Vector4& GetColor() const
		{
			return m_color;
		}
		/// <summary>
		/// ��]��ݒ�
		/// </summary>
		/// <param name="rotation">��]</param>
		void SetRotation(const float rotation)
		{
			m_rotation = rotation;
		}
		/// <summary>
		/// ��]���擾
		/// </summary>
		/// <returns>��]</returns>
		const float GetRotation() const
		{
			return m_rotation;
		}
		/// <summary>
		/// �s�{�b�g��ݒ�
		/// </summary>
		/// <param name="x">�s�{�b�g��X���W</param>
		/// <param name="y">�s�{�b�g��Y���W</param>
		void SetPivot(float x, float y)
		{
			SetPivot({ x, y });
		}
		/// <summary>
		/// �s�{�b�g��ݒ�
		/// </summary>
		/// <param name="pivot">
		///	�s�{�b�g
		/// x = 0.5, y = 0.5�ŉ摜�̒��S����_
		/// x = 0.0, y = 0.0�ŉ摜�̍���
		/// x = 1.0, y = 1.0�ŉ摜�̉E��
		/// Unity��uGUI�ɏ���
		/// </param>
		void SetPivot(const Vector2& pivot)
		{
			m_pivot = pivot;
		}
		/// <summary>
		/// �s�{�b�g���擾
		/// </summary>
		/// <returns>�s�{�b�g</returns>
		const Vector2& GetPivot() const
		{
			return m_pivot;
		}
		/// <summary>
		/// �`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void Draw(RenderContext& rc);
		/// <summary>
		/// �e�̃p�����[�^��ݒ�
		/// </summary>
		/// <param name="isDrawShadow">�e��`�悷��</param>
		/// <param name="shadowOffset">�e��`�悷��Ƃ��̃s�N�Z���̃I�t�Z�b�g��</param>
		/// <param name="shadowColor">�e�̐F</param>
		void SetShadowParam(bool isDrawShadow, float shadowOffset, const Vector4& shadowColor)
		{
			m_font.SetShadowParam(isDrawShadow, shadowOffset, shadowColor);
		}
	private:
		/// <summary>
		/// �����̕`��
		/// </summary>
		/// <param name="rc">�����_�����O�R���e�L�X�g</param>
		void RenderFont(RenderContext& rc);

	private:
		wchar_t		m_text[MAX_TEXT_SIZE];
		Vector3		m_position = Vector3::Zero;
		float		m_scale = 1.0f;
		Vector4		m_color = g_vec4White;;
		float		m_rotation = 0.0f;
		Vector2		m_pivot = Sprite::DEFAULT_PIVOT;
		Font		m_font;
		std::vector<SFont*> m_fontArray;
		int			m_initCount = 0;
		bool		m_initCheck = false;
		
		SFont		m_sfont;

	};
}


