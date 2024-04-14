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