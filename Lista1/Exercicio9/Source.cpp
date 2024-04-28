#include <iostream>
#include <string>
#include <assert.h>
#include "Shader.h"
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupGeometry();

// Constantes para as dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
    // Inicialização da GLFW
    glfwInit();

    // Criação da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 9 | Clara Burghardt", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da função de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros das funções da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;

    }

    // Obtendo as informações de versão
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Compilando e buildando o programa de shader
    Shader shader("../../shaders/helloTriangle.vs", "../../shaders/helloTriangle.fs");

    // Gerando um buffer simples, com a geometria de um triângulo
    GLuint VAO = setupGeometry();

    // Enviando a cor desejada (vec4) para o fragment shader
    // Utilizamos a variáveis do tipo uniform em GLSL para armazenar esse tipo de info
    // que não estão nos buffers
    GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
    assert(colorLoc > -1);

    glUseProgram(shader.ID);

    // Loop da aplicação - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glLineWidth(3);
        glPointSize(10);

        glBindVertexArray(VAO);

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 0, 5); // Contorno da casa

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_TRIANGLES, 5, 3); // Telhado

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 8, 4); // Porta

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINE_LOOP, 12, 5); // Janela

        // Desenhar uma cruz no meio da janela
        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 17, 4); // Linhas horizontais da cruz

        glUniform4f(colorLoc, 1.0f, 1.0f, 1.0f, 1.0f);
        glDrawArrays(GL_LINES, 19, 4); // Linhas verticais da cruz

        glBindVertexArray(0);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    // Pede pra OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO);
    // Finaliza a execução da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
    GLfloat vertices[] = {
        // Contorno da casa
        -0.4f, -0.5f, 0.0f,
        0.4f, -0.5f, 0.0f,
        0.4f, 0.5f, 0.0f,
        -0.4f, 0.5f, 0.0f,
        -0.4f, -0.5f, 0.0f,

        // Telhado
        -0.5f, 0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
        0.0f, 0.8f, 0.0f,

        // Porta
        -0.1f, -0.5f, 0.0f,
        0.1f, -0.5f, 0.0f,
        0.1f, 0.0f, 0.0f,
        -0.1f, 0.0f, 0.0f,

        // Janela 
        -0.3f, 0.2f, 0.0f,
        -0.1f, 0.2f, 0.0f,
        -0.1f, 0.4f, 0.0f,
        -0.3f, 0.4f, 0.0f,
        -0.3f, 0.2f, 0.0f,

        // Cruz no meio da janela
        -0.3f, 0.3f, 0.0f,   
        -0.1f, 0.3f, 0.0f,
        -0.2f, 0.2f, 0.0f,   
        -0.2f, 0.4f, 0.0f
    };

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return VAO;
}