#include <gameplay/gamedirector.h>

#include <game_object.h>
#include <scene.h>

#include <ui/boardcomponent.h>
#include <gameplay/settlement.h>

#include <algorithm>
#include <string.h>

void GameDirector::ResetDirector()
{
    m_AvailableWarriors.clear();
    m_CurrentTurn = 0;
    m_Cash = 100;
    m_GameOver = false;
    m_PickedModel = nullptr;
    m_PickedWarriorIndex = -1;
}

bool GameDirector::AddWarrior(std::string name, unsigned int health, unsigned int power)
{
    if (m_AvailableWarriors.size() == MAX_WARRIORS || m_Cash - WARRIOR_COST < 0)
    {
        return false;
    }

    m_AvailableWarriors.emplace_back(Warrior(name, health, power));
    m_Cash -= WARRIOR_COST;
    return true;
}

void GameDirector::AddStartingWarriors()
{
    m_AvailableWarriors.emplace_back(Warrior("Guillaume", 150, 100));
    m_AvailableWarriors.emplace_back(Warrior("Pierre", 150, 100));
}

void GameDirector::SetPickedModel(ModelComponent* picked)
{
    if (picked == nullptr && m_PickedModel != nullptr)
    {
        m_PickedModel->SetPicked(false);
        m_PickedModel = nullptr;
        return;
    }

    GameObject& object = picked->GetGameObject();
    char* header = "";
    char* message = "";
    if (SettlementComponent* settlement = object.GetComponent<SettlementComponent>())
    {
        if (Event* event = settlement->GetEvent())
        {
            header = const_cast<char*>(TextFormat("%s", event->GetName()));
            message = const_cast<char*>(TextFormat("The Event will inflict : %i per turn\n Settlement has: %i HP", event->m_Damage, settlement->m_Health));
        }
        else
        {
            header = "All Clear";
            message = "Nothing to see here ;)";
        }
    }
    object.GetComponent<BoardComponent>()->SetHeader(header)->SetMessage(message)->SetShown(true, true);
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

    return m_PickedModel->GetGameObject().GetComponent<SettlementComponent>();
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

    ++m_CurrentTurn;
    if (m_CurrentTurn == MAX_TURNS || AreAllSettlementsDestroyed(scene))
    {
        SetGameOver(true);
    }
}

void GameDirector::ResolveSettlementEvent(GameObject& child)
{
    if (SettlementComponent* settlement = child.GetComponent<SettlementComponent>())
    {
        if (settlement->GetStatus() == SettlementStatus::Clear)
        {
            if (GetRandomValue(0, 10) > 5)
            {
                settlement->AddEvent(GetRandomEvent());
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
                if (settlement->GetWarriorPower() >= event->m_RequiredPower)
                {
                    success = true;
                    settlement->HandleEventEnd(success);
                }
                else
                {
                    settlement->TakeDamage(event->m_Damage);
                    if (settlement->m_Health <= 0)
                    {
                        settlement->HandleEventEnd(success);
                    }
                }
            }
        }

        if (ModelComponent* model = child.GetComponent<ModelComponent>())
        {
            model->SetTint(settlement->GetColor());
        }
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
    for (Warrior warrior : m_AvailableWarriors)
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
    m_EventList.emplace_back(Event("ATTACK", 30, 20, 50, 20));
    m_EventList.emplace_back(Event("AMBUSH", 15, 10, 30, 10));
    m_EventList.emplace_back(Event("SKIRMISH", 5, 5, 15, 5));
}

Event& GameDirector::GetRandomEvent()
{
    return m_EventList[GetRandomValue(0, m_EventList.size() - 1)];
}