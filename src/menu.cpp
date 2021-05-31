#include <sstream>
#include <cstring>

#include "../cross/core/src/helper.h"

#include "menu.h"
#include "data.h"
#include "progress.h"

main::Menu::Menu()
{
    handlers_["body"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "ready") == 0)
        {
        }
    };
    handlers_["play"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            Play();
    };
    bridge::LoadWebView(index_, (std::int32_t)core::VIEW_INFO::Default, "menu", "");
}

main::Menu::~Menu()
{
}

void main::Menu::Escape()
{
    bridge::Exit();
}

void main::Menu::Play()
{
    progress_ = PROGRESS::GAME;
    bridge::NeedRestart();
}
