#include "GameInstanceBase.h"
#include <vector>
#include "DesktopDefense.h"
#define FIRST_QUADRANT 1
#define SECOND_QUADRANT 2
#define THIRD_QUADRANT 3
#define FOURTH_QUADRANT 4


using namespace Engine;

class DesktopDefenseGameInstance : public GameInstanceBase
{
public:
	explicit DesktopDefenseGameInstance(HWND _hWnd);

	~DesktopDefenseGameInstance() override;
	void OnMiner();

//protected:
	bool GameInstanceTick() override;
	void CreateMiner();
	void OffMiner(int mineridx);
	Vector2D RandomPosition();

	//private:
	int m_Quadrant;
	int m_RespawnX;
	int m_RespawnY;
	Sprite* m_TestSprite;
	GameObject* m_TestGameObject;
	Sprite* m_ColliderTestSpritesOne;
	GameObject* m_ColliderTestGameObjectOne;
	Sprite* m_ColliderTestSpritesTwo;
	GameObject* m_ColliderTestGameObjectTwo;
	GameObject* m_ButtonOne;
	std::vector<GameObject*> m_VectorMiner;
	std::set<int> m_DisabledMinerSet;
	GameObject* m_Miner;
	GameObject* m_UIManager;
};