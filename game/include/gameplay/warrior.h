#pragma once

#include <gameplay/buyable.h>

enum class WarriorStatus
{
    Waiting = 0,
    Dispatched = 1,
    Healing = 2,
    Dead = 3
};

class SettlementComponent;

class Warrior : public Buyable
{
public:
    Warrior() = default;
    Warrior(const char* name, int health, int power)
        : m_Name(name), m_Health{ health }, m_MaxHealth{ health }, m_PowerLevel {
        power
    } {}

    const char* GetName() { return m_Name; }
    const char* GetName() const { return m_Name; }
    const WarriorStatus GetStatus() { return m_Status; }
    void SetStatus(WarriorStatus newStatus);
    const int GetHealth() const { return m_Health; }
    void SetHealth(int newVal);
    const int GetMaxHealth() const { return m_MaxHealth; }
    void SetMaxHealth(int newMaxHealth) { m_MaxHealth = newMaxHealth; }
    const int GetPowerLevel() const { return m_PowerLevel; }
    void SetPowerLevel(int newVal) { m_PowerLevel = newVal; }
    void TakeDamage(int dmg);

    void SetAssignedSettlement(SettlementComponent* newAssignement) { m_AssignedSettlement = newAssignement; }

    void LevelUp();

    void SetIndex(int idx) { m_Index = idx; }
    int GetIndex() { return m_Index; }

    const int GetPrice() const override { return WARRIOR_COST; }

private:
    const char* m_Name;
    WarriorStatus m_Status{ WarriorStatus::Waiting };
    int m_Health{ 100 };
    int m_MaxHealth{ 100 };
    int m_PowerLevel{ 100 };
    int m_Index{ 0 };
    SettlementComponent* m_AssignedSettlement = nullptr;

    static constexpr int WARRIOR_COST{ 50 };

    static constexpr int HEALTH_LEVEL_UP_GAIN{ 15 };
    static constexpr int POWER_LEVEL_LVL_UP_GAIN{ 25 };
};