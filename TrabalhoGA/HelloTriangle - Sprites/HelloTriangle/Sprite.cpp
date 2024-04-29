#include "Sprite.h"

// Destructor: libera os recursos alocados para o Sprite
Sprite::~Sprite()
{
    // Libera o Vertex Array Object (VAO) da memória
    glDeleteVertexArrays(1, &VAO);
}

// Inicializa o Sprite com uma textura, posição e escala
void Sprite::inicializar(GLuint texID, glm::vec3 posicao, glm::vec3 escala)
{
    // Define os atributos do Sprite
    this->texID = texID;
    this->posicao = posicao;
    this->escala = escala;

    // Define os vértices do Sprite
    GLfloat vertices[] = {
        //x     y    z    r    g    b    s    t
        //Triângulo 0
        -0.5, 0.5, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0,  //v0
        -0.5, -0.5, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0,  //v1
        0.5, 0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0,   //v2
        //Triângulo 1	
        -0.5, -0.5, 0.0, 1.0, 1.0, 1.0, 0.0, 0.0,  //v1
        0.5, -0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0,  //v3
        0.5, 0.5, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0   //v2
    };

    GLuint VBO;
    // Geração do identificador do VBO
    glGenBuffers(1, &VBO);
    // Conecta o VBO como um buffer de array
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Envia os dados dos vértices para o buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Geração do identificador do VAO (Vertex Array Object)
    glGenVertexArrays(1, &VAO);
    // Vincula o VAO
    glBindVertexArray(VAO);

    // Define os atributos dos vértices
    // Atributo 0 - posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Atributo 1 - cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Atributo 2 - coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    // Desvincula o VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Desvincula o VAO
    glBindVertexArray(0);
}

// Desenha o Sprite
void Sprite::draw()
{
    // Atualiza a matriz de modelo
    atualizar();

    // Vincula a textura
    glBindTexture(GL_TEXTURE_2D, texID);
    // Vincula o VAO
    glBindVertexArray(VAO); 
    // Desenha os triângulos
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // Desvincula a textura
    glBindTexture(GL_TEXTURE_2D, 0); 
    // Desvincula o VAO
    glBindVertexArray(0); 
}

// Move o Sprite para cima
void Sprite::moveUp() {
    posicao.y += 10;
}

// Move o Sprite para baixo
void Sprite::moveDown() {
    posicao.y -= 10;
}

// Move o Sprite para a esquerda
void Sprite::moveLeft()
{
    if (posicao.x >= 0.0) {
        posicao.x -= 0.2f; // Ajuste a velocidade conforme necessário
    }
    else {
        // Se o objeto atingir a borda esquerda da tela, reposicione-o à direita
        posicao.x = 800.0f; 
    }
}

// Atualiza a transformação do Sprite
void Sprite::atualizar()
{
    glm::mat4 model = glm::mat4(1); // matriz identidade
    model = glm::translate(model, posicao);
    model = glm::scale(model, escala);
    shader->setMat4("model", glm::value_ptr(model));
}

// Define a textura do Sprite
void Sprite::setTexture(GLuint texID)
{
    this->texID = texID;
}

// Verifica colisão entre este Sprite e outro Sprite
bool Sprite::checkCollision(Sprite& sprite2)
{
    // Obtem os centros dos sprites
    glm::vec3 center1 = this->posicao; 
    glm::vec3 center2 = sprite2.getPosicao();

    // Obtem os raios dos sprites 
    float radius1 = this->escala.x * 0.5f; 
    float radius2 = sprite2.getEscala().x * 0.5f; 

    // Calcula a distância entre os centros dos sprites
    float distance = glm::length(center1 - center2);

    // Verifique se a distância é menor que a soma dos raios
    return distance < (radius1 + radius2);
}