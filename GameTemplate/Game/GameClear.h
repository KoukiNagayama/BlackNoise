#pragma once
class WhiteBoard;
class GameClear : public IGameObject
{
public:
	GameClear() {};
	~GameClear() {};
	/// <summary>
	/// ����������
	/// </summary>
	bool Start();
	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();
private:

};

