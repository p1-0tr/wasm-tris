#ifndef GAME_DATA_H
#define GAME_DATA_H

#include <array>

enum BlockType {
    I = 0,
    J,
    L,
    O,
    S,
    T,
    Z,
    EMPTY,
};

static std::array<std::array<std::array<int, 2>, 4>, 7> BLOCKS = {{
                [BlockType::I] = {{{0, -2}, {0, -1}, {0, 0}, {0, 1}}},
                [BlockType::J] = {{{-1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
                [BlockType::L] = {{{1, -1}, {-1, 0}, {0, 0}, {1, 0}}},
                [BlockType::O] = {{{0, -1}, {1, -1}, {0, 0}, {1, 0}}},
                [BlockType::S] = {{{0, -1}, {1, -1}, {-1, 0}, {0, 0}}},
                [BlockType::T] = {{{0, -1}, {-1, 0}, {0, 0}, {1, 0}}},
                [BlockType::Z] = {{{-1, -1}, {0, -1}, {0, 0}, {1, 0}}},
}};

enum Rotation {
    R0,
    R90,
    R180,
    R270,
};

static std::array<std::array<std::array<int, 2>, 2>, 4> ROTATIONS = {{
                [Rotation::R0] = {{{1, 0}, {0, 1}}},
                [Rotation::R90] = {{{0, -1}, {1, 0}}},
                [Rotation::R180] = {{{-1, 0}, {0, -1}}},
                [Rotation::R270] = {{{0, 1}, {-1, 0}}},
}};
#endif  // GAME_DATA_H
