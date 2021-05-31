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
        void reset();

    private:
        bool validate_targets() const;
        std::array<int, 2> calculate_scores() const;

    private:
        int active_target_;
        Row::first_type target_colors_;
        Row::first_type choosen_colors_;
        std::vector<Row> rows_;
        bool game_over_;
        std::random_device seeder_;
        std::default_random_engine random_;
    };

    extern Data data_;
}

#endif //SRC_DATA_H
