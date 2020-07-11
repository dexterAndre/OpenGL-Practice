#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"



int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Setting OpenGL version */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    /* Initializing OpenGL functions */
    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR: glewInit(); not valid" << std::endl;
    }

    std::cout << "[OpenGL Version: " << glGetString(GL_VERSION) << "]" << std::endl;

    /* Setting OpenGL states */
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    {
        /* Geometry content */
        float positions[] = {
            // Positions            // UVs
             0.5f,       0.5f,      1.0f,   1.0f,
            -0.5f,       0.5f,      0.0f,   1.0f,
            -0.5f,      -0.5f,      0.0f,   0.0f,
             0.5f,      -0.5f,      1.0f,   0.0f
        };

        unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3
        };

        /* Buffer setup */
        VertexArray VAO;
        VertexBuffer VBO(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        VAO.AddBuffer(VBO, layout);
        IndexBuffer EBO(indices, 6);

        /* Projection setup */
        glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        /* Shader setup */
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", glm::vec4(0.8f, 0.3f, 0.8f, 1.0f));

        /* Texture setup */
        Texture texture("res/textures/awesomeface.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        /* Cleaning up */
        VAO.Unbind();
        VBO.Unbind();
        EBO.Unbind();
        shader.Unbind();

        /* Renderer setup */
        Renderer renderer;

        /* GUI setup */
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

        /* Interactivity */
        glm::vec3 translationA = glm::vec3(-0.5f, -0.5f, 0.0f);
        glm::vec3 translationB = glm::vec3(0.5f, 0.5f, 0.0f);



        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            /* GUI setup */
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            /* Recalculating MVP matrices */
            {
                glm::mat4 modelA = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvpA = proj * view * modelA;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvpA);
                renderer.Draw(VAO, EBO, shader);
            }

            {
                glm::mat4 modelB = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvpB = proj * view * modelB;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvpB);
                renderer.Draw(VAO, EBO, shader);
            }

            /* GUI content */
            {
                ImGui::SliderFloat3("TranslationA", &translationA.x, -1.0f, 1.0f);
                ImGui::SliderFloat3("TranslationB", &translationB.x, -1.0f, 1.0f);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            /* GUI rendering */
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    /* GUI cleanup */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    /* GLFW cleanup */
    glfwTerminate();
    return 0;
}