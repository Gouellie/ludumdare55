#include <gameplay/warrior.h>

void Warrior::GetData()
{

}

void Warrior::TakeDamage(int dmg)
{
    SetHealth(m_Health - dmg);
    if (m_Health <= 0)
    {
        SetStatus(WarriorStatus::Dead);
    }
}