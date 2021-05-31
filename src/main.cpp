#include "../cross/core/src/main.h"

#include "data.h"
#include "menu.h"
#include "game.h"
#include "progress.h"

main::PROGRESS main::progress_ = main::PROGRESS::MENU;

void life_cycle::Begin()
{

}

void life_cycle::End()
{

}

void life_cycle::Create()
{

}

void life_cycle::Destroy()
{

}

void life_cycle::Start()
{
    main::data_.load();
}

void life_cycle::Stop()
{
    main::data_.save();
}

void life_cycle::Restart()
{
    switch (main::progress_)
    {
        case main::PROGRESS::MENU:
            core::Stage::stage_ = std::make_unique<main::Menu>();
        break;
        case main::PROGRESS::GAME:
            core::Stage::stage_ = std::make_unique<main::Game>();
        break;
    }
}

