#include <iostream>
#include "ShaderProgram.hpp"

#define DEBUG

using funcIv_t = PFNGLGETSHADERIVPROC;
using funcLog_t = PFNGLGETSHADERINFOLOGPROC;


static constexpr int SIZE_BUFFER = 1024;


/**
 * Функция для проверки для различных этапов работы с шейдерами, таких как
 * компиляция и линковка, на ошибки
 * @param funciv
 * @param funcInfoLog 
 * @param shaderID 
 * @param status
 * @param warning
 * @return
 */
static bool __shaderErrorTest__(
    funcIv_t funciv, 
    funcLog_t funcInfoLog, 
    GLenum status,
    GLint shaderID,
    const char *warning)
{
    GLint success;
    funciv(shaderID, status, &success);
    
    if (!success)
    {   
        GLchar infoLog[SIZE_BUFFER];
        
        funcInfoLog(shaderID, SIZE_BUFFER, nullptr, infoLog);

#ifdef DEBUG
        std::cerr << "ERROR::SHADER: " << warning << " time error:\n" << infoLog << std::endl;
#endif
    }
    return static_cast<bool>(success);
}



void rnd::ShaderProgram::attachShader(GLuint vertexShaderID, GLuint fragmentShaderID)
{
    this->_m_ID = glCreateProgram();
    glAttachShader(_m_ID, vertexShaderID);
    glAttachShader(_m_ID, fragmentShaderID);
    glLinkProgram(_m_ID);
}

void rnd::ShaderProgram::swapData(ShaderProgram &other)
{
    this->_m_ID = other._m_ID;
    this->_is_compiled = other._is_compiled;

    other._m_ID = 0;
    other._is_compiled = false;
}

bool rnd::ShaderProgram::createShader(const char *source, const GLint shaderType, GLuint &shaderID)
{
    shaderID = glCreateShader(shaderType);
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    return __shaderErrorTest__(glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS, shaderID, "Compile");
}

rnd::ShaderProgram::ShaderProgram(const char *vertex_shader, const char *fragment_shader)
{
    GLuint vertexShaderID;
    if (!createShader(vertex_shader, GL_VERTEX_SHADER, vertexShaderID))
    {
#ifdef DEBUG
        std::cerr << "VERTEX::SHADER compile time error" << std::endl;
#endif
        return;
    }

    GLuint fragmentShaderID;
    if (!createShader(fragment_shader, GL_FRAGMENT_SHADER, fragmentShaderID))
    {
#ifdef DEBUG
        std::cerr << "FRAGMENT::SHADER compile time error" << std::endl;
#endif
        glDeleteShader(vertexShaderID);
        return;
    }

    attachShader(vertexShaderID, fragmentShaderID);

    if (__shaderErrorTest__(glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS, _m_ID, "link"))
        this->_is_compiled = true;

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

rnd::ShaderProgram::ShaderProgram(ShaderProgram &&other)
{
    swapData(other);
}

rnd::ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(this->_m_ID);
}

bool rnd::ShaderProgram::isCompiled() const
{
    return this->_is_compiled;
}

void rnd::ShaderProgram::use() const
{
    glUseProgram(_m_ID);
}

rnd::ShaderProgram & rnd::ShaderProgram::operator=(ShaderProgram &&other)
{
    glDeleteProgram(this->_m_ID);

    swapData(other);

    return *this;
}