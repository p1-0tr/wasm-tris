#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <string>

#include <EGL/egl.h>

#include "game_logic.hpp"
#include "gfx/shader.hpp"
#include "gfx/sprite.hpp"
#include "gfx/texture.hpp"
#include "gfx/window.hpp"

// retain program state statically - it's the easiest way of communicating the
// required data between consecutive steps of the game loop (driven from
// JS), this relies on NO_EXIT_RUNTIME=1 being set when compiling the program
//
// TODO: consider if there is a cleaner way of handling this
static GameWindow game_window;
static Texture texture;
static ShaderProgram shader_program;
static GameState game_state;

static auto pt_step = std::chrono::steady_clock::now();
static auto ct_step = std::chrono::steady_clock::now();
static int game_speed = 500;

static GLfloat block_color[7][4] = {
                [BlockType::I] = {0.529, 0.808, 0.922, 0.0},
                [BlockType::J] = {0.275, 0.510, 0.706, 0.0},
                [BlockType::L] = {1.000, 0.647, 0.000, 0.0},
                [BlockType::O] = {1.000, 0.843, 0.000, 0.0},
                [BlockType::S] = {0.196, 0.804, 0.196, 0.0},
                [BlockType::T] = {0.416, 0.353, 0.804, 0.0},
                [BlockType::Z] = {0.863, 0.078, 0.235, 0.0},
};

auto draw_board(const GameWindow& game_window,
                const GameState& game_state) -> void {
    const std::vector<std::vector<BlockType>>& board = game_state.get_board();
    const std::array<std::array<int, 2>, 4>& block =
        game_state.get_block_state().shape;
    const BlockType active_block = game_state.get_block_state().type;
    const std::array<int, 2>& bpos = game_state.get_block_state().position;
    const Rotation rotation = game_state.get_block_state().rotation;

    int bdimx = game_window.width / board[0].size();
    int bdimy = game_window.height / board.size();

    GLuint active_tex_unit = texture.use();

    glUseProgram(shader_program.handle());
    shader_program.set_uniform_1i("tex", active_tex_unit);

    for (int y = 0; y < board.size(); ++y) {
        for (int x = 0; x < board[0].size(); ++x) {
            if (board[y][x] != BlockType::EMPTY) {
                SpriteRenderer sr;
                sr.init(x * bdimx, y * bdimy, bdimx, bdimy);
                shader_program.set_uniform_4fv("color",
                                               block_color[board[y][x]]);
                sr.draw();
            }
        }
    }

    for (auto tile : block) {
        auto ntile = apply_rotation(tile, ROTATIONS[rotation]);

        ntile[0] += bpos[0];
        ntile[1] += bpos[1];

        SpriteRenderer sr;
        sr.init(ntile[0] * bdimx, ntile[1] * bdimy, bdimx, bdimy);
        shader_program.set_uniform_4fv("color", block_color[active_block]);
        sr.draw();
    }
}

extern "C" auto user_input(int key) -> void {
    switch (key) {
        case 37: /* left */
            game_state.move({-1, 0});
            break;
        case 38: /* up */
            game_state.rotate();
            break;
        case 39: /* right */
            game_state.move({1, 0});
            break;
        case 40: /* down */
            game_state.move({0, 1});
            break;
        default:
            break;
    }
}

auto render() -> void {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_board(game_window, game_state);

    game_window.update();
}

extern "C" auto next_frame() -> void {
    ct_step = std::chrono::steady_clock::now();

    auto diff = ct_step - pt_step;

    if (std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() >
            game_speed) {
        game_state.notify_time_step();
        pt_step = ct_step;
    }

    bool cleared = game_state.clear_full();

    /* prevent from burning cycles if there was no update */
    if (game_state.get_vec()[0] == 0 && game_state.get_vec()[1] == 0 &&
            game_state.get_block_state().rotation ==
                game_state.get_new_rotation() &&
            !cleared)
        return;

    game_state.progress();

    render();

    game_state.reset_move();
}

auto main(int argc, const char** argv) -> int {
    EGLint attrs[] = {EGL_RED_SIZE,       5,
                      EGL_GREEN_SIZE,     6,
                      EGL_BLUE_SIZE,      5,
                      EGL_ALPHA_SIZE,     EGL_DONT_CARE,  // 8
                      EGL_DEPTH_SIZE,     EGL_DONT_CARE,  // 8
                      EGL_STENCIL_SIZE,   EGL_DONT_CARE,  // 8
                      EGL_SAMPLE_BUFFERS, 0,  // 1
                      EGL_NONE};

    int width = 200, height = 400;

    game_window.create(attrs, width, height);

    std::vector<ShaderDesc> shader_descs(2);
    ShaderProgramDesc program_desc;

    shader_descs[0].file_name = "assets/shaders/tile.vert";
    shader_descs[0].type = GL_VERTEX_SHADER;
    shader_descs[1].file_name = "assets/shaders/tile.frag";
    shader_descs[1].type = GL_FRAGMENT_SHADER;

    program_desc.attributes.push_back("vPosition");

    shader_program.build(shader_descs, program_desc);
    shader_program.use();

    glClearColor(0.f, 0.f, 0.f, 0.f);

    float l = 0.0f, r = width;
    float t = 0.0f, b = height;
    float n = -1.0f, f = 1.0f;

    GLfloat projection[] =
            {2.f / (r - l),      0.f,                0.f,                0.f,
             0.f,                2.f / (t - b),      0.f,                0.f,
             0.f,                0.f,                -2.f / (f - n),     0.f,
             -(r + l) / (r - l), -(t + b) / (t - b), -(f + n) / (f - n), 1.f};

    shader_program.set_uniform_matrix_4fv("projection", projection);

    texture.load("assets/gfx/tile.png");

    glViewport(0, 0, game_window.width, game_window.height);

    game_state.progress();

    render();

    return 0;
}
