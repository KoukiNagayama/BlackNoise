#pragma once
class WhiteBoard;
class GameClear : public IGameObject
{
public:
	GameClear() {};
	~GameClear() {};
	/// <summary>
	/// 初期化処理
	/// </summary>
	bool Start();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
private:

};

