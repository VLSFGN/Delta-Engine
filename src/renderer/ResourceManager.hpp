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

    /**
     * Синглтон-менеджер, отвечающий за загрузку, кэширование и выдачу ресурсов.
     *
     * В текущей реализации управляет только шейдерными программами (ShaderProgram).
     * Ресурсы загружаются из файлов, пути к которым задаются относительно корневой папки ресурсов.
     * Класс реализован как синглтон (доступ через статический метод getResourceManager).
     */
    class ResourceManager final
    {
    private:
        // --------------------------------------------------------------------
        // Типы-алиасы для удобства
        // --------------------------------------------------------------------
        /** Умный указатель на шейдерную программу. */
        using shader_ptr_t = std::shared_ptr<ShaderProgram>;

        /** Ассоциативный контейнер: имя программы -> указатель на неё. */
        using shader_map_t = std::map<const std::string, shader_ptr_t>;

        /**
         * Хранилище загруженных шейдерных программ.
         * Ключом служит имя, под которым программа зарегистрирована (например, "DefaultShader").
         */
        shader_map_t _container;

        /**
         * Путь к корневой папке с ресурсами (без завершающего слеша).
         * Формируется из пути к исполняемому файлу (argv[0]) путём отсечения имени файла.
         * Все относительные пути в методах load/get строятся относительно этой папки.
         */
        std::string resources_path;

        /**
         * Приватный конструктор (синглтон).
         * @param _res_path Путь к исполняемому файлу (обычно argv[0]). В конструкторе из него извлекается директория, которая становится resources_path.
         */
        ResourceManager(const std::string& _res_path);

        /**
         * Вспомогательный метод для чтения содержимого файла в строку.
         * @param relativePath Относительный путь к файлу (относительно resources_path).
         * @return Содержимое файла в виде std::string, либо пустая строка в случае ошибки.
         */
        std::string getFileString(const std::string& relativePath);

    public:
        // --------------------------------------------------------------------
        // Публичные методы (синглтон, загрузка, получение)
        // --------------------------------------------------------------------


        /**
         * Статический метод доступа к единственному экземпляру менеджера.
         * @param _res_path Путь к исполняемому файлу (argv[0]) – используется для инициализации пути к ресурсам.
         * @return Ссылка на объект ResourceManager.
         */
        static ResourceManager& getResourceManager(const std::string& _res_path);
        
        ~ResourceManager() = default;

        /**
         * Загружает шейдерную программу из двух файлов (вершинный и фрагментный шейдеры).
         *
         * Файлы читаются, затем создаётся объект ShaderProgram.
         * Если компиляция и линковка прошли успешно, программа сохраняется в контейнере.
         *
         * @param shaderName         Имя, под которым программа будет сохранена (например, "DefaultShader").
         * @param vertexShaderPath   Относительный путь к файлу с вершинным шейдером.
         * @param fragmentShaderPath Относительный путь к файлу с фрагментным шейдером.
         * @return shared_ptr на загруженную программу, либо nullptr при неудаче.
         */
        shader_ptr_t load(
            const std::string& shaderName,
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath
        );
        
        /**
         * Получает ранее загруженную шейдерную программу по её имени.
         * @param shaderName Имя программы.
         * @return shared_ptr на программу, либо nullptr, если программа с таким именем не найдена.
         */
        shader_ptr_t get(const std::string& shaderName);


        ResourceManager(const ResourceManager&) = delete;
        ResourceManager(ResourceManager&&) = delete;
        ResourceManager& operator=(const ResourceManager&) = delete;
        ResourceManager& operator=(ResourceManager&&) = delete;
    };

} // namespace rnd

#endif // RESOURCEMANAGER_HPP