#pragma once

#include <string>

enum class WarriorStatus
{
    Waiting = 0,
    Dispatched = 1,
    Healing = 2,
    Dead = 3
};

class Warrior
{
public:
    std::string m_Name;
    WarriorStatus m_Status{ WarriorStatus::Waiting };

    Warrior() = default;
    Warrior(std::string name, unsigned int health, unsigned int power)
        : m_Name(name), m_Health{ health }, m_PowerLevel{ power } {}

    void GetData();

    const WarriorStatus GetStatus() { return m_Status; }
    void SetStatus(WarriorStatus newStatus) { m_Status = newStatus; }
    const unsigned int GetHealth() const { return m_Health; }
    void SetHealth(unsigned int newVal) { m_Health = newVal; }
    const unsigned int GetPowerLevel() const { return m_PowerLevel; }
    void SetPowerLevel(unsigned int newVal) { m_PowerLevel = newVal; }

private:
    unsigned int m_Health{ 100 };
    unsigned int m_PowerLevel{ 100 };
};