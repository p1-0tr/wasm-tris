#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <chrono>
#include <random>
#include <vector>

#include "game_data.hpp"

inline auto apply_rotation(std::array<int, 2> point,
                           std::array<std::array<int, 2>, 2> rotation)
        -> std::array<int, 2> {
    std::array<int, 2> npoint;

    npoint[0] = rotation[0][0] * point[0] + rotation[0][1] * point[1];
    npoint[1] = rotation[1][0] * point[0] + rotation[1][1] * point[1];

    return npoint;
}

inline auto next_rotation(Rotation r) -> Rotation {
    return (Rotation)((r + 1) % 4);
}

inline auto prev_rotation(Rotation r) -> Rotation {
    return (Rotation)((r - 1) % 4);
}

enum CollisionType {
    NONE = 0,
    SIDE,
    BOTTOM,
};

struct BlockState {
    BlockType type;
    std::array<std::array<int, 2>, 4> shape;
    std::array<int, 2> position;
    Rotation rotation;
};

class GameState {
    std::vector<std::vector<BlockType>> board;
    std::array<int, 2> vec;
    Rotation new_rotation;

    BlockState block_state;

    std::mt19937 generator;
    std::uniform_int_distribution<int> distribution;

   public:
    GameState() {
        board = std::vector<std::vector<BlockType>>(20,
                    std::vector<BlockType>(10, BlockType::EMPTY));

        block_state.position = {5, -1};
        vec = {0, 1};

        block_state.rotation = Rotation::R0;
        new_rotation = Rotation::R0;

        generator = std::mt19937(
                std::chrono::duration_cast<std::chrono::microseconds>(
                    std::chrono::steady_clock::now().time_since_epoch()
                ).count());
        distribution = std::uniform_int_distribution<int>(BlockType::I,
                                                          BlockType::Z);

        block_state.type = next_block();
        block_state.shape = BLOCKS[block_state.type];
    }

    auto get_board() const ->
            const std::vector<std::vector<BlockType>>& {
        return board;
    }

    auto get_vec() const -> const std::array<int, 2>& {
        return vec;
    }

    auto get_new_rotation() const -> Rotation {
        return new_rotation;
    }

    auto get_block_state() const -> const BlockState& {
        return block_state;
    }

    auto notify_time_step() -> void {
        vec[1] = 1;
    }

    auto reset_move() -> void {
        vec = {0, 0};
    }

    // TODO: change this to collect moves into a queue
    auto move(const std::array<int, 2> _vec) -> void {
        vec = _vec;
    }

    auto rotate() -> void {
        new_rotation = next_rotation(block_state.rotation);
    }

    auto next_block() -> BlockType {
        return (BlockType)distribution(generator);
    }

    auto clear_full() -> bool;

    auto check_collisions(const std::array<std::array<int, 2>, 4>& block,
                          const std::array<int, 2>& vec,
                          const Rotation rotation) -> CollisionType;

    auto progress() -> void;

};
#endif  // GAME_LOGIC_H
