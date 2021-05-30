#ifndef SRC_GAME_H
#define SRC_GAME_H

#include "../cross/core/src/stage.h"

namespace main
{
    class Game: public core::Stage
    {
    public:
        Game();
        ~Game();
        void Escape() override;
    };
}

#endif //SRC_GAME_H
