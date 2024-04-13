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
            header = const_cast<char*>(TextFormat("%s", event->GetName().c_str()));
            message = const_cast<char*>(TextFormat("The Event will last for : %i turns", event->m_TurnsToResolve));
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
                    settlement->AddEvent(std::string("ATTACK"), 1, 10, 0, 0);
                    settlement->SetStatus(SettlementStatus::Attacked);
                }
            }
            else
            {
                std::vector<Warrior*> warriors;
                settlement->GetWarriors(warriors);
                if (Event* event = settlement->GetEvent())
                {
                    if (event->m_TurnsToResolve == 0)
                    {
                        settlement->SetStatus(SettlementStatus::Destroyed);
                    }
                    else
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

    ++m_CurrentTurn;
    if (m_CurrentTurn == MAX_TURNS)
    {
        // SIGNAL END GAME HERE
    }
}