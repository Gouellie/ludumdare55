#include <gameplay/settlement.h>

#include <palette.h>
#include <gameplay/warrior.h>
#include <gameplay/gamedirector.h>

void SettlementComponent::AddEvent(const char* name, int damagePerTurn, int requiredPower, int penalty, int cost)
{
    m_CurrentEvent = new Event(name, damagePerTurn, requiredPower, penalty, cost);
}

void SettlementComponent::AddEvent(const Event& event)
{
    m_CurrentEvent = new Event(event);
}

void SettlementComponent::ClearEvent()
{
    delete m_CurrentEvent;
    m_CurrentEvent = nullptr;
}

void SettlementComponent::HandleEventEnd(const bool success)
{
    if (success == true)
    {
        SetStatus(SettlementStatus::Clear);
        DealDamageToWarriors();
        ClearEvent();
        ClearWarriors();
    }
    else
    {
        SetStatus(SettlementStatus::Destroyed);
        DealDamageToWarriors();
        InflictPenalty();
        ClearEvent();
        ClearWarriors();
    }
}

void SettlementComponent::GetWarriors(std::vector<Warrior*>& warriors)
{
    warriors = m_AssignedWarriors;
}

void SettlementComponent::AddWarrior(Warrior* warrior)
{
    if (m_AssignedWarriors.size() >= MAX_ASSIGNED_WARRIORS || warrior->GetStatus() != WarriorStatus::Waiting)
    {
        return;
    }

    m_AssignedWarriors.push_back(warrior);
    warrior->SetStatus(WarriorStatus::Dispatched);
}

void SettlementComponent::RemoveWarrior(std::size_t index)
{
    m_AssignedWarriors.at(index)->SetStatus(WarriorStatus::Waiting);
    auto it = m_AssignedWarriors.begin();
    it = std::next(it, index);
    m_AssignedWarriors.erase(it);
}

const int SettlementComponent::GetWarriorPower() const
{
    int totalPower = 0;
    for (const Warrior* warrior : m_AssignedWarriors)
    {
        totalPower += warrior->GetPowerLevel();
    }
    return totalPower;
}

void SettlementComponent::DealDamageToWarriors()
{
    for (Warrior* warrior : m_AssignedWarriors)
    {
        warrior->TakeDamage(m_CurrentEvent->GetDamageToWarriors());
    }
}

void SettlementComponent::ClearWarriors()
{
    for (Warrior* warrior : m_AssignedWarriors)
    {
        if (warrior->GetStatus() == WarriorStatus::Dispatched)
        {
            warrior->SetStatus(WarriorStatus::Waiting);
        }
    }
    m_AssignedWarriors.clear();
}

void SettlementComponent::GetData(SettlementStatus& status, std::vector<Warrior*>& assignedWarriors)
{
    status = m_Status;
    GetWarriors(assignedWarriors);
}

void SettlementComponent::InflictPenalty()
{
    GameDirector& director = GameDirector::GetInstance();
    director.TakePenalty(m_CurrentEvent->GetPenalty());
}

Color SettlementComponent::GetColor()
{
    switch (GetStatus())
    {
    case SettlementStatus::Clear:
        return PAL_LIGHT_BLUE;
        break;
    case SettlementStatus::Attacked:
        return PAL_RED;
        break;
    case SettlementStatus::Destroyed:
        return PAL_NEAR_BLACK;
        break;
    default:
        return PAL_PURPLE;
        break;
    }
}