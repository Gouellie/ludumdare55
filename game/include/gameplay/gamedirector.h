#pragma once

#include <vector>

class Scene;

class GameDirector
{
public:
    static GameDirector& GetInstance()
    {
        static GameDirector* directorInstance = new GameDirector();
        return *directorInstance;
    }

    void ResolveTurn(const Scene& scene);
    unsigned int m_CurrentTurn{ 0 };

private:
    GameDirector() {}

    static constexpr unsigned int MAX_TURNS{ 30 };
};