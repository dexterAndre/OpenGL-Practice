#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
//#include "VertexBufferLayout.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

class VertexBufferLayout;

class Renderer
{
private:

public:
    void Clear() const;
    void Draw(const VertexArray& VAO, const IndexBuffer& EBO, const Shader& shader) const;
};