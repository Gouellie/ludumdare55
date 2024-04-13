#pragma once

#include <string>

class Event
{
public:
    char* m_Name;
    unsigned int m_TurnsToResolve{ 0 };
    unsigned int m_Damage{ 0 };
    unsigned int m_RequiredPower{ 0 };
    unsigned int m_Penalty{ 0 }; // If ignored
    unsigned int m_Cost{ 0 }; // If solved

    Event();
    Event(char* name, unsigned int timeToSolve, unsigned int requiredPower, unsigned int penalty, unsigned int cost)
        : m_Name(name), m_TurnsToResolve{ timeToSolve }, m_RequiredPower{ requiredPower }, m_Penalty{penalty}, m_Cost{cost} {}

    void SetName(char* name) { m_Name = name; }
    char* GetName() { return m_Name; }
};