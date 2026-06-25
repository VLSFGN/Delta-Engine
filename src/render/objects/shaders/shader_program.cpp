#include "../../../log/logger.hpp"
#include "shader_program.hpp"


using funcIv_t = PFNGLGETSHADERIVPROC;
using funcLog_t = PFNGLGETSHADERINFOLOGPROC;


static constexpr int SIZE_BUFFER = 1024;


static bool __shaderErrorTest__(
    funcIv_t funciv, 
    funcLog_t func_info_log, 
    GLenum status,
    GLint shader_id,
    const char *warning)
{
    GLint success;
    funciv(shader_id, status, &success);
    
    if (!success)
    {   
        GLchar infoLog[SIZE_BUFFER];
        
        func_info_log(shader_id, SIZE_BUFFER, nullptr, infoLog);

        Logger::getLogger()->clog(LOG_LEVEL_ERROR, "ERROR::SHADER: %s time error: %s\n", warning, infoLog);
    }
    return static_cast<bool>(success);
}



void rnd::ShaderProgram::attachShader(GLuint vshader, GLuint fshader)
{
    this->__ID = glCreateProgram();
    glAttachShader(__ID, vshader);
    glAttachShader(__ID, fshader);
    glLinkProgram(__ID);
}


void rnd::ShaderProgram::swapData(ShaderProgram &other)
{
    this->__ID = other.__ID;
    this->__is_compiled = other.__is_compiled;

    other.__ID = 0;
    other.__is_compiled = false;
}


bool rnd::ShaderProgram::createShader(const char *source, const GLint type, GLuint &shader_id)
{
    shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &source, nullptr);
    glCompileShader(shader_id);

    return __shaderErrorTest__(glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS, shader_id, "Compile");
}


rnd::ShaderProgram::ShaderProgram(const char *vshader_path, const char *fshader_path)
{
    GLuint vshader_id;
    if (!createShader(vshader_path, GL_VERTEX_SHADER, vshader_id))
    {
        Logger::getLogger()->clog(LOG_LEVEL_ERROR, "VERTEX::SHADER compile time error");
        return;
    }

    GLuint fshader_id;
    if (!createShader(fshader_path, GL_FRAGMENT_SHADER, fshader_id))
    {
        Logger::getLogger()->clog(LOG_LEVEL_ERROR, "FRAGMENT::SHADER compile time error");
        glDeleteShader(vshader_id);
        return;
    }

    attachShader(vshader_id, fshader_id);

    if (__shaderErrorTest__(glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS, __ID, "link"))
        this->__is_compiled = true;

    glDeleteShader(vshader_id);
    glDeleteShader(fshader_id);
}


rnd::ShaderProgram::ShaderProgram(ShaderProgram &&other)
{
    swapData(other);
}


rnd::ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(this->__ID);
}


bool rnd::ShaderProgram::isCompiled() const
{
    return this->__is_compiled;
}


void rnd::ShaderProgram::use() const
{
    glUseProgram(__ID);
}


rnd::ShaderProgram & rnd::ShaderProgram::operator=(ShaderProgram &&other)
{
    glDeleteProgram(this->__ID);

    swapData(other);

    return *this;
}