#include "TestUniformColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"

test::TestUniformColor::TestUniformColor()
	: m_UniformColor { 0.2f, 0.3f, 0.8f, 1.0f }
{
}

test::TestUniformColor::~TestUniformColor()
{
}

void test::TestUniformColor::OnUpdate(float deltaTime)
{
}

void test::TestUniformColor::OnRender()
{
	GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));


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

    /* Shader setup */
    Shader shader("res/shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", glm::vec4(0.8f, 0.3f, 0.8f, 1.0f));

    /* Cleaning up */
    VAO.Unbind();
    VBO.Unbind();
    EBO.Unbind();
    shader.Unbind();




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
}

void test::TestUniformColor::OnImGuiRender()
{
	ImGui::ColorEdit4("Clear Color", m_UniformColor);
}
