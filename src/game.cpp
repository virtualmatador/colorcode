#include <sstream>
#include <cstring>

#include "../cross/core/src/helper.h"

#include "game.h"
#include "progress.h"

main::Game::Game()
{
    handlers_["body"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "ready") == 0)
        {
            update_view();
        }
    };
    handlers_["bucket"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            if (!data_.game_over_)
            {
                int color = std::stoi(info);
                data_.target_colors_[data_.active_target_] = color;
                std::ostringstream js;
                js.str("");
                js.clear();
                js << "setTargetColor(" << data_.active_target_
                    << ", " << color << ");";
                bridge::CallFunction(js.str().c_str());
            }
        }
    };
    handlers_["target"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            if (!data_.game_over_)
            {
                int active_target = std::stoi(info);
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
        }
    };
    handlers_["game"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "stop") == 0)
        {
            Escape();
        }
        else if (std::strcmp(command, "reset") == 0)
        {
            data_.reset();
            update_view();
        }
        else if (std::strcmp(command, "step") == 0)
        {
            if (!data_.game_over_)
            {
                if (data_.validate_targets())
                {
                    auto scores = data_.calculate_scores();
                    data_.rows_.push_back(std::make_pair(data_.target_colors_, scores));
                    add_row(data_.rows_.back());
                    if (scores[0] == Data::targets_max_)
                    {
                        data_.game_over_ = true;
                        data_.active_target_ = Data::targets_max_;
                        for (std::size_t i = 0; i < 4; ++i)
                        {
                            data_.target_colors_[i] = Data::target_colors_max_;
                            std::ostringstream js;
                            js.str("");
                            js.clear();
                            js << "clearTargetColor(" << i << ");";
                            bridge::CallFunction(js.str().c_str());
                            js.str("");
                            js.clear();
                            js << "clearTargetActive(" << i << ");";
                            bridge::CallFunction(js.str().c_str());
                        }
                        bridge::CallFunction("gameOver(true);");
                    }
                }
            }
        }
        else if (std::strcmp(command, "giveup") == 0)
        {
            if (!data_.game_over_)
            {
                data_.game_over_ = true;
                data_.active_target_ = Data::targets_max_;
                for (std::size_t i = 0; i < 4; ++i)
                {
                    data_.target_colors_[i] = data_.choosen_colors_[i];
                    std::ostringstream js;
                    js.str("");
                    js.clear();
                    js << "setTargetColor(" << i << ", "
                        << data_.target_colors_[i] << ");";
                    bridge::CallFunction(js.str().c_str());
                    js.str("");
                    js.clear();
                    js << "clearTargetActive(" << i << ");";
                    bridge::CallFunction(js.str().c_str());
                }
                bridge::CallFunction("gameOver(true);");
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

void main::Game::update_view()
{
    std::ostringstream js;
    for (std::size_t i = 0; i < 4; ++i)
    {
        if (data_.target_colors_[i] < Data::target_colors_max_)
        {
            js.str("");
            js.clear();
            js << "setTargetColor(" << i << ","
                << data_.target_colors_[i] << ");";
            bridge::CallFunction(js.str().c_str());
        }
        else
        {
            js.str("");
            js.clear();
            js << "clearTargetColor(" << i << ");";
            bridge::CallFunction(js.str().c_str());
        }
        js.str("");
        js.clear();
        js << "clearTargetActive(" << i << ");";
        bridge::CallFunction(js.str().c_str());
    }
    js.str("");
    js.clear();
    js << "setTargetActive(" << data_.active_target_ << ");";
    bridge::CallFunction(js.str().c_str());
    bridge::CallFunction("clearRows();");
    for (const auto& row : data_.rows_)
    {
        add_row(row);
    }
    js.str("");
    js.clear();
    js << "gameOver(" << (data_.game_over_ ? "true" : "false") << ");";
    bridge::CallFunction(js.str().c_str());
}

void main::Game::add_row(const Data::Row& row)
{
    std::ostringstream js;
    js.str("");
    js.clear();
    js << "addRow([";
    for (auto& target_color : row.first)
    {
        js << target_color << ", ";
    }
    js << "], [";
    for (auto& score : row.second)
    {
        js << score << ", ";
    }
    js << "]);";
    bridge::CallFunction(js.str().c_str());
}
