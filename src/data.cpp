#include <istream>
#include <ostream>
#include <utility>

#include "data.h"

main::Data main::data_;

const int main::Data::targets_max_;
const int main::Data::target_colors_max_;
const int main::Data::rows_max_;

main::Data::Data()
    : random_{seeder_()}
{
}

main::Data::~Data()
{
}

void main::Data::load(std::istream& input)
{
    reset_all();
    input >> std::dec >> std::noboolalpha >> std::skipws;
    int version = 0;
    if (!(input >> version))
        return;
    if (version != save_version_)
    {
        incompatible_save_ = true;
        return;
    }

    Data loaded;
    if (!(input >> loaded.show_text_ >> loaded.sound_
                >> loaded.game_over_ >> loaded.active_target_) ||
        loaded.game_over_ < 0 || loaded.game_over_ >= 3 ||
        loaded.active_target_ < 0 || loaded.active_target_ > targets_max_ ||
        (loaded.active_target_ == targets_max_ && !loaded.game_over_))
        return;

    for (auto& color : loaded.target_colors_)
    {
        if (!(input >> color) || color < 0 || color > target_colors_max_)
            return;
    }
    for (auto& color : loaded.choosen_colors_)
    {
        if (!(input >> color) || color < 0 || color >= target_colors_max_)
            return;
    }

    int rows_size = 0;
    if (!(input >> rows_size) || rows_size < 0 || rows_size >= 1024)
        return;
    for (int i = 0; i < rows_size; ++i)
    {
        Row row;
        for (auto& color : row.first)
        {
            if (!(input >> color) || color < 0 || color >= target_colors_max_)
                return;
        }
        for (auto& score : row.second)
        {
            if (!(input >> score) || score < 0 || score > targets_max_)
                return;
        }
        if (row.second[0] + row.second[1] > targets_max_)
            return;
        loaded.rows_.push_back(row);
    }
    input >> std::ws;
    if (!input.eof() || input.bad())
        return;

    show_text_ = loaded.show_text_;
    sound_ = loaded.sound_;
    game_over_ = loaded.game_over_;
    active_target_ = loaded.active_target_;
    target_colors_ = loaded.target_colors_;
    choosen_colors_ = loaded.choosen_colors_;
    rows_ = std::move(loaded.rows_);
}

void main::Data::save(std::ostream& output) const
{
    if (incompatible_save_)
    {
        output.setstate(std::ios::failbit);
        return;
    }

    output << std::dec << std::noboolalpha << std::noshowbase << std::noshowpos;
    output.width(0);
    output << save_version_ << '\n'
           << show_text_ << ' ' << sound_ << ' '
           << game_over_ << ' ' << active_target_ << '\n';
    for (const auto color : target_colors_)
        output << color << ' ';
    output << '\n';
    for (const auto color : choosen_colors_)
        output << color << ' ';
    output << '\n' << rows_.size() << '\n';
    for (const auto& row : rows_)
    {
        for (const auto color : row.first)
            output << color << ' ';
        output << row.second[0] << ' ' << row.second[1] << '\n';
    }
}

void main::Data::reset_all()
{
    incompatible_save_ = false;
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
