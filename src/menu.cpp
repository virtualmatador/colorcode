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
            std::ostringstream js;
            js.str("");
            js.clear();
            js << "setText(" << (data_.show_text_ ? "true" : "false") << ");";
            bridge::CallFunction(js.str().c_str());
            js.str("");
            js.clear();
            js << "setSound(" << (data_.sound_ ? "true" : "false") << ");";
            bridge::CallFunction(js.str().c_str());
        }
    };
    handlers_["play"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            Play();
    };
    handlers_["text"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            if (std::strlen(info) == 0)
            {
                return;
            }
            else if (std::strcmp(info, "true") == 0)
            {
                data_.show_text_ = true;
            }
            else if (std::strcmp(info, "false") == 0)
            {
                data_.show_text_ = false;
            }
        }
    };
    handlers_["sound"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            if (std::strlen(info) == 0)
            {
                return;
            }
            else if (std::strcmp(info, "true") == 0)
            {
                data_.sound_ = true;
            }
            else if (std::strcmp(info, "false") == 0)
            {
                data_.sound_ = false;
            }
        }
    };
    handlers_["reset"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
            data_.reset_all();
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
