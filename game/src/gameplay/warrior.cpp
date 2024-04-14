#include <gameplay/warrior.h>

#include <algorithm>

void Warrior::SetHealth(int newVal) 
{
    newVal = std::clamp(newVal, 0, m_MaxHealth);

    m_Health = newVal;
}

void Warrior::SetStatus(WarriorStatus newStatus)
{
    m_Status = newStatus;
}

void Warrior::TakeDamage(int dmg)
{
    SetHealth(m_Health - dmg);
    if (m_Health <= 0)
    {
        SetStatus(WarriorStatus::Dead);
    }
}

void Warrior::LevelUp()
{
    int newMaxHealth = m_MaxHealth + HEALTH_LEVEL_UP_GAIN;
    if (GetHealth() == GetMaxHealth())
    {
        SetHealth(newMaxHealth);
    }
    SetMaxHealth(newMaxHealth);
    SetPowerLevel(m_PowerLevel + POWER_LEVEL_LVL_UP_GAIN);
}