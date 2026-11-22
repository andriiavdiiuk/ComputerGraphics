#pragma once
#include <vector>

#include "Models/IfsFractal.hpp"

struct MenuParams
{
    std::function<void()> applyChanges;
    IfsFractalParams& ifsParams;
};

void drawMenu(MenuParams& params);
