#include "resource_manager.hpp"
#include "objects/shaders/shader_program.hpp"

#include <sstream>
#include <fstream>
#include <iostream>


static bool __shaderLoadTest__(const std::string &__shaderStr, const char *message)
{
    if (__shaderStr.empty())
    {
        std::cerr << message << " shader is no load!" << std::endl;
        return false;
    }
    return true;
}



rnd::ResourceManager::ResourceManager(const std::string &resources_path)
{
    size_t found = resources_path.find_last_of("/\\");
    this->__resource_path = resources_path.substr(0, found);
}


std::string rnd::ResourceManager::__get_file_string__(const std::string &relative_path)
{
    std::ifstream iFile(this->__resource_path + "/" + relative_path.c_str(), std::ios::in | std::ios::binary);

    if (!iFile.is_open())
    {
        std::cerr << "Failed to open file: " << relative_path << std::endl;
        return std::string{};
    }

    std::stringstream buffer;
    buffer << iFile.rdbuf();
    
    iFile.close();

    return buffer.str();
}


rnd::ResourceManager::shader_ptr_t rnd::ResourceManager::__load_shader__(
    const std::string name, 
    const char *vshader, 
    const char *fshader)
{
    auto shader_prog = this->__shader_container.emplace(name, 
        std::make_shared<ShaderProgram>(vshader, fshader)
    ).first->second;

    if (shader_prog->isCompiled())
        return shader_prog;

    std::cerr << "Can't load shader program:\n" << 
        "Vertex: " << vshader << ";\n" <<
        "Fragment: " << fshader << ";\n";

    return nullptr;
}


rnd::ResourceManager* rnd::ResourceManager::getResourceManager(const std::string &resources_path)
{
    static ResourceManager obj(resources_path);
    return &obj;
}


rnd::ResourceManager::shader_ptr_t rnd::ResourceManager::load(
    const std::string &shader_name, 
    const std::string &vshader_path, 
    const std::string &fshader_path)
{
    std::string vertex_str = __get_file_string__(vshader_path);
    if(!__shaderLoadTest__(vertex_str, "Vertex"))
        return nullptr;

    std::string fragment_str = __get_file_string__(fshader_path);
    if(!__shaderLoadTest__(fragment_str, "Fragment"))
        return nullptr;

    return __load_shader__(shader_name, vertex_str.c_str(), fragment_str.c_str());
}


rnd::ResourceManager::shader_ptr_t rnd::ResourceManager::get_shader(const std::string &shader_name)
{
    auto it = this->__shader_container.find(shader_name);

    if (it != this->__shader_container.end()) return it->second;

    std::cerr << "Can't find shader program: " << shader_name << "\n";

    return nullptr;
}
