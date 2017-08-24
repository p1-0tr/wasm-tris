#ifndef SPRITE_H
#define SPRITE_H

#include <GLES2/gl2.h>

#include "shader.hpp"
#include "window.hpp"

class SpriteRenderer {
    GLuint shape;

   public:
    auto init(float x, float y, float w, float h) -> void {
        GLfloat vertices[] = {x,     y,     0.f, 0.f,
                              x + w, y,     1.f, 0.f,
                              x,     y + h, 0.f, 1.f,
                              x,     y + h, 0.f, 1.f,
                              x + w, y + h, 1.f, 1.f,
                              x + w, y,     1.f, 0.f};

        glGenBuffers(1, &this->shape);

        glBindBuffer(GL_ARRAY_BUFFER, this->shape);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,
                     GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, this->shape);
        glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    auto draw() -> void {
        glBindBuffer(GL_ARRAY_BUFFER, this->shape);
        glVertexAttribPointer(0, 4, GL_FLOAT, 0, 0, 0);
        glEnableVertexAttribArray(0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual ~SpriteRenderer() { glDeleteBuffers(1, &shape); }
};
#endif  // SPRITE_H
