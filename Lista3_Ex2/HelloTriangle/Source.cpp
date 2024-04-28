#include <iostream>
#include <string>
#include <cassert>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Common/include/Shader.h"

using namespace std;

// Prot�tipos das fun��es
int setupGeometry();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;
const float moveSpeed = 10.0f; // Velocidade de movimento

// N�mero de v�rtices do tri�ngulo
const int nPoints = 3;
const float pi = 3.14159;

// Posi��o inicial da geometria
glm::vec2 position(WIDTH / 2.0f, HEIGHT / 2.0f);

int main()
{
    // Inicializa��o da GLFW
    glfwInit();

    // Cria��o da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 2 | Clara Burghardt", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Fazendo o registro da fun��o de callback para a janela GLFW
    glfwSetKeyCallback(window, key_callback);

    // GLAD: carrega todos os ponteiros d fun��es da OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    // Obtendo as informa��es de vers�o
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Compilando e buildando o programa de shader
    Shader shader("../shaders/ortho.vs", "../shaders/ortho.fs");

    // Gerando um buffer simples, com a geometria de um tri�ngulo
    GLuint VAO = setupGeometry();

    // Enviando a cor desejada (vec4) para o fragment shader
    GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
    assert(colorLoc > -1);

    // Defini��o das cores
    glm::vec4 backgroundColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    glm::vec4 triangleColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glUseProgram(shader.ID);

    // Criando a matriz de proje��o usando a GLM
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), 0.0f, static_cast<float>(HEIGHT), -1.0f, 1.0f); // Orto
    GLint projLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Loop da aplica��o - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a); // Cor de fundo preto
        glClear(GL_COLOR_BUFFER_BIT);

        // Desenha o tri�ngulo
        glBindVertexArray(VAO);

        glm::mat4 model = glm::mat4(1); // Matriz identidade
        model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
        model = glm::scale(model, glm::vec3(100.0f)); // Aumenta o tamanho do tri�ngulo
        shader.setMat4("model", glm::value_ptr(model));

        glUniform4fv(colorLoc, 1, glm::value_ptr(triangleColor));
        glDrawArrays(GL_TRIANGLES, 0, nPoints);

        glBindVertexArray(0);

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }
    // Pede pra OpenGL desalocar os buffers
    glDeleteVertexArrays(1, &VAO);
    // Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

// Fun��o de callback de teclado 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch (key) {
        case GLFW_KEY_W:
        case GLFW_KEY_UP:
            position.y += moveSpeed;
            break;
        case GLFW_KEY_S:
        case GLFW_KEY_DOWN:
            position.y -= moveSpeed;
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            position.x -= moveSpeed;
            break;
        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            position.x += moveSpeed;
            break;
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        default:
            break;
        }
    }
}

int setupGeometry()
{
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,  // Vertice 1
         0.5f, -0.5f, 0.0f,  // Vertice 2
         0.0f,  0.5f, 0.0f   // Vertice 3
    };

    GLuint VBO, VAO;

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Gera��o do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Para cada atributo do vertice, criamos um "AttribPointer" 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}