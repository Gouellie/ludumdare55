#pragma once

#include <gameplay/buyable.h>

#include <string>

enum class WarriorStatus
{
    Waiting = 0,
    Dispatched = 1,
    Healing = 2,
    Dead = 3
};

class Warrior : public Buyable
{
public:
    Warrior() = default;
    Warrior(char* name, int health, int power)
        : m_Name(name), m_Health{ health }, m_MaxHealth{ health }, m_PowerLevel {
        power
    } {}

    void GetData();

    char* GetName() { return m_Name; }
    const char* GetName() const { return m_Name; }
    const WarriorStatus GetStatus() { return m_Status; }
    void SetStatus(WarriorStatus newStatus) { m_Status = newStatus; }
    const int GetHealth() const { return m_Health; }
    void SetHealth(int newVal);
    const int GetMaxHealth() const { return m_MaxHealth; }
    const int GetPowerLevel() const { return m_PowerLevel; }
    void SetPowerLevel(int newVal) { m_PowerLevel = newVal; }
    void TakeDamage(int dmg);

    void SetIndex(int idx) { m_Index = idx; }
    int GetIndex() { return m_Index; }

    const int GetPrice() const override { return WARRIOR_COST; }

private:
    char* m_Name;
    WarriorStatus m_Status{ WarriorStatus::Waiting };
    int m_Health{ 100 };
    int m_MaxHealth{ 100 };
    int m_PowerLevel{ 100 };
    int m_Index{ 0 };
    static constexpr int WARRIOR_COST{ 50 };
};