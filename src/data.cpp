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
    }
    catch(...)
    {
        Reset();
    }
}

void main::Data::Save()
{
    toolbox::Save("GAME_ACTIVE_TARGET", active_target_);
}

void main::Data::Reset()
{
    active_target_ = 0;
    sound_ = true;
}
