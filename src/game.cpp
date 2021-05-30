#include <sstream>
#include <cstring>

#include "../cross/core/src/helper.h"

#include "game.h"
#include "data.h"
#include "progress.h"

main::Game::Game()
{
    handlers_["body"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "ready") == 0)
        {
            std::ostringstream js;
            for (std::size_t i = 0; i < 4; ++i)
            {
                if (data_.target_colors_[i] < Data::target_colors_max_)
                {
                    js.clear();
                    js << "setTargetColor(" << i << ","
                        << data_.target_colors_[i] << ");";
                    bridge::CallFunction(js.str().c_str());
                    js.str("");
                }
                else
                {
                    js.str("");
                    js.clear();
                    js << "clearTargetColor(" << i << ");";
                    bridge::CallFunction(js.str().c_str());
                }
            }
            js.str("");
            js.clear();
            js << "setTargetActive(" << data_.active_target_ << ");";
            bridge::CallFunction(js.str().c_str());
        }
    };
    handlers_["bucket"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            std::size_t color = std::stoul(info);
            data_.target_colors_[data_.active_target_] = color;
            std::ostringstream js;
            js.str("");
            js.clear();
            js << "setTargetColor(" << data_.active_target_
                << ", " << color << ");";
            bridge::CallFunction(js.str().c_str());
        }
    };
    handlers_["target"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            int active_target = std::stoul(info);
            if (data_.active_target_ == active_target)
            {
                data_.target_colors_[data_.active_target_] = Data::target_colors_max_;
                std::ostringstream js;
                js.str("");
                js.clear();
                js << "clearTargetColor(" << data_.active_target_ << ");";
                bridge::CallFunction(js.str().c_str());
            }
            else
            {
                std::ostringstream js;
                js.str("");
                js.clear();
                js << "clearTargetActive(" << data_.active_target_ << ");";
                bridge::CallFunction(js.str().c_str());
                data_.active_target_ = active_target;
                js.str("");
                js.clear();
                js << "setTargetActive(" << data_.active_target_ << ");";
                bridge::CallFunction(js.str().c_str());
            }
        }
    };
    bridge::LoadWebView(index_, (std::int32_t)core::VIEW_INFO::Default, "game", "");
}

main::Game::~Game()
{
}

void main::Game::Escape()
{
    main::progress_ = PROGRESS::MENU;
    bridge::NeedRestart();
}
