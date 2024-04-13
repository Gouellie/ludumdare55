#include <gameplay/gamedirector.h>

#include <scene.h>

#include <gameplay/settlement.h>

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
                    settlement->SetStatus(SettlementStatus::Attacked);
                }
            }
            else
            {
                settlement->SetStatus(SettlementStatus::Destroyed);
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