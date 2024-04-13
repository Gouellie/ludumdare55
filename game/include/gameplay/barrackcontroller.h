#pragma once

#include <behavior.h>

class BarrackController : public GameObjectBehavior
{
private:
    float m_fHeight = 40;
    bool m_bExpanded = false;

public:
    DEFINE_BEHAVIOR(BarrackController)

    void OnUpdate() override
    {
    }

    void OnRender() override;
};