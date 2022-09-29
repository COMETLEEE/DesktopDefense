#pragma once
#include "ComponentBase.h"

#include "Vector2D.h"
#include "Weapon.h"
#include "AudioClip.h"

using namespace Engine;

class Player : public ComponentBase
{
public:

	Player(GameObject* _gameObject);

	void Tick() override;

	void Initialize();

	bool AttackTick();

	bool m_IsAttacking;

	void ChangeWeaponTick();

	int m_SelectWeapon;

	static constexpr int SLOT_COUNT = 5;

	GameObject* m_WeaponObjectsSlot[SLOT_COUNT];

	inline Weapon* GetEquippedWeapon() const {
		if (m_WeaponObjectsSlot[m_SelectWeapon] == nullptr)
			return nullptr;

		return m_WeaponObjectsSlot[m_SelectWeapon]->GetComponent<Weapon>();
	}
};