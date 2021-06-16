#ifndef SRC_DATA_H
#define SRC_DATA_H

#include <array>
#include <list>
#include <random>
#include <vector>

namespace main
{
    class Data
    {
        static const int targets_max_ = 4;
        static const int target_colors_max_ = 6;
        static const int rows_max_ = 20;
        friend class Menu;
        friend class Game;

    public:
        using Row =
            std::pair<std::array<int, targets_max_>, std::array<int, 2>>;

    public:
        Data();
        ~Data();
        void load();
        void save() const;
        void reset_all();
        void reset_game();

    private:
        bool validate_targets() const;
        std::array<int, 2> calculate_scores() const;

    private:
        bool show_text_;
        bool sound_;
        int game_over_;
        int active_target_;
        Row::first_type target_colors_;
        Row::first_type choosen_colors_;
        std::vector<Row> rows_;
        std::random_device seeder_;
        std::default_random_engine random_;
    };

    extern Data data_;
}

#endif //SRC_DATA_H
