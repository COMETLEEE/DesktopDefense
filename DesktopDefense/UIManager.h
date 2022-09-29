#pragma once
#include "ComponentBase.h"
#include "Sprite.h"
#include "Vector2D.h"
#include "Player.h"
#include "Sprite.h"
#include <map>
#include <string>

using namespace Engine;

class UIManager : public ComponentBase
{
public:
	UIManager(GameObject* _gameObject);

	void Tick() override;

	// GameStart 후 몇 초 지났는지 표시하는 함수
	void GameTimeTick();

	// 0. 초기화
	void Initialize(Player* _Player);

	// 1. 인벤토리 UI를 그리고, 무기를 획득할 경우 아이콘 생성하는 함수
	void InventoryTick();

	// 2. 현재 선택한 무기에 따라 커서를 변경시키기 위한 함수
	void ChangeWeaponCursor();
	
	void MoveToMouseCursorTick();

	// inline Vector2D GetInventoryPosition() const { return this->m_InventoryPosition; }
	
private:
	// 인벤토리 Ui의 Position
	Vector2D m_InventoryPosition;

	// 인벤토리 Ui 내 무기 아이콘의 Position
	Vector2D m_WeaponIconPosition;

	// 플레이어에게서 인벤토리 정보 및 현재 사용중인 무기를 얻기 위함
	Player* m_Player;

	// 인벤토리 스프라이트
	Sprite* m_Select_WeaponSprite[5];

	// 인벤토리 UI를 Render하기 위한 오브젝트
	GameObject* m_InventoryObject;

	// 무기 커서 스프라이트
	Sprite* m_WeaponCursorSprite[5];

	// 무기 커서 공격 시 스프라이트
	Sprite* m_WeaponCursorAttackSprite[5];

	// 무기 커서를 Render할 오브젝트
	GameObject* m_WeaponCursorObject;

	// 무기 커서 공격시의 스프라이트를 Render할 오브젝트
	GameObject* m_WeaponCursorAttackObject;

	Vector2D m_MousePosition;

	// 현재까지 몇 초 버텼는지 출력하기 위한 스프라이트들의 모임
	vector<Sprite*> m_NumbersForTime;
};