#pragma once

#include <gameplay/warrior.h>

#include <vector>

class Scene;
class ModelComponent;

class GameDirector
{
public:
    std::vector<Warrior> m_AvailableWarriors;
    ModelComponent* m_PickedModel = nullptr;
    unsigned int m_CurrentTurn{ 0 };

    static GameDirector& GetInstance()
    {
        static GameDirector* directorInstance = new GameDirector();
        return *directorInstance;
    }

    void ResolveTurn(const Scene& scene);

    void SetPickedModel(ModelComponent* picked);
    ModelComponent* GetPickedModel() { return m_PickedModel; }

    void AddWarrior(std::string name, unsigned int health, unsigned int power);
    Warrior* GetWarrior(std::size_t index) { return &m_AvailableWarriors.at(index); }
    const Warrior* GetWarrior(std::size_t index) const { return &m_AvailableWarriors.at(index); }

private:
    GameDirector() {}

    static constexpr unsigned int MAX_TURNS{ 30 };
};