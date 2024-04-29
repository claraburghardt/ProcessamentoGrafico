#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>
#include "Shader.h"
#include "Sprite.h"

using namespace std;

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
GLuint loadTexture(string texturePath);

// Dimens�es da janela 
const GLuint WIDTH = 800, HEIGHT = 600;

// Vari�veis globais para os sprites do jogo
Sprite bird;
Sprite background;
Sprite tunnelTop1;
Sprite tunnelBottom1;
Sprite tunnelTop2;
Sprite tunnelBottom2;
Sprite tunnelTop3;
Sprite tunnelBottom3;

// Fun��o MAIN
int main()
{
    // Inicializa��o da GLFW
    glfwInit();

    // Configura��o das op��es GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria��o da janela GLFW
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Flappy Bird", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Registro da fun��o de callback de teclado
    glfwSetKeyCallback(window, key_callback);

    // Carregamento dos ponteiros de fun��o da OpenGL (GLAD)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
    }

    // Obtendo as informa��es de vers�o
    const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
    const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
    cout << "Renderer: " << renderer << endl;
    cout << "OpenGL version supported " << version << endl;

    // Habilita��o do teste de profundidade
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS); // a cada ciclo

    // Habilita��o do modo de transpar�ncia (blend - mistura de cores)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Compila��o e linkagem do programa de shader
    Shader shader("../shaders/tex.vs", "../shaders/tex.fs");

    // Carregamento das texturas
    GLuint birdTexture = loadTexture("../textures/bird.png");
    GLuint tunnelTopTexture = loadTexture("../textures/tunnelTop.png");
    GLuint tunnelBottomTexture = loadTexture("../textures/tunnelBottom.png");
    GLuint backgroundTexture = loadTexture("../textures/background.png");
    GLuint backgroundGameOverTexture = loadTexture("../textures/backgroundGameOver.png");

    // Inicializa��o dos sprites
    background.setShader(&shader);
    background.inicializar(backgroundTexture, glm::vec3(400.0, 300.0, 0.0), glm::vec3(1920.0 / 2.0, 1080.0 / 2.0, 1.0));

    bird.setShader(&shader);
    bird.inicializar(birdTexture, glm::vec3(100.0f, HEIGHT / 2.0f, 0.0f), glm::vec3(50.0f, 50.0f, 1.0f));

    tunnelTop1.setShader(&shader);
    tunnelTop1.inicializar(tunnelTopTexture, glm::vec3(900.0f, 500.0f, 0.0f), glm::vec3(100.0f, 350.0f, 1.0f));
    tunnelBottom1.setShader(&shader);
    tunnelBottom1.inicializar(tunnelBottomTexture, glm::vec3(900.0f, 0.0f, 0.0f), glm::vec3(100.0f, 400.0f, 1.0f));

    tunnelTop2.setShader(&shader);
    tunnelTop2.inicializar(tunnelTopTexture, glm::vec3(1200.0f, 500.0f, 0.0f), glm::vec3(100.0f, 250.0f, 1.0f));
    tunnelBottom2.setShader(&shader);
    tunnelBottom2.inicializar(tunnelBottomTexture, glm::vec3(1200.0f, 0.0f, 0.0f), glm::vec3(100.0f, 500.0f, 1.0f));

    tunnelTop3.setShader(&shader);
    tunnelTop3.inicializar(tunnelTopTexture, glm::vec3(1500.0f, 500.0f, 0.0f), glm::vec3(100.0f, 450.0f, 1.0f));
    tunnelBottom3.setShader(&shader);
    tunnelBottom3.inicializar(tunnelBottomTexture, glm::vec3(1500.0f, 0.0f, 0.0f), glm::vec3(100.0f, 300.0f, 1.0f));

    // Ativando o buffer de textura 0 da OpenGL
    glActiveTexture(GL_TEXTURE0);

    shader.Use();

    // Matriz de proje��o paralela ortogr�fica
    glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);

    // Enviando para o shader a matriz como uma var uniform
    shader.setMat4("projection", glm::value_ptr(projection));

    // Matriz de transforma��o do objeto (matriz de modelo)
    shader.setInt("texBuffer", 0);

    // Loop da aplica��o - "game loop"
    while (!glfwWindowShouldClose(window))
    {
        // Checa se houve eventos de input e chama as fun��es de callback correspondentes
        glfwPollEvents();     

        // Configura��o do viewport
        glViewport(0, 0, WIDTH, HEIGHT);

        // Limpa o buffer de cor
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); // cor de fundo
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Desenha todos os sprites
        background.draw();
        bird.draw();
        tunnelTop1.draw();
        tunnelBottom1.draw();
        tunnelTop2.draw();
        tunnelBottom2.draw();
        tunnelTop3.draw();
        tunnelBottom3.draw();

        // Atualiza posi��es dos t�neis
        tunnelTop1.moveLeft();
        tunnelBottom1.moveLeft();

        tunnelTop2.moveLeft();
        tunnelBottom2.moveLeft();

        tunnelTop3.moveLeft();
        tunnelBottom3.moveLeft();

        // Verifica colis�o entre o p�ssaro e os t�neis
        if (bird.checkCollision(tunnelTop1) || bird.checkCollision(tunnelBottom1) ||
            bird.checkCollision(tunnelTop2) || bird.checkCollision(tunnelBottom2) ||
            bird.checkCollision(tunnelTop3) || bird.checkCollision(tunnelBottom3))
        {
            // Se houver colis�o, finaliza todos os sprites
            bird.~Sprite();
            tunnelTop1.~Sprite();
            tunnelBottom1.~Sprite();
            tunnelTop2.~Sprite();
            tunnelBottom2.~Sprite();
            tunnelTop3.~Sprite();
            tunnelBottom3.~Sprite();

            // Atualiza o background para o de game over
            background.setTexture(backgroundGameOverTexture);
        }

        // Troca os buffers da tela
        glfwSwapBuffers(window);
    }

    // Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
    glfwTerminate();
    return 0;
}

// Fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_UP || key == GLFW_KEY_W)
        bird.moveUp();

    if (key == GLFW_KEY_DOWN || key == GLFW_KEY_S)
        bird.moveDown();

}

// Fun��o para carregar texturas
GLuint loadTexture(string texturePath)
{
    GLuint texID;

    // Gera��o do identificador de textura
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    // Configura��o do par�metro WRAPPING nas coords s e t
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Confiruga��o do par�metro FILTERING na minifica��o e magnifica��o da textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);

    if (data)
    {
        if (nrChannels == 3) // jpg, bmp
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        }
        else // png
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texID;
}