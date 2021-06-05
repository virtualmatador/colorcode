#include <iostream>
#include <sstream>

#include "../cross/core/src/bridge.h"

#include "data.h"

#include "toolbox.hpp"

main::Data main::data_;

const int main::Data::targets_max_;
const int main::Data::target_colors_max_;

main::Data::Data()
    : random_{seeder_()}
{
}

main::Data::~Data()
{
}

void main::Data::load()
{
    try
    {
        toolbox::Load("OPTION_TEXT", show_text_, false, false);
        toolbox::Load("OPTION_SOUND", sound_, false, false);
        toolbox::Load("GAME_OVER", game_over_, false, false);
        toolbox::Load("GAME_ACTIVE_TARGET", active_target_, 0, targets_max_ + 1);
        if (active_target_ == targets_max_ && !game_over_)
        {
            throw "";
        }
        for (std::size_t i = 0; i < targets_max_; ++i)
        {
            std::ostringstream composer;
            composer << "GAME_TARGET_COLOR_" << i;
            toolbox::Load(composer.str().c_str(), target_colors_[i], 0,
                target_colors_max_ + 1);
        }
        for (std::size_t i = 0; i < targets_max_; ++i)
        {
            std::ostringstream composer;
            composer << "GAME_CHOOSEN_COLOR_" << i;
            toolbox::Load(composer.str().c_str(), choosen_colors_[i], 0,
                target_colors_max_);
        }
        int rows_size;
        toolbox::Load("GAME_ROWS_SIZE", rows_size, 0, 1024);
        rows_.clear();
        for (std::size_t i = 0; i < rows_size; ++i)
        {
            Row row;
            int total_score = 0;
            for (std::size_t j = 0; j < row.first.size(); ++j)
            {
                std::ostringstream composer;
                composer << "GAME_ROWS_" << i << "_COLOR_" << j;
                toolbox::Load(composer.str().c_str(), row.first[j], 0, target_colors_max_);
            }
            for (std::size_t j = 0; j < row.second.size(); ++j)
            {
                std::ostringstream composer;
                composer << "GAME_ROWS_" << i << "_SCORE_" << j;
                toolbox::Load(composer.str().c_str(), row.second[j], 0, targets_max_ + 1);
                total_score += row.second[j];
            }
            if (total_score < targets_max_ + 1)
            {
                rows_.emplace_back(row);
            }
            else
            {
                throw "";
            }
        }
    }
    catch(...)
    {
        reset_all();
    }
}

void main::Data::save() const
{
    toolbox::Save("OPTION_TEXT", show_text_);
    toolbox::Save("OPTION_SOUND", sound_);
    toolbox::Save("GAME_OVER", game_over_);
    toolbox::Save("GAME_ACTIVE_TARGET", active_target_);
    for (std::size_t i = 0; i < targets_max_; ++i)
    {
        std::ostringstream composer;
        composer << "GAME_TARGET_COLOR_" << i;
        toolbox::Save(composer.str().c_str(), target_colors_[i]);
    }
    for (std::size_t i = 0; i < targets_max_; ++i)
    {
        std::ostringstream composer;
        composer << "GAME_CHOOSEN_COLOR_" << i;
        toolbox::Save(composer.str().c_str(), choosen_colors_[i]);
    }
    toolbox::Save("GAME_ROWS_SIZE", (int)rows_.size());
    for (std::size_t i = 0; i < rows_.size(); ++i)
    {
        for (std::size_t j = 0; j < rows_[i].first.size(); ++j)
        {
            std::ostringstream composer;
            composer << "GAME_ROWS_" << i << "_COLOR_" << j;
            toolbox::Save(composer.str().c_str(), rows_[i].first[j]);
        }
        for (std::size_t j = 0; j < rows_[i].second.size(); ++j)
        {
            std::ostringstream composer;
            composer << "GAME_ROWS_" << i << "_SCORE_" << j;
            toolbox::Save(composer.str().c_str(), rows_[i].second[j]);
        }
    }
}

void main::Data::reset_all()
{
    show_text_ = false;
    sound_ = false;
    reset_game();
}

void main::Data::reset_game()
{
    active_target_ = 0;
    for (auto& target_color : target_colors_)
    {
        target_color = target_colors_max_;
    }
    for (int i = 0; i < choosen_colors_.size(); ++i)
    {
        int dist = std::uniform_int_distribution<int>(
            0, target_colors_max_ - i - 1)(random_);
        for (int j = 0; j < target_colors_max_; ++j)
        {
            bool selected = false;
            for (int k = 0; k < i; ++k)
            {
                if (j == choosen_colors_[k])
                {
                    selected = true;
                    break;
                }
            }
            if (!selected)
            {
                if (--dist == -1)
                {
                    choosen_colors_[i] = j;
                    break;
                }
            }
        }
    }
    rows_.clear();
    game_over_ = false;
}

bool main::Data::validate_targets() const
{
    for (int i = 0; i < target_colors_.size(); ++i)
    {
        if (target_colors_[i] == target_colors_max_)
        {
            return false;
        }
        for (int j = 0; j < i; ++j)
        {
            if (target_colors_[i] == target_colors_[j])
            {
                return false;
            }
        }
    }
    for (const auto& row : rows_)
    {
        bool is_same = true;
        for (int i = 0; i < target_colors_.size(); ++i)
        {
            if (target_colors_[i] != row.first[i])
            {
                is_same = false;
                break;
            }
        }
        if (is_same)
        {
            return false;
        }
    }
    return true;
}

std::array<int, 2> main::Data::calculate_scores() const
{
    int black = 0, white = 0;
    for (int i = 0; i < target_colors_.size(); ++i)
    {
        for (int j = 0; j < choosen_colors_.size(); ++j)
        {
            if (target_colors_[i] == choosen_colors_[j])
            {
                if (i == j)
                {
                    ++black;
                }
                else
                {
                    ++white;
                }
            }
        }
    }
    return {black, white};
}
