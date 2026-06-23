/**
 * 
 */

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP
#pragma once

#include <glad/glad.h>


namespace rnd
{
    class ShaderProgram final
    {
    private:
        bool __is_compiled = false;

        GLuint __ID = 0;


        bool createShader(const char *source, const GLint type, GLuint& shader_id);

        void attachShader(GLuint vshader, GLuint fshader);
        
        void swapData(rnd::ShaderProgram &other) noexcept;

    public:
        ShaderProgram(const char *vshader_path, const char *fshader_path);

        ShaderProgram(ShaderProgram&& other) noexcept;

        ShaderProgram &operator=(ShaderProgram &&other) noexcept;

        ~ShaderProgram();


        bool isCompiled() const;

        void use() const;


        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
    };

} // namespace rnd


#endif //SHADERPROGRAM_HPP