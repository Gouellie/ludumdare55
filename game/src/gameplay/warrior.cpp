#include <gameplay/warrior.h>

void Warrior::GetData()
{

}

void Warrior::SetHealth(int newVal) 
{
    if (newVal > m_MaxHealth)
    {
        newVal = m_MaxHealth;
    }

    m_Health = newVal;
}

void Warrior::TakeDamage(int dmg)
{
    SetHealth(m_Health - dmg);
    if (m_Health <= 0)
    {
        SetStatus(WarriorStatus::Dead);
    }
}