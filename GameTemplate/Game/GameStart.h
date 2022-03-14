#pragma once
class Player;
class GameCamera;
class BackGround;

class GameStart : public IGameObject
{
public:
	GameStart();
	~GameStart();
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:
	Player* m_player;
	GameCamera* m_gamecam;
	BackGround* m_bg;
};

