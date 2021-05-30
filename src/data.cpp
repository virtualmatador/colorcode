#include <iostream>
#include <sstream>

#include "../cross/core/src/bridge.h"

#include "data.h"

#include "toolbox.hpp"

main::Data main::data_;

const int main::Data::targets_max_;

main::Data::Data()
    : random_{seeder_()}
{
}

main::Data::~Data()
{
}

void main::Data::Load()
{
    try
    {
        toolbox::Load("GAME_ACTIVE_TARGET", active_target_, 0, targets_max_);
        for (std::size_t i = 0; i < targets_max_; ++i)
        {
            std::ostringstream composer;
            composer << "GAME_TARGET_COLOR_" << i;
            toolbox::Load(composer.str().c_str(), target_colors_[i], 0,
                target_colors_max_ + 1);
        }
    }
    catch(...)
    {
        Reset();
    }
}

void main::Data::Save()
{
    toolbox::Save("GAME_ACTIVE_TARGET", active_target_);
    for (std::size_t i = 0; i < targets_max_; ++i)
    {
        std::ostringstream composer;
        composer << "GAME_TARGET_COLOR_" << i;
        toolbox::Save(composer.str().c_str(), target_colors_[i]);
    }
}

void main::Data::Reset()
{
    active_target_ = 0;
    for (auto& target_color : target_colors_)
    {
        target_color = target_colors_max_;
    }
    sound_ = true;
}
