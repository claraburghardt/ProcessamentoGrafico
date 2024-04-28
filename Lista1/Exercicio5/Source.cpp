#include <iostream>
#include <string>
#include <assert.h>
#include "Shader.h" 

using namespace std;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Constantes para as dimensões da janela
const GLuint WIDTH = 800, HEIGHT = 600;

// Função que configura a geometria dos triângulos
int setupGeometry()
{
    // Define as coordenadas dos vértices dos dois triângulos
    GLfloat vertices[] = {
        -0.5, 0.5, 0.0,   // Triângulo 0, vértice 0
        0.0, 0.0, 0.0,    // Triângulo 0, vértice 1
        0.5, 0.5, 0.0,    // Triângulo 0, vértice 2
        0.0, 0.0, 0.0,    // Triângulo 1, vértice 3
        -0.5, -0.5, 0.0,  // Triângulo 1, vértice 4
        0.5, -0.5, 0.0    // Triângulo 1, vértice 5
    };

    // Geração do identificador do VBO para armazenar os dados dos vértices
    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    // Conecta o buffer de array ao buffer de dados atual
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Envia os dados dos vértices para o buffer da OpenGL
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Geração do identificador do VAO (Vertex Array Object) para encapsular o estado do buffer de vértices
    glGenVertexArrays(1, &VAO);
    // Vincula o VAO
    glBindVertexArray(VAO);
    // Define os atributos dos vértices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // Desvincula o VBO e o VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO; // Retorna o identificador do VAO
}

// Função principal
int main()
{
    glfwInit();

    // Configuração da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 5 | Clara Burghardt", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    // Verifica se o GLAD foi inicializado corretamente
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    GLuint VAO = setupGeometry(); // Configura a geometria dos triângulos

    Shader shader("../shaders/helloTriangle.vs", "../shaders/helloTriangle.fs"); // Compila e carrega os shaders

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Limpa o buffer de cor
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);

        // Desenha os triângulos com diferentes modos
        glDrawArrays(GL_TRIANGLES, 0, 6); // Preenchido (A)
        glDrawArrays(GL_LINE_LOOP, 0, 3); // Contorno do primeiro triângulo (B)
        glDrawArrays(GL_LINE_LOOP, 3, 3); // Contorno do segundo triângulo (B)
        glDrawArrays(GL_POINTS, 0, 6);    // Pontos (C)
        glPointSize(10.0f);               // Define o tamanho dos pontos (C)

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO); // Deleta o VAO

    glfwTerminate(); // Finaliza GLFW

    return 0;
}

// Função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}