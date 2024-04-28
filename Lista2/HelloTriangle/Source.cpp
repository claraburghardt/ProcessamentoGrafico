#include <iostream>
#include <string>
#include <assert.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../../Common/include/Shader.h"

using namespace std;

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupGeometry();
void drawScene(int VAO);

int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exercicio 4 | Clara Burghardt", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Compilando e buildando o programa de shader
	Shader shader("../shaders/ortho.vs", "../shaders/ortho.fs");

	// Gerando um buffer simples, com a geometria de um triângulo
	GLuint VAO = setupGeometry();
	
	shader.Use();

	//Matriz de projeção paralela ortográfica
	//glm::mat4 projection = glm::ortho(-10.0, 10.0, -10.0, 10.0, -1.0, 1.0); // Exercicio 1
	//glm::mat4 projection = glm::ortho(0.0, 800.0, 600.0, 0.0, -1.0, 1.0); // Exercicio 2
	glm::mat4 projection = glm::ortho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
	//Enviando para o shader a matriz como uma var uniform
	shader.setMat4("projection", glm::value_ptr(projection));
	
	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		//glViewport(0, 0, width, height);
		
		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		// Exercicio 4
		glViewport(width / 2, height / 2, width / 2, height / 2); // unidades de tela: pixel
		drawScene(VAO);

		/*// Exercicio 5
		// Primeira viewport (quadrante inferior esquerdo)
		glViewport(0, 0, width / 2, height / 2); // unidades de tela: pixel
		drawScene(VAO);

		// Segunda viewport (quadrante inferior direito)
		glViewport(width / 2, 0, width / 2, height / 2); // unidades de tela: pixel
		drawScene(VAO);

		// Terceira viewport (quadrante superior esquerdo)
		glViewport(0, height / 2, width / 2, height / 2); // unidades de tela: pixel
		drawScene(VAO);

		// Quarta viewport (quadrante superior direito)
		glViewport(width / 2, height / 2, width / 2, height / 2); // unidades de tela: pixel
		drawScene(VAO);*/
	
		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do triângulo 
	GLfloat vertices[] = {
		//Triangulo 0
		-0.5, 0.5, 0.0, 1.0, 0.0, 0.0,//v0
		 0.0, 0.0, 0.0, 0.0, 1.0, 0.0,//v1
		 0.5, 0.5, 0.0, 0.0, 0.0, 1.0,//v2 
		//Triangulo 1
		 0.0, 0.0, 0.0, 1.0, 1.0, 0.0,//v3
		-0.5,-0.5, 0.0, 0.0, 1.0, 1.0,//v4
		 0.5,-0.5, 0.0, 1.0, 0.0, 1.0,//v5 
	};

	GLuint VBO, VAO;
	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo)	
	//Atributo 0 - posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo 1 - cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return VAO;
}

void drawScene(int VAO)
{
	glBindVertexArray(VAO); //Conectando ao buffer de geometria

	//1 - Polígono Preenchido
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//2 - Polígono com contorno
	glDrawArrays(GL_LINE_LOOP, 0, 3);
	glDrawArrays(GL_LINE_LOOP, 3, 3);

	//3 - Apenas os vértices dos polígonos
	glDrawArrays(GL_POINTS, 0, 6);

	glBindVertexArray(0); //Desconectando o buffer de geometria
}