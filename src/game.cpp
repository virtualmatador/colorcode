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
            std::ostringstream js;
            js.str("");
            js.clear();
            js << "setText(" << (data_.show_text_ ? "true" : "false") << ");";
            bridge::CallFunction(js.str().c_str());
            update_view();
        }
    };
    handlers_["bucket"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            if (data_.game_over_ == 0)
            {
                int color = std::stoi(info);
                data_.target_colors_[data_.active_target_] = color;
                std::ostringstream js;
                js.str("");
                js.clear();
                js << "setTargetColor(" << data_.active_target_
                    << ", " << color << ");";
                bridge::CallFunction(js.str().c_str());
                if (data_.sound_)
                {
                    bridge::PlayAudio(20 + color);
                }
            }
        }
    };
    handlers_["target"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            int active_target = std::stoi(info);
            if (data_.game_over_ == 0)
            {
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
                if (data_.sound_)
                {
                    bridge::PlayAudio(active_target);
                }
            }
            else if (data_.game_over_ == 2)
            {
                if (data_.sound_)
                {
                    bridge::PlayAudio(20 + data_.target_colors_[active_target]);
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
            if (data_.game_over_ == 0)
            {
                if (data_.sound_)
                {
                    bridge::PlayAudio(31);
                }
            }
            data_.reset_game();
            update_view();
        }
        else if (std::strcmp(command, "step") == 0)
        {
            if (data_.game_over_ == 0)
            {
                if (data_.validate_targets())
                {
                    auto scores = data_.calculate_scores();
                    data_.rows_.push_back(std::make_pair(data_.target_colors_, scores));
                    add_row(data_.rows_.back());
                    if (scores[0] == Data::targets_max_)
                    {
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
                        data_.game_over_ = 1;
                        game_over();
                        if (data_.sound_)
                        {
                            bridge::PlayAudio(30);
                        }
                    }
                    else if (data_.rows_.size() == data_.rows_max_)
                    {
                        give_up();
                    }
                    else
                    {
                        if (data_.sound_)
                        {
                            bridge::PlayAudio(29);
                        }
                    }
                }
                else
                {
                    if (data_.sound_)
                    {
                        bridge::PlayAudio(28);
                    }
                }
            }
        }
        else if (std::strcmp(command, "giveup") == 0)
        {
            if (data_.game_over_ == 0)
            {
                give_up();
            }
        }
    };
    handlers_["guess"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            std::istringstream is{ info };
            int row, column;
            is >> row >> column;
            if (data_.sound_)
            {
                bridge::PlayAudio(20 + data_.rows_[row].first[column]);
            }
        }
    };
    handlers_["score"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            std::istringstream is{ info };
            int row, column;
            is >> row >> column;
            if (column < data_.rows_[row].second[0])
            {
                if (data_.sound_)
                {
                    bridge::PlayAudio(20 + 6 + 0);
                }
            }
            else if (column < data_.rows_[row].second[0] + data_.rows_[row].second[1])
            {
                if (data_.sound_)
                {
                    bridge::PlayAudio(20 + 6 + 1);
                }
            }
        }
    };
    handlers_["index"] = [&](const char* command, const char* info)
    {
        if (std::strlen(command) == 0)
            return;
        else if (std::strcmp(command, "click") == 0)
        {
            std::istringstream is{ info };
            int index;
            is >> index;
            if (data_.sound_)
            {
                bridge::PlayAudio(index);
            }
        }
    };
    bridge::LoadWebView(index_,
        (std::int32_t)core::VIEW_INFO::Portrait |
        (std::int32_t)core::VIEW_INFO::AudioNoSolo, "game",
        "n0 n1 n2 n3 n4 n5 n6 n7 n8 n9 n10 n11 n12 n13 n14 n15 n16 n17 n18 n19"
        " c0 c1 c2 c3 c4 c5 s0 s1 p0 p1 e0 e1");
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
    game_over();
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

void main::Game::give_up()
{
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
    data_.game_over_ = 2;
    game_over();
    if (data_.sound_)
    {
        bridge::PlayAudio(31);
    }
}

void main::Game::game_over()
{
    std::ostringstream js;
    js.str("");
    js.clear();
    js << "gameOver(" << data_.game_over_ << ");";
    bridge::CallFunction(js.str().c_str());
}
