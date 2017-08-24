#ifndef SHADER_H
#define SHADER_H

#include <GLES2/gl2.h>

#include <cstdio>

struct ShaderDesc {
    GLenum type;
    std::string file_name;
};

struct ShaderProgramDesc {
    std::vector<std::string> attributes;
};

class Shader {
    GLuint _handle;

   public:
    auto handle() -> GLuint { return _handle; }

    auto compile(GLenum type, const char *shaderSrc) -> int {
        GLuint shader;
        GLint compiled;

        shader = glCreateShader(type);

        if (shader == 0) return 1;

        glShaderSource(shader, 1, &shaderSrc, NULL);

        glCompileShader(shader);

        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled) {
            GLint infoLen = 0;

            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                std::unique_ptr<char[]> infoLog(new char[infoLen]);

                glGetShaderInfoLog(shader, infoLen, NULL, infoLog.get());
                printf("Error compiling shader:\n%s\n", infoLog.get());
            }

            glDeleteShader(shader);

            return 1; /* error */
        }

        _handle = shader;

        return 0;
    }

    virtual ~Shader() { glDeleteShader(_handle); }
};

auto load_file(const std::string file_name) -> std::string {
    std::ifstream file(file_name);
    return std::string((std::istreambuf_iterator<char>(file)),
                       (std::istreambuf_iterator<char>()));
}

class ShaderProgram {
    GLuint _handle;

   public:
    auto handle() -> GLuint { return _handle; }

    auto build(const std::vector<ShaderDesc> sources, ShaderProgramDesc desc)
            -> int {
        GLuint program_object;
        GLint linked;

        // Create the program object
        program_object = glCreateProgram();

        if (program_object == 0) return 1; /* error */

        for (auto shader_desc : sources) {
            std::string src = load_file(shader_desc.file_name);
            Shader shader;

            shader.compile(shader_desc.type, src.c_str());

            glAttachShader(program_object, shader.handle());
        }

        for (int i = 0; i < desc.attributes.size(); ++i)
            glBindAttribLocation(program_object, i, desc.attributes[i].c_str());

        glLinkProgram(program_object);

        glGetProgramiv(program_object, GL_LINK_STATUS, &linked);

        if (!linked) {
            GLint infoLen = 0;

            glGetProgramiv(program_object, GL_INFO_LOG_LENGTH, &infoLen);

            if (infoLen > 1) {
                std::unique_ptr<char[]> infoLog(new char[infoLen]);

                glGetProgramInfoLog(program_object, infoLen, NULL,
                                    infoLog.get());
                printf("Error linking program:\n%s\n", infoLog.get());
            }

            glDeleteProgram(program_object);

            return 1; /* error */
        }

        _handle = program_object;

        return 0;
    }

    auto use() -> void {
        glUseProgram(_handle);
    }

    auto set_uniform_matrix_4fv(std::string name, GLfloat val[]) -> void {
        GLint attr_loc = glGetUniformLocation(_handle, name.c_str());
        glUniformMatrix4fv(attr_loc, 1, GL_FALSE, val);
    }

    auto set_uniform_1i(std::string name, GLint val) -> void {
        glUniform1i(glGetUniformLocation(_handle, name.c_str()), val);
    }

    auto set_uniform_4fv(std::string name, GLfloat val[]) -> void {
        glUniform4fv(glGetUniformLocation(_handle, name.c_str()), 1, val);
    }

    virtual ~ShaderProgram() { glDeleteProgram(_handle); }
};
#endif  // SHADER_H
