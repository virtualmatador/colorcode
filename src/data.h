#ifndef SRC_DATA_H
#define SRC_DATA_H

#include <array>
#include <list>
#include <random>

namespace main
{
    class Data
    {
        static const int targets_max_ = 4;
        friend class Menu;
        friend class Game;
    public:
        Data();
        ~Data();
        void Load();
        void Save();
        void Reset();

    private:
        int active_target_;
        bool sound_;
        std::random_device seeder_;
        std::default_random_engine random_;
    };

    extern Data data_;
}

#endif //SRC_DATA_H
