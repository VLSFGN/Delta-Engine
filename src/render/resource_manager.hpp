/**
 * 
 */

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP
#pragma once

#include <memory>
#include <string>
#include <map>


namespace rnd
{
    class ShaderProgram;


    class ResourceManager final
    {
    public:
        using shader_ptr_t = std::shared_ptr<ShaderProgram>;

        using shader_map_t = std::map<const std::string, shader_ptr_t>;

    private:
        shader_map_t __shader_container;

        std::string __resource_path;


        ResourceManager(const std::string& resources_path);

        std::string __get_file_string__(const std::string& relative_path);


        shader_ptr_t __load_shader__(
            const std::string name,
            const char *vshader,
            const char *fshader
        );

    public:
        static ResourceManager* getResourceManager(const std::string& resources_path);
        
        ~ResourceManager() = default;

        
        shader_ptr_t load(
            const std::string &shader_name, 
            const std::string &vshader_path, 
            const std::string &fshader_path
        );
        
        shader_ptr_t get_shader(const std::string& shader_name);


        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
    };

} // namespace rnd

#endif // RESOURCEMANAGER_HPP