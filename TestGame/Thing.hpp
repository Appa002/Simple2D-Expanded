#pragma once
#include <Behavior.h>
#include <RenderableUiElement.h>

using namespace Simple2D;

class Thing : public Behavior
{
private:
    float offset = 0;

public:
    void setup() override;
    void update(long deltaTime) override;
};

