#include <gameplay/event.h>

Event::Event(const Event& other)
{
    m_Name = other.m_Name;
    m_Damage = other.m_Damage;
    m_RequiredPower = other.m_RequiredPower;
    m_Penalty = other.m_Penalty;
    m_DamageToWarriors = other.m_DamageToWarriors;
}