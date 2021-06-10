#ifndef SRC_GAME_H
#define SRC_GAME_H

#include "../cross/core/src/stage.h"

#include "data.h"

namespace main
{
    class Game: public core::Stage
    {
    public:
        Game();
        ~Game();
        void Escape() override;

    private:
        void update_view();
        void add_row(const Data::Row& row);
        void game_over();
    };
}

#endif //SRC_GAME_H
