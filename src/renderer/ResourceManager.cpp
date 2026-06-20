#include "ResourceManager.hpp"
#include "ShaderProgram.hpp"

#include <sstream>
#include <fstream>
#include <iostream>

/**
 * 
 * @param __shaderStr 
 * @param message 
 * @return 
 */
static bool __shaderLoadTest__(const std::string &__shaderStr, const char *message)
{
    if (__shaderStr.empty())
    {
#ifdef DEBUG
        std::cerr << message << " shader is no load!" << std::endl;
#endif
        return false;
    }
    return true;
}


rnd::ResourceManager::ResourceManager(const std::string &_res_path)
{
    size_t found = _res_path.find_last_of("/\\");
    this->resources_path = _res_path.substr(0, found);
}

std::string rnd::ResourceManager::getFileString(const std::string &relativePath)
{
    std::ifstream iFile(this->resources_path + "/" + relativePath.c_str(), std::ios::in | std::ios::binary);

    if (!iFile.is_open())
    {
#ifdef DEBUG
    std::cerr << "Failed t oopen file: " << relativePath << std::endl;
#endif
        return std::string{};
    }

    std::stringstream buffer;
    buffer << iFile.rdbuf();
    return buffer.str();
}

rnd::ResourceManager &rnd::ResourceManager::getResourceManager(const std::string &_res_path)
{
    static ResourceManager obj(_res_path);
    return obj;
}

rnd::ResourceManager::shader_ptr_t rnd::ResourceManager::load(
    const std::string &shaderName, 
    const std::string &vertexShaderPath, 
    const std::string &fragmentShaderPath)
{
    //  Получение данных о вертексном шейдере
    std::string vertexStr = getFileString(vertexShaderPath);
    if(!__shaderLoadTest__(vertexStr, "Vertex"))
        return nullptr;

    //  Получение данных о фрагментном шейдере
    std::string fragmentStr = getFileString(fragmentShaderPath);
    if(!__shaderLoadTest__(fragmentStr, "Fragment"))
        return nullptr;

    auto& newShader = this->_container.emplace(shaderName, std::make_shared<ShaderProgram>(vertexStr.c_str(), fragmentStr.c_str())).first->second;
    if (newShader->isCompiled())
        return newShader;

#ifdef DEBUG
    std::cerr << "Can't load shader program:\n" << 
        "Vertex: " << vertexShaderPath << ";\n" <<
        "Fragment: " << fragmentShaderPath << ";\n";
#endif
    return nullptr;
}

rnd::ResourceManager::shader_ptr_t rnd::ResourceManager::get(const std::string &shaderName)
{
    auto it = this->_container.find(shaderName);

    if (it != this->_container.end()) return it->second;

#ifdef DEBUG
    std::cerr << "Can't find shader program: " << shaderName << "\n";
#endif
    return nullptr;
}
