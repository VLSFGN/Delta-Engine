/**
 * 
 */

#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP
#pragma once

#include <glad/glad.h>



/**
 * Пространство имен для программ, подпрограмм,
 * классов, объектов и прочих сущностей, связанных
 * с рендером картинки
 */
namespace rnd
{
    /**
     * Класс для управления шейдерной программой.
     * Создаёт программу из вершинного и фрагментного шейдеров,
     * выполняет компиляцию и линковку, предоставляет методы
     * для активации и проверки состояния.
     */
    class ShaderProgram final
    {
        /**
         * Флаг, указывающий, успешно ли скомпилирована и слинкована программа.
         */
        bool _is_compiled = false;

        /**
         * Идентификатор (ID) шейдерной программы в OpenGL.
         */
        GLuint _m_ID = 0;

        /**
         * Создаёт и компилирует шейдер заданного типа из исходного кода.
         * @param source      Исходный код шейдера (строка с GLSL).
         * @param shaderType  Тип шейдера (GL_VERTEX_SHADER или GL_FRAGMENT_SHADER).
         * @param shaderID    Ссылка на переменную, куда будет записан ID созданного шейдера.
         * @return true, если компиляция прошла успешно, иначе false.
         */ 
        bool createShader(const char *source, const GLint shaderType, GLuint& shaderID);

        /**
         * Прикрепляет вершинный и фрагментный шейдеры к программе и выполняет линковку.
         * @param vs  ID вершинного шейдера.
         * @param fs  ID фрагментного шейдера.
         * @return    ID слинкованной программы.
         */
        void attachShader(GLuint vs, GLuint fs);
        
        /**
         * Обменивает данные (идентификатор программы и флаг компиляции) между двумя объектами.
         * @param other Другой объект ShaderProgram, с которым производится обмен.
         */
        inline void swapData(rnd::ShaderProgram &other);

    public:
        /**
         * Конструктор, создающий шейдерную программу из вершинного и фрагментного шейдеров.
         * @param vertex_shader   Исходный код вершинного шейдера.
         * @param fragment_shader Исходный код фрагментного шейдера.
         */
        ShaderProgram(const char *vertex_shader, const char* fragment_shader);

        /**
         * Конструктор перемещения шейдерной программы.
         * @param other Перемещаемый объект.
         */
        ShaderProgram(ShaderProgram&& other);

        ~ShaderProgram();

        /**
         * Проверка, успешно ли скомпилирована и слинкована шейдерная программа.
         * @return true, если программа готова к использованию, иначе false.
         */
        bool isCompiled() const;

        /**
         * Активирует шейдерную программу для использования в конвейере рендеринга.
         */
        void use() const;

        /**
         * Оператор перемещающего присваивания.
         * @param other Перемещаемый объект.
         * @return      Ссылка на текущий объект.
         */
        ShaderProgram &operator=(ShaderProgram &&other);

        
        // Удаленные методы и конструкторы

        ShaderProgram() = delete;
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
    };

} // namespace rnd


#endif //SHADERPROGRAM_HPP