#ifndef TEXTURE_H
#define TEXTURE_H

#include <GLES2/gl2.h>
#include <SOIL.h>

class Texture {
    GLuint _handle;

   public:
    auto load(const char* name) -> void {
        glGenTextures(1, &_handle);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _handle);

        int width, height;
        unsigned char* image =
                SOIL_load_image(name, &width, &height, 0, SOIL_LOAD_RGBA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, image);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        SOIL_free_image_data(image);
    }

    virtual ~Texture() { glDeleteTextures(1, &_handle); }

    auto use() -> GLuint {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _handle);

        return GL_TEXTURE0;
    }
};
#endif  // TEXTURE_H
