#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

class Sprite
{
public:
    Sprite() {}
    ~Sprite(); // Libera os recursos alocados para o Sprite

    // Inicializa o Sprite com uma textura, posi��o padr�o e escala padr�o
    void inicializar(GLuint texID, glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), glm::vec3 escala = glm::vec3(1.0, 1.0, 1.0));
    
    // Desenha o Sprite
    void draw();

    // Move o Sprite para cima
    void moveUp();

    // Move o Sprite para baixo
    void moveDown();

    // Move o Sprite para a esquerda
    void moveLeft();

    // Define a textura do Sprite
    void setTexture(GLuint texID);

    // Define o programa de shader que o Sprite ir� usar
    inline void setShader(Shader* shader) { this->shader = shader; }

    // Retorna a posi��o do Sprite
    inline glm::vec3 getPosicao() const { return posicao; }

    // Retorna a escala do Sprite
    inline glm::vec3 getEscala() const { return escala; }

    // Verifica colis�o entre este Sprite e outro Sprite
    bool checkCollision(Sprite& sprite2);

protected:
    // Atualiza a transforma��o do Sprite
    void atualizar();

    GLuint VAO; //Identificador do Vertex Array Object (VAO)
    GLuint texID; //Identificador do buffer de textura

    // Vari�veis com as informa��es para aplicar as transforma��es no objeto
    glm::vec3 posicao, escala;

    // Uma refer�ncia ao programa de shader que a sprite ir� usar para seu desenho
    Shader* shader;
};


