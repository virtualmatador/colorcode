#include <sstream>
#include <cstring>

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
            if (data_.incompatible_save_)
                bridge::CallFunction("showSaveVersionError()");
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
            if (data_.incompatible_save_ || std::strlen(info) == 0)
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
            if (data_.incompatible_save_ || std::strlen(info) == 0)
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
        {
            return;
        }
        else if (std::strcmp(command, "click") == 0)
        {
            data_.reset_all();
            RequestStage();
        }
    };
}

void main::Menu::Attach()
{
    bridge::SetAudioNoSolo(true);
    bridge::SetLayout(false, false);
    bridge::LoadView(Index(), "menu");
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
    if (data_.incompatible_save_)
        return;
    progress_ = PROGRESS::GAME;
    RequestStage();
}

void main::Menu::FeedUri(const char* uri, std::function<void(
    const std::vector<unsigned char>&)>&& consume)
{
}
