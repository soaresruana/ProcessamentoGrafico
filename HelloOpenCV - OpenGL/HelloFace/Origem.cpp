#include "Shader.h"

#include "opencv2/objdetect.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include <iostream>
#include <string>
#include <assert.h>

// GLM
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


using namespace cv;
using namespace std;


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções - TÁ NA HORA DE REFATORAR ISSO, NÃO ACHAM? 
int setupSprite();
void drawText(Mat& image);
GLuint loadTexture(Mat* img);

void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 640, HEIGHT = 480;

glm::vec3 facePos(0.0, 0.0, 0.0);

int main()
{
	//Coisas do OpenCV
	VideoCapture capture;
	Mat frame, image;
	string inputName;
	bool tryflip = true;
	CascadeClassifier cascade, nestedCascade;
	double scale = 1.0;
	string cascadeName, nestedCascadeName;

	cascadeName = "data/haarcascades/haarcascade_frontalface_alt.xml";
	nestedCascadeName = "data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";

	if (!nestedCascade.load(samples::findFileOrKeep(nestedCascadeName)))
		cerr << "WARNING: Could not load classifier cascade for nested objects" << endl;
	if (!cascade.load(samples::findFile(cascadeName)))
	{
		cerr << "ERROR: Could not load classifier cascade" << endl;
		return -1;
	}
	//----------------------------------



	// Inicialização da GLFW
	glfwInit();

	//Muita atenção aqui: alguns ambientes não aceitam essas configurações
	//Você deve adaptar para a versão do OpenGL suportada por sua placa
	//Sugestão: comente essas linhas de código para desobrir a versão e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello OpenCV - OpenGL!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	// Compilando e buildando o programa de shader
	Shader shader("../shaders/sprite.vs", "../shaders/sprite.fs");

	// Gerando uma geometria de quadrilátero com coordenadas de textura
	GLuint VAO = setupSprite();

	Mat mask1 = imread("data/teste.png", IMREAD_UNCHANGED/*IMREAD_COLOR*/);
	GLuint texID2 = loadTexture(&mask1);

	// Ativando o shader
	glUseProgram(shader.ID);

	// Associando com o shader o buffer de textura que conectaremos
	// antes de desenhar com o bindTexture
	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);

	//Criando a matriz de projeção usando a GLM
	glm::mat4 projection = glm::mat4(1); //matriz identidade
	projection = glm::ortho(0.0, (double)WIDTH, 0.0, (double)HEIGHT, -1.0, 1.0);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	/////opencv
	cout << "Built with OpenCV " << CV_VERSION << endl;
    capture.open(0);

	//Mat background_img = imread("imagens/sky.jpg", IMREAD_COLOR);

	if (capture.isOpened())
	{
		cout << "entra aqui" << endl;
		// Loop da aplicação - "game loop"
		while (!glfwWindowShouldClose(window))
		{
			// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
			glfwPollEvents();

			///////opencv
			capture >> image;
			if (image.empty())
				break;

			//cout << image.rows << " " << image.cols << endl;

			Mat frame1 = image.clone();
			detectAndDraw(frame1, cascade, nestedCascade, scale, tryflip);
		
			GLuint texID = loadTexture(&frame1);

			//////


			// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);
			glViewport(0, 0, width, height);

			// Limpa o buffer de cor
			glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
			glClear(GL_COLOR_BUFFER_BIT);

			glLineWidth(10);
			glPointSize(20);

			//Criando a matriz de modelo usando a GLM
			glm::mat4 model = glm::mat4(1); //matriz identidade
			model = glm::translate(model, glm::vec3((float)WIDTH/2.0f,(float)HEIGHT/2.0, 0));
			//model = glm::rotate(model, (float)glfwGetTime() /*glm::radians(90.0f)*/, glm::vec3(0, 0, 1));
			model = glm::scale(model, glm::vec3((float)WIDTH,(float)HEIGHT, 1.0));
			GLint modelLoc = glGetUniformLocation(shader.ID, "model");
			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

			//Ativando o primeiro buffer de textura (0) e conectando ao identificador gerado
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texID);

			// Chamada de desenho - drawcall
			// Poligono Preenchido - GL_TRIANGLES
			// Observe que como  usamos EBO, agora usamos a glDrawElements!
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			

			//Desenhando os objetos do filtro

			model = glm::mat4(1); //matriz identidade
			model = glm::translate(model, facePos);
			//model = glm::rotate(model, (float)glfwGetTime() /*glm::radians(90.0f)*/, glm::vec3(0, 0, 1));
			model = glm::scale(model, glm::vec3(50.0, 50.0, 1.0));
			shader.setMat4("model", glm::value_ptr(model));
			glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));
			glBindTexture(GL_TEXTURE_2D, texID2);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			
			
			glBindVertexArray(0); //unbind - desconecta

			// Troca os buffers da tela
			glfwSwapBuffers(window);
		}//while
	}
	else
	{
		cout << "No capture" << endl;
		//image = Mat::zeros(480, 640, CV_8UC1);
		//drawText(image);
		//imshow("Sample", image);
		//waitKey(0);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupSprite()
{
	GLuint VAO;
	GLuint VBO, EBO;

	float vertices[] = {
		// posicoes          // cores          // coordenadas de textura
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // superior direito
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // inferior direito
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // inferior esquerdo
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // superior esquerdo
	};
	unsigned int indices[] = {
	0, 1, 3, // primeiro triangulo
	1, 2, 3  // segundo triangulo
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Primeiro atributo - Layout 0 - posição - 3 valores - x, y, z
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// Segundo atributo - Layout 1 - cor - 3 valores - r, g, b
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// Terceiro atributo - Layout 2 - coordenadas de textura - 2 valores - s, t (ou u, v)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); //desvincula

	return VAO;
}

void drawText(Mat& image)
{
    putText(image, "Hello OpenCV",
        Point(20, 50),
        FONT_HERSHEY_COMPLEX, 1, // font face and scale
        Scalar(255, 255, 255), // white
        1, LINE_AA); // line thickness and type
}

GLuint loadTexture(Mat* img)
{
	GLuint texID;
	glGenTextures(1, &texID);

	glBindTexture(GL_TEXTURE_2D, texID);
	flip(*img, *img, 0);

	/*for (int i = 0; i < img->rows; i++)
	{
		for (int j = 0; j < img->cols; j++)
		{
			Vec3b intensity = img->at<Vec3b>(i, j);
			uchar blue = intensity.val[0];
			uchar green = intensity.val[1];
			uchar red = intensity.val[2];

			float media = 0.33 * red + 0.33 * green + 0.33 * blue;
			intensity.val[0] = media; intensity.val[1] = media; intensity.val[2] = media;
			img->at<Vec3b>(i, j) = intensity;
		}
	}*/


	if (img->channels() == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img->cols, img->rows, 0, GL_BGR, GL_UNSIGNED_BYTE, img->data);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img->cols, img->rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, img->data);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

void detectAndDraw(Mat& img, CascadeClassifier& cascade,
	CascadeClassifier& nestedCascade,
	double scale, bool tryflip)
{
	double t = 0;
	vector<Rect> faces, faces2;
	const static Scalar colors[] =
	{
		Scalar(255,0,0),
		Scalar(255,128,0),
		Scalar(255,255,0),
		Scalar(0,255,0),
		Scalar(0,128,255),
		Scalar(0,255,255),
		Scalar(0,0,255),
		Scalar(255,0,255)
	};
	Mat gray, smallImg;

	cvtColor(img, gray, COLOR_BGR2GRAY);
	double fx = 1 / scale;
	resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
	equalizeHist(smallImg, smallImg);

	t = (double)getTickCount();
	cascade.detectMultiScale(smallImg, faces,
		1.1, 2, 0
		//|CASCADE_FIND_BIGGEST_OBJECT
		//|CASCADE_DO_ROUGH_SEARCH
		| CASCADE_SCALE_IMAGE,
		Size(30, 30));
	if (tryflip)
	{
		flip(smallImg, smallImg, 1);
		cascade.detectMultiScale(smallImg, faces2,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (vector<Rect>::const_iterator r = faces2.begin(); r != faces2.end(); ++r)
		{
			faces.push_back(Rect(smallImg.cols - r->x - r->width, r->y, r->width, r->height));
		}
	}
	t = (double)getTickCount() - t;
	printf("detection time = %g ms\n", t * 1000 / getTickFrequency());

	cout << faces.size() << endl;

	for (size_t i = 0; i < faces.size(); i++)
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = colors[i % 8];
		int radius;

		double aspect_ratio = (double)r.width / r.height;
		if (0.75 < aspect_ratio && aspect_ratio < 1.3)
		{
			center.x = cvRound((r.x + r.width * 0.5) * scale);
			center.y = cvRound((r.y + r.height * 0.5) * scale);
			radius = cvRound((r.width + r.height) * 0.25 * scale);
			if (i == 0)
			{
				facePos.x = center.x;
				facePos.y = center.y;
				facePos.z = 0.0;
			}
			circle(img, center, radius, color, 3, 8, 0);
		}
		else
			rectangle(img, Point(cvRound(r.x * scale), cvRound(r.y * scale)),
				Point(cvRound((r.x + r.width - 1) * scale), cvRound((r.y + r.height - 1) * scale)),
				color, 3, 8, 0);
		if (nestedCascade.empty())
			continue;
		smallImgROI = smallImg(r);
		nestedCascade.detectMultiScale(smallImgROI, nestedObjects,
			1.1, 2, 0
			//|CASCADE_FIND_BIGGEST_OBJECT
			//|CASCADE_DO_ROUGH_SEARCH
			//|CASCADE_DO_CANNY_PRUNING
			| CASCADE_SCALE_IMAGE,
			Size(30, 30));
		for (size_t j = 0; j < nestedObjects.size(); j++)
		{
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
			center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
			radius = cvRound((nr.width + nr.height) * 0.25 * scale);
			circle(img, center, radius, color, 3, 8, 0);
		}
	}
	//imshow("result", img);
}