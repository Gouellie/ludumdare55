#pragma once

class Event
{
public:
    unsigned int m_TurnsToResolve{ 0 };
    unsigned int m_RequiredPower{ 0 };
    unsigned int m_Penalty{ 0 }; // If ignored
    unsigned int m_Cost{ 0 }; // If solved

    Event();
    Event(unsigned int timeToSolve, unsigned int requiredPower, unsigned int penalty, unsigned int cost)
        : m_TurnsToResolve{ timeToSolve }, m_RequiredPower{ requiredPower }, m_Penalty{penalty}, m_Cost{cost} {}

};