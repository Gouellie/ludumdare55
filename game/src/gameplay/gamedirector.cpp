#include <gameplay/gamedirector.h>

#include <game_object.h>
#include <scene.h>

#include <ui/boardcomponent.h>
#include <gameplay/settlement.h>
#include <gameplay/warrior.h>

#include <string>

void GameDirector::AddWarrior(std::string name, unsigned int health, unsigned int power)
{
    m_AvailableWarriors.emplace_back(Warrior(name, health, power));
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
            message = const_cast<char*>(TextFormat("The Event will last for : %i turns", event->m_TurnsToResolve));
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

// Handle end of turn
void GameDirector::ResolveTurn(const Scene& scene)
{
    HealWarriors();

    for (GameObject* child : scene.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            if (settlement->GetStatus() == SettlementStatus::Clear)
            {
                int min = 0;
                int max = 10;
                int random = min + (rand() % static_cast<int>(max - min + 1));
                if (random > 5)
                {
                    settlement->AddEvent("ATTACK", 1, 10, 0, 0);
                    settlement->SetStatus(SettlementStatus::Attacked);
                }
            }
            else
            {
                std::vector<Warrior*> warriors;
                settlement->GetWarriors(warriors);
                if (Event* event = settlement->GetEvent())
                {
                    if (settlement->GetWarriorPower() >= event->m_RequiredPower)
                    {
                        settlement->SetStatus(SettlementStatus::Clear);
                        settlement->ClearEvent();
                        settlement->ClearWarriors();
                    }
                    else
                    {
                        --event->m_TurnsToResolve;
                        if (event->m_TurnsToResolve == 0)
                        {
                            settlement->SetStatus(SettlementStatus::Destroyed);
                        }
                    }
                }
            }

            if (ModelComponent* model = child->GetComponent<ModelComponent>())
            {
                model->SetTint(settlement->GetColor());
            }
        }
    }

    AddCash(scene);

    ++m_CurrentTurn;
    if (m_CurrentTurn == MAX_TURNS || AreAllSettlementsDestroyed(scene))
    {
        // SIGNAL END GAME HERE
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
            if (settlement->GetStatus() != SettlementStatus::Clear)
            {
                return false;
            }
        }
    }

    return true;
}

void GameDirector::AddCash(const Scene& scene)
{
    for (GameObject* child : scene.GetChildren())
    {
        if (!child)
            continue;

        if (SettlementComponent* settlement = child->GetComponent<SettlementComponent>())
        {
            if (settlement->GetStatus() == SettlementStatus::Clear)
            {
                m_Cash += settlement->GetIncome();
            }
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