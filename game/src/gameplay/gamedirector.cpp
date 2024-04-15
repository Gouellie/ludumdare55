#include <gameplay/gamedirector.h>

#include <screens.h>
#include <raymath.h>
#include <game_object.h>
#include <scene.h>

#include <ui/boardcomponent.h>
#include <gameplay/buyable.h>
#include <gameplay/settlement.h>

#include <algorithm>

void GameDirector::ResetDirector()
{
    m_AvailableWarriors.clear();
    m_CurrentTurn = 0;
    m_Cash = 100;
    m_GameOver = false;
    m_PickedModel = nullptr;
    m_PickedWarriorIndex = -1;
    for (GameObject* child : Settlements.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            settlement->Reset();
        }
    }

}

bool GameDirector::AddWarrior(Warrior warrior)
{
    if (m_AvailableWarriors.size() == MAX_WARRIORS || !CanBuy(warrior))
    {
        return false;
    }

    m_AvailableWarriors.emplace_back(warrior);
    m_AvailableWarriors[m_AvailableWarriors.size() - 1].SetIndex(m_AvailableWarriors.size() - 1);
    m_Cash -= warrior.GetPrice();
    return true;
}

void GameDirector::AddStartingWarriors()
{
    m_AvailableWarriors.emplace_back(Warrior("Guillaume", 150, 100));
    m_AvailableWarriors[0].SetIndex(0);
    m_AvailableWarriors.emplace_back(Warrior("Pierre", 150, 100));
    m_AvailableWarriors[1].SetIndex(1);
}

void GameDirector::SetPickedModel(ModelComponent* picked)
{
    if (picked == nullptr && m_PickedModel != nullptr)
    {
        m_PickedModel->GetComponent<BoardComponent>()->SetShown(false);
        m_PickedModel->SetPicked(false);
        m_PickedModel = nullptr;
        return;
    }
    else if (picked == nullptr) 
    {
        return;
    }
    picked->GetComponent<BoardComponent>()->SetShown(true, true);
    if (m_PickedModel != nullptr)
    {
        GameObject& oldObject = m_PickedModel->GetGameObject();
        oldObject.GetComponent<BoardComponent>()->SetShown(false);
        m_PickedModel->SetPicked(false);
        m_PickedModel = picked;
        m_PickedModel->SetPicked(true);
    }
    else
    {
        m_PickedModel = picked;
        m_PickedModel->SetPicked(true);
    }
}

SettlementComponent* GameDirector::GetPickedSettlement()
{
    if (m_PickedModel == nullptr)
    {
        return nullptr;
    }

    return m_PickedModel->GetComponent<SettlementComponent>();
}

// Handle end of turn
void GameDirector::ResolveTurn(const Scene& scene)
{
    HealWarriors();
    for (GameObject* child : scene.GetChildren())
    {
        if (!child)
            continue;

        ResolveSettlementEvent(*child);
        AddCash(*child);
    }

    SetPickedModel(nullptr);
    IncrementTurn();
    EvaluateGameOver(scene);
}

void GameDirector::ResolveSettlementEvent(GameObject& child)
{
    if (SettlementComponent* settlement = child.GetComponent<SettlementComponent>())
    {
        if (settlement->GetStatus() == SettlementStatus::Clear)
        {
            int targetProbability = 25 + GetCurrentTurn() * 2;
            if (GetRandomValue(0, 100) < targetProbability)
            {
                settlement->AddEvent(GetRandomEvent());
                settlement->GetEvent()->ApplyDifficultyScale(GetCurrentTurn());
                settlement->SetStatus(SettlementStatus::Attacked);
            }
        }
        else
        {
            std::vector<Warrior*> warriors;
            settlement->GetWarriors(warriors);
            if (Event* event = settlement->GetEvent())
            {
                bool success = false;
                if (settlement->GetWarriorPower() >= event->GetRequiredPower())
                {
                    success = true;
                    settlement->HandleEventEnd(success);
                }
                else
                {
                    settlement->TakeDamage(event->GetDamage());
                    if (settlement->GetHealth() <= 0)
                    {
                        settlement->HandleEventEnd(success);
                    }
                }
            }
        }

        settlement->ClearWarriors();
    }
}

bool GameDirector::AreAllSettlementsDestroyed(const Scene& scene)
{
    for (GameObject* child : scene.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            if (settlement->GetStatus() != SettlementStatus::Destroyed)
            {
                return false;
            }
        }
    }

    return true;
}

void GameDirector::AddCash(GameObject& child)
{
    if (SettlementComponent* settlement = child.GetComponent<SettlementComponent>())
    {
        if (settlement->GetStatus() == SettlementStatus::Clear)
        {
            m_Cash += settlement->GetIncome();
        }
    }
}

void GameDirector::HealWarriors()
{
    for (Warrior& warrior : m_AvailableWarriors)
    {
        if (warrior.GetStatus() == WarriorStatus::Waiting)
        {
            warrior.SetHealth(warrior.GetHealth() + 10);
        }
        else if (warrior.GetStatus() == WarriorStatus::Healing)
        {
            warrior.SetHealth(warrior.GetHealth() + 25);
        }
    }
}

void GameDirector::PopulateEventList()
{
    m_EventList.emplace_back(Event("SKIRMISH", 5, 5, 15, 5));
    m_EventList.emplace_back(Event("AMBUSH", 15, 10, 30, 10));
    m_EventList.emplace_back(Event("ATTACK", 30, 20, 50, 20));
}

Event& GameDirector::GetRandomEvent()
{
    return m_EventList[GetRandomValue(0, m_EventList.size() - 1)];
}

void GameDirector::EvaluateGameOver(const Scene& scene)
{
    if (AreAllSettlementsDestroyed(scene))
    {
        SetGameState(GameState::Fail);
        SetGameOver(true);
    }
    else if (m_CurrentTurn >= MAX_TURNS)
    {
        SetGameState(GameState::Victory);
        SetGameOver(true);
    }
}

const bool GameDirector::CanBuy(const Buyable& item) const
{
    if (GetCash() - item.GetPrice() < 0)
    {
        return false;
    }

    return true;
}

void GameDirector::TakePenalty(int penalty)
{
    m_Cash = Clamp(m_Cash - penalty, 0, INT_MAX);
}