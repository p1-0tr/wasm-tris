#include "game_logic.hpp"

auto GameState::clear_full() -> bool {
    bool cleared = false;

    for (auto it = board.begin(); it != board.end(); ++it) {
        bool full = true;

        for (auto tile : *it) full = full && (tile != BlockType::EMPTY);

        if (full) {
            for (auto rit = it - 1; rit != board.begin(); --rit)
                *(rit + 1) = *rit;
            board[0] = std::vector<BlockType>(10, BlockType::EMPTY);

            cleared = true;
        }
    }

    return cleared;
}

auto GameState::check_collisions(const std::array<std::array<int, 2>, 4>& block,
                                 const std::array<int, 2>& vec,
                                 const Rotation rotation) -> CollisionType {
    for (auto tile : block) {
        auto ntile = apply_rotation(tile, ROTATIONS[rotation]);
        ntile[0] += vec[0];
        ntile[1] += vec[1];

        if (ntile[1] >= 0 and ntile[1] >= board.size())
            return CollisionType::BOTTOM;

        if (ntile[0] < 0 or ntile[0] >= board[0].size())
            return CollisionType::SIDE;

        if ((ntile[1] >= 0 and ntile[1] < board.size()) and
            board[ntile[1]][ntile[0]] != BlockType::EMPTY) {
            if (board[ntile[1] - vec[1]][ntile[0]] == BlockType::EMPTY)
                return CollisionType::BOTTOM;
            else
                return CollisionType::SIDE;
        }
    }

    return CollisionType::NONE;
}

auto GameState::progress() -> void {
    bool should_stop = false;
    Rotation current_rotation = block_state.rotation;

    std::array<int, 2> nbpos;
    nbpos[0] = block_state.position[0] + vec[0];
    nbpos[1] = block_state.position[1] + vec[1];

    if (block_state.rotation != new_rotation)
        current_rotation = new_rotation;

    CollisionType collision = check_collisions(block_state.shape, nbpos,
                                               current_rotation);
    if (collision != CollisionType::NONE) {
        if (block_state.rotation != new_rotation) {
            current_rotation = block_state.rotation;
            vec = {0, 1};
        } else if (vec[0] > 0 and vec[1] > 0 and
                   collision == CollisionType::BOTTOM) {
            vec[1] = 0;
        } else {
            vec = {0, 1};
        }

        nbpos[0] = block_state.position[0] + vec[0];
        nbpos[1] = block_state.position[1] + vec[1];

        collision = check_collisions(block_state.shape, nbpos,
                                     current_rotation);
        if (collision != CollisionType::NONE) {
            vec = {0, 0};
            nbpos[0] = block_state.position[0] + vec[0];
            nbpos[1] = block_state.position[1] + vec[1];

            should_stop = true;
        }
    }

    block_state.position = nbpos;

    for (auto tile : block_state.shape) {
        auto ntile = apply_rotation(tile, ROTATIONS[current_rotation]);

        ntile[0] += block_state.position[0];
        ntile[1] += block_state.position[1];

        if (should_stop) board[ntile[1]][ntile[0]] = block_state.type;
    }

    if (should_stop) {
        block_state.position = {5, -1};
        block_state.type = next_block();
        block_state.shape = BLOCKS[block_state.type];
        current_rotation = Rotation::R0;
    }
    vec = {0, 1};
    block_state.rotation = current_rotation;
    new_rotation = current_rotation;
}
