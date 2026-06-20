#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define DEBUG

#include "renderer/ShaderProgram.hpp"
#include "renderer/ResourceManager.hpp"

#define N 9

GLfloat point[N] = {
    0.0f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f};
GLfloat colors[N] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f};

int gWindowWidth = 640;
int gWindowHeight = 480;


void glfwWindowSizeCallBack(GLFWwindow *pWindow, int width, int height)
{
    gWindowHeight = height;
    gWindowWidth = width;
    glViewport(0, 0, gWindowWidth, gWindowHeight);
}

void glfwKeyCallback(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

GLuint vbo(GLfloat _pos[N])
{
    GLuint _vbo = 0;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * N, _pos, GL_STATIC_DRAW);
    return _vbo;
}

void vertexAttrArray(int index, GLuint _vbo)
{
    glEnableVertexAttribArray(index);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

int main(int argc, char *argv[])
{
    rnd::ResourceManager &manager = rnd::ResourceManager::getResourceManager(argv[0]);

    const char *engine_name = "Delta";
    const char *title_name = "Test1";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(gWindowWidth, gWindowHeight, title_name, NULL, NULL);
    ;
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(window, glfwWindowSizeCallBack);
    glfwSetKeyCallback(window, glfwKeyCallback);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        std::cout << "Can't load GLAD" << std::endl;
        return -1;
    }

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    {
        glClearColor(1, 1, 1, 1);

        auto pDefShaderProgram = manager.load("DefaultShader", "data/shaders/vertex.txt", "data/shaders/fragment.txt");
        if (!pDefShaderProgram)
        {
            std::cerr << "Can't create shader program!\n";
            return -1;
        }

        if (!pDefShaderProgram->isCompiled())
        {
            std::cerr << "Can't create shader programm!" << std::endl;
            return -1;
        }

        GLuint points_vbo = vbo(point);
        GLuint colors_vbo = vbo(colors);

        GLuint vao = 0;

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        vertexAttrArray(0, points_vbo);
        vertexAttrArray(1, colors_vbo);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefShaderProgram->use();
            glBindVertexArray(vao);

            glDrawArrays(GL_TRIANGLES, 0, 3);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}