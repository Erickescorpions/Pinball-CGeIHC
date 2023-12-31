/*

Proyecto final de la materia computacion grafica e interaccin humano computadora

Pinball tematica Jujutsu Kaisen
Autores:
- Fuerte Martinez Nestor Enrique
- Vazquez Sanchez Erick Alejandro

*/


#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <time.h>
#include <map>
#include <array>

#include <fstream>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

// biblioteca de audio
#include <irrklang\irrKlang.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

// Para usar mapas de luces
#include "Map_T.h"
#include <map>
#include <string>

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n
float movCoche;
float movOffset;
float rotllanta;
float rotllantaOffset;
bool avanza;
float toffsetflechau = 0.0f;
float toffsetflechav = 0.0f;
float toffsetnumerou = 0.0f;
float toffsetnumerov = 0.0f;
float toffsetnumerocambiau = 0.0;
float angulovaria = 0.0f;

//==================================Variables para la animacion de la bola======================================
float movBallX;
float movBallY;
float movBallZ;

float rotBall;

bool yPositiva;
bool yNegativa;
bool xPositiva;
bool xNegativa;
bool zPositiva;
bool zNegativa;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;
Texture sillonRojo;
Texture piedra;


Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model torsoGojo;
//Modelos para el conejo
Model pataDI;
Model pataTI;
Model pataDD;
Model pataTD;
Model cara;
Model torso;

//Modelo para el purpura
Model esfera;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";



//c�lculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};
	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,


	};
	

	unsigned int flechaIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat flechaVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int scoreIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat scoreVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	unsigned int numeroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat numeroVertices[] = {
		-0.5f, 0.0f, 0.5f,		0.0f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, 0.5f,		0.25f, 0.67f,		0.0f, -1.0f, 0.0f,
		0.5f, 0.0f, -0.5f,		0.25f, 1.0f,		0.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};

	//================================================Primitiva piramide ==========================================================
	unsigned int piramideIndices[] = {
		0, 2, 1,
		0, 1, 3,
		0, 3, 4,
		0, 4, 5,
		5, 0, 6,
		2, 6, 0,
		3, 2, 6,
		3, 5, 6,
	};

	GLfloat piramideVertices[] = {
		0.0f, 0.0f, 4.0,	0.5f, 1.0f,		0.0f, -1.0f, 0.0f, //G - 0
		1.0f, 1.52f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //F - 1
		2.0f, 0.0f, 0.0f,	0.5f, 0.0f,		0.0f, -1.0f, 0.0f, //E - 2
		0.0f, 3.0f, 0.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //H - 3

		-1.0f, 1.5f, 0.0f,   0.5f, 0.0f,	0.0f, -1.0f, 0.0f, //I - 4
		-2.0f, 0.0f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //J - 5
		0.0f, 0.0f, 0.0f,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //K - 6
	};

	//===========================================Primitiva cubo=====================================================================
	unsigned int cuboIndices[] = {
		//Cara frontal
		0,1,2,
		2,0,3,
		//Cara derecha
		4,5,6,
		5,6,7,
		//Cara izquierda
		8,9,10,
		9,10,11,
		//Cara trasera
		12,13,14,
		13,14,15,
		//Cara superior
		16,17,18,
		17,18,19,
		//Cara inferior
		20,21,22,
		21,22,23,
	};

	GLfloat cuboVertices[] = {
		//Cara frontal
		-2.0f, 0.0f, 0.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //A - 0
		2.0f, 0.0f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //B - 1
		2.0f, 4.0f, 0.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //C - 2
		-2.0f, 4.0f, 0.0,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f, //D - 3

		//Cara derecha
		2.0f, 0.0f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //E - 4
		2.0f, 4.0f, 0.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //F - 5
		2.0f, 0.0f, -4.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //G - 6
		2.0f, 4.0f, -4.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //H - 7

		//Cara izquierda
		-2.0f, 0.0f, 0.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //I - 8
		-2.0f, 4.0f, 0.0,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f, //J - 9
		-2.0f, 0.0f, -4.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //K - 10
		-2.0f, 4.0f, -4.0,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f, //L - 11

		//Cara trasera
		-2.0f, 0.0f, -4.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //M - 12
		-2.0f, 4.0f, -4.0,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f, //N - 13
		2.0f, 0.0f, -4.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //� - 14
		2.0f, 4.0f, -4.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //O - 15
		
		//Cara superior
		-2.0f, 4.0f, 0.0,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f, //P- 16
		2.0f, 4.0f, 0.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //Q - 17
		-2.0f, 4.0f, -4.0,	0.0f, 1.0f,		0.0f, -1.0f, 0.0f, //R - 18
		2.0f, 4.0f, -4.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //S - 19

		//Cara inferior
		-2.0f, 0.0f, 0.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //T - 20
		-2.0f, 0.0f, -4.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //U - 21
		2.0f, 0.0f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //V - 22
		2.0f, 0.0f, -4.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //W - 23
	};

	//==============================================Primitiva triangulo=========================================
	unsigned int trianguloIndices[] = {
		0,1,2,	
	};

	GLfloat trianguloVertices[] = {
	
		-1.0f, 0.0f, 0.0,	1.0f, 0.0f,		0.0f, -1.0f, 0.0f, //Inferior izquierdo
		1.0f, 0.0f, 0.0f,	1.0f, 1.0f,		0.0f, -1.0f, 0.0f, //Inferior derecho
		0.0f, 2.0f, 0.0f,	0.0f, 0.5f,		0.0f, -1.0f, 0.0f, //Punta

	};

	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);


	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(flechaVertices, flechaIndices, 32, 6);
	meshList.push_back(obj5);

	Mesh* obj6 = new Mesh();
	obj6->CreateMesh(scoreVertices, scoreIndices, 32, 6);
	meshList.push_back(obj6);

	Mesh* obj7 = new Mesh();
	obj7->CreateMesh(numeroVertices, numeroIndices, 32, 6);
	meshList.push_back(obj7);

	Mesh* obj8 = new Mesh();
	obj8->CreateMesh(piramideVertices, piramideIndices, 56, 24);
	meshList.push_back(obj8);

	Mesh* obj9 = new Mesh();
	obj9->CreateMesh(cuboVertices, cuboIndices, 192, 36);
	meshList.push_back(obj9);

	Mesh* obj10 = new Mesh();
	obj10->CreateMesh(trianguloVertices, trianguloIndices, 24, 3);
	meshList.push_back(obj10);
}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}


float calcularPosicionY(float tiempo) {

	float alturaInicial = 0.0f;
	const float gravedad = 9.8f;
	float alturaMaxima = 5.0f;

	// Calcula la velocidad inicial necesaria para alcanzar la altura m�xima
	float velocidadInicial = sqrt(2.0f * gravedad * alturaMaxima);

	// Posicion de y acorde a la ecuacin del tiro vertical
	return alturaInicial + (velocidadInicial * tiempo) - (0.5f * gravedad * tiempo * tiempo);
}

// ==========================================================================
// ============================ Inicio KeyFrames ============================
// ==========================================================================

//funci�n para teclado de keyframes 
void inputKeyframes(bool* keys);

//variables para keyframes
float reproduciranimacion = 1, habilitaranimacion, guardoFrame, reinicioFrame, ciclo, ciclo2, contador = 0;

bool animacion = false;

//NEW// Keyframes
float posXPelota = 398.0f, posYPelota = 184.0f, posZPelota = -156.0f; 
float movPelotaX = 0.0f, movPelotaY = 0.0f, movPelotaZ = 0.0f, rotaPelota = 0.0f;

#define MAX_FRAMES 300
int i_max_steps = 250;
int i_curr_steps = 48;

typedef struct _frame
{
	float movPelotaX, movPelotaY, movPelotaZ, rotaPelota;
	float movPelotaXInc, movPelotaYInc, movPelotaZInc, rotaPelotaInc;
} FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 48;			//introducir datos
bool play = false;
int playIndex = 0;

// Serializar y guardar en un archivo
std::ofstream archivo("keyframes.txt");


void saveFrame(void) //tecla L
{
	printf("frameindex %d\n", FrameIndex);

	KeyFrame[FrameIndex].movPelotaX = movPelotaX;
	KeyFrame[FrameIndex].movPelotaY = movPelotaY;
	KeyFrame[FrameIndex].movPelotaZ = movPelotaZ;
	KeyFrame[FrameIndex].rotaPelota = rotaPelota;
	

	if (archivo.is_open())
	{
		archivo << "KeyFrame[" << FrameIndex << "].movPelotaX = " << KeyFrame[FrameIndex].movPelotaX << std::endl;
		archivo << "KeyFrame[" << FrameIndex << "].movPelotaY = " << KeyFrame[FrameIndex].movPelotaY << std::endl;
		archivo << "KeyFrame[" << FrameIndex << "].movPelotaZ = " << KeyFrame[FrameIndex].movPelotaZ << std::endl;
		archivo << "KeyFrame[" << FrameIndex << "].rotaPelota = " << KeyFrame[FrameIndex].rotaPelota << std::endl;
		archivo << std::endl; // Separador entre keyframes
		
		std::cout << "Datos guardados correctamente en keyframes.txt" << std::endl;
	}
	else
	{
		std::cerr << "Error al abrir el archivo." << std::endl;
	}
	
	//no volatil, agregar una forma de escribir a un archivo para guardar los frames
	FrameIndex++;
}

void resetElements(void) //Tecla 0
{
	movPelotaX = KeyFrame[0].movPelotaX;
	movPelotaY = KeyFrame[0].movPelotaY;
	movPelotaZ = KeyFrame[0].movPelotaZ;
	rotaPelota = KeyFrame[0].rotaPelota;
}

void interpolation(void)
{
	KeyFrame[playIndex].movPelotaXInc = (KeyFrame[playIndex + 1].movPelotaX - KeyFrame[playIndex].movPelotaX) / i_max_steps;
	KeyFrame[playIndex].movPelotaYInc = (KeyFrame[playIndex + 1].movPelotaY - KeyFrame[playIndex].movPelotaY) / i_max_steps;
	KeyFrame[playIndex].movPelotaZInc = (KeyFrame[playIndex + 1].movPelotaZ - KeyFrame[playIndex].movPelotaZ) / i_max_steps;
	KeyFrame[playIndex].rotaPelotaInc = (KeyFrame[playIndex + 1].rotaPelota - KeyFrame[playIndex].rotaPelota) / i_max_steps;
}

void animate(void)
{
	//Movimiento del objeto con barra espaciadora
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //fin de animaci�n entre frames?
		{
			playIndex++;
			//printf("playindex : %d\n", playIndex);
			if (playIndex > FrameIndex - 2)	//Fin de toda la animaci�n con �ltimo frame?
			{
				//printf("Frame index= %d\n", FrameIndex);
				//printf("termino la animacion\n");
				playIndex = 0;
				play = false;
			}
			else //Interpolaci�n del pr�ximo cuadro
			{

				i_curr_steps = 0; //Resetea contador
				//Interpolar
				interpolation();
			}
		}
		else
		{
			//Dibujar Animaci�n
			movPelotaX += KeyFrame[playIndex].movPelotaXInc;
			movPelotaY += KeyFrame[playIndex].movPelotaYInc;
			movPelotaZ += KeyFrame[playIndex].movPelotaZInc;
			movPelotaZ += KeyFrame[playIndex].rotaPelota;
			i_curr_steps++;
		}

	}
}

// ==========================================================================
// ============================ Fin KeyFrames ===============================
// ==========================================================================

void iniciaAnimacionKeyframe();
void reseteaAnimacionKeyframe();

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();


	Camera camaraIsometrica;
	Camera camaraPersonaje;
	Camera* camera;

	camaraIsometrica = Camera(glm::vec3(78.5, 550.0f, 350.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.0f, 0.5f);
	camaraPersonaje = Camera(glm::vec3(90.0f, 240.0f, 427.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 3.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	piedra = Texture("Textures/piedra.tga");
	piedra.LoadTextureA();

	// ======================== Modelos ========================

	Model pelota_M = Model();
	pelota_M.LoadModel("Models/Pinball/pelota.obj");

	Model pinball_M = Model();
	pinball_M.LoadModel("Models/Pinball/pinball.obj");

	Model palanca_M = Model();
	palanca_M.LoadModel("Models/Pinball/palanca.obj");

	Model resorte_M = Model();
	resorte_M.LoadModel("Models/Pinball/resorte.obj");

	Model santuario_M = Model();
	santuario_M.LoadModel("Models/SantuarioMalevolo/SantuarioMalevolo.obj");
	
	//Modelos para el avatar=========================================================================
	Model torsoGojo_M = Model();
	torsoGojo_M.LoadModel("Models/Gojo/torsoSuperior.obj");

	Model cabezaGojo_M = Model();
	cabezaGojo_M.LoadModel("Models/Gojo/cara.obj");

	Model piernasGojo_M = Model();
	piernasGojo_M.LoadModel("Models/Gojo/piernasGojo.obj");

	Model dedoSukuna_M = Model();
	dedoSukuna_M.LoadModel("Models/DedoSukuna/DedoSukuna.obj");

	Model ojoGojo_M = Model();
	ojoGojo_M.LoadModel("Models/Pinball/ojo.obj");

	pataDD = Model();
	pataDD.LoadModel("Models/Conejo/pataDelanteraDerecha.obj");

	pataDI = Model();
	pataDI.LoadModel("Models/Conejo/pataDelanteraIzquierda.obj");

	pataTD = Model();
	pataTD.LoadModel("Models/Conejo/pataTraseraDerecha.obj");

	pataTI = Model();
	pataTI.LoadModel("Models/Conejo/pataTraseraIzquierda.obj");

	cara = Model();
	cara.LoadModel("Models/Conejo/cara.obj");

	torso = Model();
	torso.LoadModel("Models/Conejo/torso.obj");

	Model cristal_M = Model();
	cristal_M.LoadModel("Models/Pinball/cristal.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	// ===========================================================
	// ========================== Luces ==========================
	// ===========================================================
	
	// luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, -1.0f, 0.0f);
	
	// contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	
	// ================== Mapas de luces ==================
	std::map<std::string, PointLight> pointLightsMap;
	std::map<std::string, SpotLight> spotLightsMap;

	// mapas auxiliares para guardar todas las luces
	std::map<std::string, PointLight> pointLightsMapAux;
	std::map<std::string, SpotLight> spotLightsMapAux;

	// ================== Luz Todo Tablero ==================
	// ================== Luz Santuario ==================
	pointLightsMap["santuario"] = PointLight(
		1.0f, 0.0f, 0.0f,
		1.5f, 1.5f,
		83.0f, 180.0f, 65.0f,
		0.0001f, 0.003f, 0.004f
	);
	pointLightCount++;

	// ================== Luz Azul flippers ==================
	pointLightsMap["flippers"] = PointLight(
		0.0f, 1.0f, 1.0f,
		2.5f, 2.5f,
		83.0f, 175.0f, 227.0f,
		0.0001f, 0.003f, 0.004f
	);
	pointLightCount++;

	// ================== Luz Morada Avatar ==================
	spotLightsMap["avatar"] = SpotLight(
		1.0f, 0.0f, 0.0f,
		1.5f, 1.5f,
		90.0f, 240.0f + 60.0f, 427.0f, // posicion inicial del avatar
		0.0f, -1.0f, 0.0f,
		0.001f, 0.001f, 0.0004f, 
		15.0f
	);
	spotLightCount++;

	// ================== copiando el contenido de los mapas a los aux ==================
	pointLightsMapAux = pointLightsMap;
	spotLightsMapAux = spotLightsMap;
	
	// ================== libreria de sonido ===============
	// motor de sonido
	irrklang::ISoundEngine* SoundEngine = irrklang::createIrrKlangDevice();

	if (!SoundEngine)
		return 0; 

	SoundEngine->setSoundVolume(0.2f);

	// sonido 2D
	irrklang::ISound* sonido2D = SoundEngine->play2D("audio/JujutsuKaisen[OST].mp3", true);

	if (sonido2D) {
		float volumen2d = 0.01f;
		sonido2D->setVolume(volumen2d);
	}

	irrklang::ISound* sonidoPinball = SoundEngine->play3D("audio/PINBALL.mp3",
		irrklang::vec3df(83.0f, 180.0f, 65.0f), true, false, true);

	if (sonidoPinball) {
		sonidoPinball->setMinDistance(200.0f);
		float volumen3d = 0.9f; 
		sonidoPinball->setVolume(volumen3d);
	}


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// variables para la animacion del resorte/palanca
	bool animacionResorteContrae = false;
	const float contraeResorteOffset = 0.2f;
	const float muevePalancaOffset = 0.2f;
	float contraeResorte = 100.0f;
	float muevePalanca = 0.0f;


	const float sueltaPalancaOffset = 2.0f;
	const float expandeResorteOffset = 2.0f;
	bool animacionResorteExpande = false;


	// variables para la camara del avatar
	bool cambioPersonaje = false;
	glm::vec3 posicionGojo = glm::vec3(90.0f, 240.0f, 427.0f);
	//float rotacionGojo = 180.0f;
	bool imprimirPosicion = false;


	// variables para el movimiento de los flippers
	float flipper1 = 0.0f;
	float flipper2 = 0.0f;
	float flipper3 = 0.0f;

	float rotaFlipperOffset = 2.0f;

	// keyframes pelota

	KeyFrame[0].movPelotaX = 0.0f;
	KeyFrame[0].movPelotaY = 0.0f;
	KeyFrame[0].movPelotaZ = 0.0f;
	KeyFrame[0].rotaPelota = 0.0f;

	KeyFrame[1].movPelotaX = -68;
	KeyFrame[1].movPelotaY = 6;
	KeyFrame[1].movPelotaZ = 0;
	KeyFrame[1].rotaPelota = 0;

	KeyFrame[2].movPelotaX = -99.9999;
	KeyFrame[2].movPelotaY = 9;
	KeyFrame[2].movPelotaZ = 0;
	KeyFrame[2].rotaPelota = 0;

	KeyFrame[3].movPelotaX = -171;
	KeyFrame[3].movPelotaY = 14;
	KeyFrame[3].movPelotaZ = 0;
	KeyFrame[3].rotaPelota = 0;

	KeyFrame[4].movPelotaX = -265;
	KeyFrame[4].movPelotaY = 22;
	KeyFrame[4].movPelotaZ = 0;
	KeyFrame[4].rotaPelota = 0;

	KeyFrame[5].movPelotaX = -312.999;
	KeyFrame[5].movPelotaY = 26.9999;
	KeyFrame[5].movPelotaZ = 0;
	KeyFrame[5].rotaPelota = 0;

	KeyFrame[6].movPelotaX = -316.999;
	KeyFrame[6].movPelotaY = 26.9999;
	KeyFrame[6].movPelotaZ = 8;
	KeyFrame[6].rotaPelota = 0;

	KeyFrame[7].movPelotaX = -316.999;
	KeyFrame[7].movPelotaY = 26.9999;
	KeyFrame[7].movPelotaZ = 17;
	KeyFrame[7].rotaPelota = 0;

	KeyFrame[8].movPelotaX = -316.998;
	KeyFrame[8].movPelotaY = 26.9999;
	KeyFrame[8].movPelotaZ = 44;
	KeyFrame[8].rotaPelota = 0;

	KeyFrame[9].movPelotaX = -316.998;
	KeyFrame[9].movPelotaY = 26.9999;
	KeyFrame[9].movPelotaZ = 59;
	KeyFrame[9].rotaPelota = 0;

	KeyFrame[10].movPelotaX = -314.998;
	KeyFrame[10].movPelotaY = 26.9999;
	KeyFrame[10].movPelotaZ = 62;
	KeyFrame[10].rotaPelota = 0;

	KeyFrame[11].movPelotaX = -277.998;
	KeyFrame[11].movPelotaY = 22.9999;
	KeyFrame[11].movPelotaZ = 62.0001;
	KeyFrame[11].rotaPelota = 0;

	KeyFrame[12].movPelotaX = -290.999;
	KeyFrame[12].movPelotaY = 23.9998;
	KeyFrame[12].movPelotaZ = 73.0001;
	KeyFrame[12].rotaPelota = 0;

	KeyFrame[13].movPelotaX = -290.999;
	KeyFrame[13].movPelotaY = 23.9998;
	KeyFrame[13].movPelotaZ = 57.0001;
	KeyFrame[13].rotaPelota = 0;

	KeyFrame[14].movPelotaX = -313.999;
	KeyFrame[14].movPelotaY = 26.9998;
	KeyFrame[14].movPelotaZ = 84.0001;
	KeyFrame[14].rotaPelota = 0;

	KeyFrame[15].movPelotaX = -317.998;
	KeyFrame[15].movPelotaY = 26.9997;
	KeyFrame[15].movPelotaZ = 93.0003;
	KeyFrame[15].rotaPelota = 0;

	KeyFrame[16].movPelotaX = -293.998;
	KeyFrame[16].movPelotaY = 24.9997;
	KeyFrame[16].movPelotaZ = 114;
	KeyFrame[16].rotaPelota = 0;

	KeyFrame[17].movPelotaX = -285.998;
	KeyFrame[17].movPelotaY = 23.9995;
	KeyFrame[17].movPelotaZ = 99.9999;
	KeyFrame[17].rotaPelota = 0;

	KeyFrame[18].movPelotaX = -280.997;
	KeyFrame[18].movPelotaY = 23.9993;
	KeyFrame[18].movPelotaZ = 94.9992;
	KeyFrame[18].rotaPelota = 0;

	KeyFrame[19].movPelotaX = -271.997;
	KeyFrame[19].movPelotaY = 22.9993;
	KeyFrame[19].movPelotaZ = 90.9992;
	KeyFrame[19].rotaPelota = 0;

	KeyFrame[20].movPelotaX = -247.997;
	KeyFrame[20].movPelotaY = 22.9993;
	KeyFrame[20].movPelotaZ = 78.9992;
	KeyFrame[20].rotaPelota = 0;

	KeyFrame[21].movPelotaX = -255.997;
	KeyFrame[21].movPelotaY = 21.9993;
	KeyFrame[21].movPelotaZ = 62.9992;
	KeyFrame[21].rotaPelota = 0;

	KeyFrame[22].movPelotaX = -247.995;
	KeyFrame[22].movPelotaY = 21.9991;
	KeyFrame[22].movPelotaZ = 53.9986;
	KeyFrame[22].rotaPelota = 0;

	KeyFrame[23].movPelotaX = -229.995;
	KeyFrame[23].movPelotaY = 19.9991;
	KeyFrame[23].movPelotaZ = 49.9986;
	KeyFrame[23].rotaPelota = 0;

	KeyFrame[24].movPelotaX = -229.993;
	KeyFrame[24].movPelotaY = 19.9989;
	KeyFrame[24].movPelotaZ = 56.998;
	KeyFrame[24].rotaPelota = 0;

	KeyFrame[25].movPelotaX = -229.993;
	KeyFrame[25].movPelotaY = 19.9989;
	KeyFrame[25].movPelotaZ = 49.998;
	KeyFrame[25].rotaPelota = 0;

	KeyFrame[26].movPelotaX = -229.993;
	KeyFrame[26].movPelotaY = 19.9989;
	KeyFrame[26].movPelotaZ = 55.998;
	KeyFrame[26].rotaPelota = 0;

	KeyFrame[27].movPelotaX = -229.993;
	KeyFrame[27].movPelotaY = 19.9989;
	KeyFrame[27].movPelotaZ = 49.998;
	KeyFrame[27].rotaPelota = 0;

	KeyFrame[28].movPelotaX = -220.993;
	KeyFrame[28].movPelotaY = 19.9989;
	KeyFrame[28].movPelotaZ = 53.998;
	KeyFrame[28].rotaPelota = 0;

	KeyFrame[29].movPelotaX = -218.991;
	KeyFrame[29].movPelotaY = 19.9987;
	KeyFrame[29].movPelotaZ = 48.9974;
	KeyFrame[29].rotaPelota = 0;

	KeyFrame[30].movPelotaX = -164.989;
	KeyFrame[30].movPelotaY = 13.9985;
	KeyFrame[30].movPelotaZ = 61.9969;
	KeyFrame[30].rotaPelota = 0;

	KeyFrame[31].movPelotaX = -134.987;
	KeyFrame[31].movPelotaY = 11.9983;
	KeyFrame[31].movPelotaZ = 71.9964;
	KeyFrame[31].rotaPelota = 0;

	KeyFrame[32].movPelotaX = -113.984;
	KeyFrame[32].movPelotaY = 8.9981;
	KeyFrame[32].movPelotaZ = 36.9961;
	KeyFrame[32].rotaPelota = 0;

	KeyFrame[33].movPelotaX = -106.982;
	KeyFrame[33].movPelotaY = 8.99789;
	KeyFrame[33].movPelotaZ = 37.9957;
	KeyFrame[33].rotaPelota = 0;

	KeyFrame[34].movPelotaX = -103.982;
	KeyFrame[34].movPelotaY = 8.99789;
	KeyFrame[34].movPelotaZ = 46.9957;
	KeyFrame[34].rotaPelota = 0;

	KeyFrame[35].movPelotaX = -98.9798;
	KeyFrame[35].movPelotaY = 8.99771;
	KeyFrame[35].movPelotaZ = 58.9953;
	KeyFrame[35].rotaPelota = 0;

	KeyFrame[36].movPelotaX = -82.9798;
	KeyFrame[36].movPelotaY = 7.99771;
	KeyFrame[36].movPelotaZ = 74.9953;
	KeyFrame[36].rotaPelota = 0;

	KeyFrame[37].movPelotaX = -45.9851;
	KeyFrame[37].movPelotaY = 3.99692;
	KeyFrame[37].movPelotaZ = 81.9952;
	KeyFrame[37].rotaPelota = 0;

	KeyFrame[38].movPelotaX = -5.99027;
	KeyFrame[38].movPelotaY = -1.00385;
	KeyFrame[38].movPelotaZ = 81.9953;
	KeyFrame[38].rotaPelota = 0;

	KeyFrame[39].movPelotaX = -4.99549;
	KeyFrame[39].movPelotaY = -1.00461;
	KeyFrame[39].movPelotaZ = 81.9953;
	KeyFrame[39].rotaPelota = 0;

	KeyFrame[40].movPelotaX = -2.99549;
	KeyFrame[40].movPelotaY = -1.00461;
	KeyFrame[40].movPelotaZ = 69.9953;
	KeyFrame[40].rotaPelota = 0;

	KeyFrame[41].movPelotaX = -1.99549;
	KeyFrame[41].movPelotaY = -1.00461;
	KeyFrame[41].movPelotaZ = 60.9953;
	KeyFrame[41].rotaPelota = 0;

	KeyFrame[42].movPelotaX = -0.995494;
	KeyFrame[42].movPelotaY = -1.00461;
	KeyFrame[42].movPelotaZ = 51.9953;
	KeyFrame[42].rotaPelota = 0;

	KeyFrame[43].movPelotaX = 0.00450611;
	KeyFrame[43].movPelotaY = -1.00461;
	KeyFrame[43].movPelotaZ = 43.9953;
	KeyFrame[43].rotaPelota = 0;

	KeyFrame[44].movPelotaX = 0.00450611;
	KeyFrame[44].movPelotaY = -1.00461;
	KeyFrame[44].movPelotaZ = 31.9953;
	KeyFrame[44].rotaPelota = 0;

	KeyFrame[45].movPelotaX = 0.999277;
	KeyFrame[45].movPelotaY = -1.00538;
	KeyFrame[45].movPelotaZ = 19.9957;
	KeyFrame[45].rotaPelota = 0;

	KeyFrame[46].movPelotaX = 0.999277;
	KeyFrame[46].movPelotaY = -1.00538;
	KeyFrame[46].movPelotaZ = 4.99569;
	KeyFrame[46].rotaPelota = 0;

	KeyFrame[47].movPelotaX = 0.994045;
	KeyFrame[47].movPelotaY = -1.00615;
	KeyFrame[47].movPelotaZ = -0.0039382;
	KeyFrame[47].rotaPelota = 0;
	
	// variables animacion conejo
	// ========= variables para animaci�n de la bola =========
	float movOffset = 0.05f;
	float movOffsetX = 0.35f;
	float movOffsetY = 0.027f;
	float movOffsetZ = 0.35f;
	float movBola = 0.0f;

	movBallX = 398.0f;
	movBallY = 184.0f;
	movBallZ = -156.0f;

	rotBall = 0.0f;
	float rotBallOffset = 5.0f;

	xPositiva = true;
	xNegativa = false;
	yPositiva = false;
	yNegativa = false;
	zPositiva = false;
	zNegativa = false;
	bool rebotePiramide = false;
	bool rebotePared = false;
	bool reboteSantuario = false;
	bool reboteConejo1 = false;
	bool rebotePared2 = false;
	bool rebotePared3 = false;
	bool rebotePared4 = false;

	float movConejoOffset = 0.05f;
	float movConejo = 0.0f;
	float tiempoTranscurrido = 0.0f;


	printf("Acciones: \n");
	printf("Iniciar animacion de la pelota:     Presionar click derecho y soltar\n");
	printf("Cambiar de camara:                  Presionar tecla 'C'\n");
	printf("Mover paleta principal derecha:     Presionar tecla '1'\n");
	printf("Mover paleta principal izquierda:   Presionar tecla '2'\n");
	printf("Mover paleta secundaria:            Presionar tecla '3'\n");
	printf("Apagar la luz de los flippers:      Presionar tecla 'Z'\n");
	printf("Apagar la luz del santuario:        Presionar tecla 'X'\n");
	
	// =================================== Loop mientras no se cierra la ventana  ===================================
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();

		if (mainWindow.getCamara()) {

			camera = &camaraPersonaje;
		}
		else {
			camera = &camaraIsometrica;
			camera->setPosition(78.5, 550.0f, 350.0f);
		}

		camera->keyControl(mainWindow.getsKeys(), deltaTime);
		camera->mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		inputKeyframes(mainWindow.getsKeys());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera->calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera->getCameraPosition().x, camera->getCameraPosition().y, camera->getCameraPosition().z);

		// =================== Paso de map a array =================== 
		// pasamos el contenido de los mapas a las listas
		Map_T::toArray(pointLightsMap, pointLights, MAX_POINT_LIGHTS);
		Map_T::toArray(spotLightsMap, spotLights, MAX_SPOT_LIGHTS);

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		// ================== control de las luces ==================
		// solo si la luz existe
		// si el interruptor esta en encendido y existe la luz, se elimina
		if (mainWindow.getInterruptorFlippers() && pointLightsMap.count("flippers") > 0) {
			pointLightsMap.erase("flippers");
			--pointLightCount;
		}
		// si el interruptor esta apagado y no existe la luz, se agrega de nuevo la llave
		// se agregan estas validaciones para que no entre al else en cada iterecion y nos sume + 1 en
		// la variable pointLightCount
		else if (!mainWindow.getInterruptorFlippers() && pointLightsMap.count("flippers") == 0) {
			pointLightsMap["flippers"] = pointLightsMapAux["flippers"];
			++pointLightCount;
		}

		if (mainWindow.getInterruptorSantuario() && pointLightsMap.count("santuario") > 0) {
			pointLightsMap.erase("santuario");
			--pointLightCount;
		} else if (!mainWindow.getInterruptorSantuario() && pointLightsMap.count("santuario") == 0) {
			pointLightsMap["santuario"] = pointLightsMapAux["santuario"];
			++pointLightCount;
		}


		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::mat4 modelauxPurpura(1.0);
		glm::mat4 modelauxConejo(1.0);
		glm::mat4 modelauxConejo2(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);
		
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();

		//========================== Animacion pelota 1 =======================================================
		if ( xPositiva == true )
		{
			if (movBallX > 85.f) {
				movBallX -= movOffsetX * deltaTime;
				if ( movBallX < 340.0f)
				{
					movBallY += movOffsetY * deltaTime;
				}
			}
			else
			{
				xPositiva = false;
				zNegativa = true;

			}
			rotBall += rotBallOffset * deltaTime;
		}

		if (zNegativa == true)
		{
			if (movBallZ < -20.0f)
			{
				movBallZ += movOffsetZ * deltaTime;
			}
			else
			{
				zNegativa = false;
				yNegativa = true;
			}
		}

		//Rbote para abajo
		if (yNegativa == true)
		{
			if (movBallX < 110.0f)
			{
				movBallX += movOffsetX * deltaTime;
			}
			else
			{
				yNegativa = false;
				rebotePiramide = true;

			}
		}

		//Rebote con piramide
		if (rebotePiramide == true)
		{
			if (movBallZ < -16.0f)
			{
				movBallZ += movOffsetZ * deltaTime;
			}
			else
			{
				rebotePiramide = false;
				rebotePared = true;
			}
		}

		//Rebota en la pared
		if (rebotePared == true)
		{
			if (movBallZ > -57.0f)
			{
				movBallX += movOffsetX * deltaTime;
				movBallZ -= movOffsetZ * deltaTime;
				movBallY -= movOffsetY * deltaTime;

			}
			else
			{
				rebotePared = false;
				reboteSantuario = true;
			}
		}

		//Rebota con el santuario
		if (reboteSantuario == true)
		{
			if (movBallZ < -30.0f)
			{
				movBallX += movOffsetX * deltaTime;
				movBallZ += movOffsetZ * deltaTime;
				movBallY -= movOffsetY * deltaTime;
			}
			else
			{
				reboteSantuario = false;
				reboteConejo1 = true;
			}
		}

		//Rebota con el primer conejo
		if (reboteConejo1 == true)
		{
			if (movBallZ > -120.0f)
			{
				movBallX += movOffsetX * deltaTime;
				movBallZ -= movOffsetZ * deltaTime;
				movBallY -= movOffsetY * deltaTime;
			}
			else
			{
				reboteConejo1 = false;
				rebotePared2 = true;
			}
		}

		//Segundo rebote a una pared
		if (rebotePared2 == true)
		{
			if (movBallZ < -42.0f)
			{
				movBallX += movOffsetX * deltaTime;
				movBallZ += movOffsetZ * deltaTime;
				movBallY -= movOffsetY * deltaTime;
			}
			else
			{
				rebotePared2 = false;
				rebotePared3 = true;
			}
		}

		//Tercera colicion con pared
		if (rebotePared3 == true)
		{
			if (movBallZ > -75.0f)
			{
				movBallZ -= movOffsetZ * deltaTime;				
			}
			
			else
			{
				rebotePared3 = false;
				rebotePared4 = true;
			}
		}

		//Cuarto choque con pared
		if (rebotePared4 == true)
		{
			if (movBallZ < -73.0f)
			{
				if (movBallX < 420.0f)
				{
					movBallX += movOffsetX * deltaTime;
					movBallY -= movOffsetY * deltaTime;
				}
			}
		}
		
		
		// ========================= Para posicionar el avatar en la camara =========================
		if (mainWindow.getCamara()) {
			posicionGojo =  glm::vec3(camera->getCameraPosition().x, camera->getCameraPosition().y - 15.0f, camera->getCameraPosition().z - 40.0f);
		}

		spotLightsMap["avatar"].SetPos(glm::vec3(posicionGojo.x, posicionGojo.y + 60.0f, posicionGojo.z));

		/*if (mainWindow.imprimirPosicion()) {
			std::cout << "x: " << posicionGojo.x << "y: " << posicionGojo.y << "z: " << posicionGojo.z << std::endl;
		}*/

		// ========================= Piernas gojo =========================

		model = glm::mat4(1.0);
		model = glm::translate(model, posicionGojo);
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piernasGojo_M.RenderModel();

		// ========================= Torso gojo =========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(3.0f, 10.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		torsoGojo_M.RenderModel();

		// ========================= Cabeza gojo =========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(5.0f, 51.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cabezaGojo_M.RenderModel();

		// ======================== pinball ========================
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, -1.0f, -90.0f));
		// Descomentar lineas de abajo para ver el resultado final, para establecer posiciones
		// dejarlas comentadas
		model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pinball_M.RenderModel();

		// ======================== palanca ========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(407.0f + muevePalanca, 178.0f, -156.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		palanca_M.RenderModel();

		// colocamos de nuevo el color en blanco para las siguientes texturas
		color = glm::vec3(1.0f, 1.0f, 1.0f);

		// ======================== resorte ========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(460.0f, 178.0f, -156.0f));
		model = glm::scale(model, glm::vec3((1.0f * contraeResorte) / 100, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		resorte_M.RenderModel();

		// ======================== animacion resorte/palanca ========================
		if (mainWindow.getClickDerecho()) {
			animacionResorteContrae = true;
			animacionResorteExpande = false;
			reseteaAnimacionKeyframe();
		}
		else {
			animacionResorteContrae = false;
			animacionResorteExpande = true;
		}

		if (animacionResorteContrae) {
			
			if (muevePalanca <= 25.0f) {

				muevePalanca += muevePalancaOffset * deltaTime;
				
				if (contraeResorte >= 10.0f) {
					contraeResorte -= contraeResorteOffset * deltaTime;
				}
			}
		}

		if (animacionResorteExpande) {
			if (muevePalanca > 0.0f) {
				muevePalanca -= sueltaPalancaOffset * deltaTime;

				if (contraeResorte <= 100.0f) {
					contraeResorte += expandeResorteOffset * deltaTime;
				}
			}
			else {
				iniciaAnimacionKeyframe();
			}
		}

		// ======================== bola animada por keyframes ========================

		model = modelaux;
		model = glm::translate(model, glm::vec3(posXPelota + movPelotaX, posYPelota + movPelotaY, posZPelota + movPelotaZ));
		model = glm::rotate(model, glm::radians(-15.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pelota_M.RenderModel();

		animate();

		//============================= Bola animada ===============================================================
		model = modelaux;
		model = glm::translate(model, glm::vec3(movBallX, movBallY, movBallZ));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		model = glm::rotate(model, rotBall * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		pelota_M.RenderModel();

		// ======================== Santuario Malevolo ========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(175.0f, 215.0f, -75.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		santuario_M.RenderModel();

		// ======================== Ojos Gojo ========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(131.0f, 204.0f, -95.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojoGojo_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(131.0f, 204.0f, -50.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojoGojo_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(111.0f, 204.0f, -72.5f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojoGojo_M.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(270.0f, 195.0f, -10.0f)); 
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojoGojo_M.RenderModel();

		/*model = modelaux;
		model = glm::translate(model, glm::vec3(290.0f, 193.0f, -50.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		ojoGojo_M.RenderModel();*/


		// ======================== Flippers ========================

		// flipper 1
		model = modelaux;
		model = glm::translate(model, glm::vec3(353.0f, 185.0f, -95.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(20.0f - flipper1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		dedoSukuna_M.RenderModel();

		// flipper 2
		model = modelaux;
		model = glm::translate(model, glm::vec3(353.0f, 185.0f, -50.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		model = glm::rotate(model, glm::radians(160.0f + flipper2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		dedoSukuna_M.RenderModel();

		// flipper 3
		model = modelaux;
		model = glm::translate(model, glm::vec3(300.0f, 189.0f, -115.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(20.0f - flipper3), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		dedoSukuna_M.RenderModel();

		// movimiento de cada flipper 
		if (mainWindow.getFlipper1()) {
			if (flipper1 <= 60) {
				flipper1 += rotaFlipperOffset * deltaTime;
			}
		}
		else {
			if (flipper1 > 0) {
				flipper1 -= rotaFlipperOffset * deltaTime;
			}
		}

		if (mainWindow.getFlipper2()) {
			if (flipper2 <= 60) {
				flipper2 += rotaFlipperOffset * deltaTime;
			}
		}
		else {
			if (flipper2 > 0) {
				flipper2 -= rotaFlipperOffset * deltaTime;
			}
		}

		if (mainWindow.getFlipper3()) {
			if (flipper3 <= 60) {
				flipper3 += rotaFlipperOffset * deltaTime;
			}
		}
		else {
			if (flipper3 > 0) {
				flipper3 -= rotaFlipperOffset * deltaTime;
			}
		}

		// ================================================================================
		// ====================== Modelo instanciado jerarquicamente ======================
		// ================================================================================
		
		//movBola -= movOffset * deltaTime;
		tiempoTranscurrido += movOffset * deltaTime;
		//
		// Actualiza la posici�n de la pelota en funci�n del tiempo transcurrido
		float nuevaAltura = calcularPosicionY(tiempoTranscurrido);

		// Reinicia el salto cuando la pelota toca el suelo
		if (nuevaAltura <= 0.0f) {
			tiempoTranscurrido = 0.0f;
		}
		
		// ====================== Conejo 1 ======================
		// ====================== Cuerpo del conejito ======================
		model = modelaux;
		model = glm::translate(model, glm::vec3(190.0f, 195.0f + nuevaAltura, -25.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxConejo = model;
		modelauxConejo2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		torso.RenderModel();

		// ====================== Pata delantera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(0.3f, 1.2f, 0.2f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDD.RenderModel();

		// ====================== Pata delantera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.69f, 0.2f, -0.04f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDI.RenderModel();

		// ====================== Pata trasera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.3f, -0.37f, -1.1f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTI.RenderModel();

		// ====================== Pata trasera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTD.RenderModel();

		// ====================== Cabecita del conejito ======================
		model = modelauxConejo2;
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cara.RenderModel();


		// ====================== Conejo 2 ======================
		// ====================== Cuerpo del conejito ======================
		model = modelaux;
		model = glm::translate(model, glm::vec3(175.0f, 195.0f + nuevaAltura, -115.0f)); 
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxConejo = model;
		modelauxConejo2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		torso.RenderModel();

		// ====================== Pata delantera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(0.3f, 1.2f, 0.2f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDD.RenderModel();

		// ====================== Pata delantera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.69f, 0.2f, -0.04f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDI.RenderModel();

		// ====================== Pata trasera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.3f, -0.37f, -1.1f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTI.RenderModel();

		// ====================== Pata trasera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTD.RenderModel();

		// ====================== Cabecita del conejito ======================
		model = modelauxConejo2;
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cara.RenderModel();

		// ====================== Conejo 3 ======================
		// ====================== Cuerpo del conejito ======================
		model = modelaux;
		model = glm::translate(model, glm::vec3(270.0f, 189.0f + nuevaAltura, -75.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxConejo = model;
		modelauxConejo2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		torso.RenderModel();

		// ====================== Pata delantera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(0.3f, 1.2f, 0.2f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDD.RenderModel();

		// ====================== Pata delantera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.69f, 0.2f, -0.04f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDI.RenderModel();

		// ====================== Pata trasera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.3f, -0.37f, -1.1f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTI.RenderModel();

		// ====================== Pata trasera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTD.RenderModel();

		// ====================== Cabecita del conejito ======================
		model = modelauxConejo2;
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cara.RenderModel();

		// ====================== Conejo 4 ======================
		// ====================== Cuerpo del conejito ======================
		model = modelaux;
		model = glm::translate(model, glm::vec3(320.0f, 185.0f + nuevaAltura, -25.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelauxConejo = model;
		modelauxConejo2 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		torso.RenderModel();

		// ====================== Pata delantera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(0.3f, 1.2f, 0.2f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDD.RenderModel();

		// ====================== Pata delantera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.69f, 0.2f, -0.04f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataDI.RenderModel();

		// ====================== Pata trasera izquierda del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(-0.3f, -0.37f, -1.1f));
		modelauxConejo = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTI.RenderModel();

		// ====================== Pata trasera derecha del conejito ======================
		model = modelauxConejo;
		model = glm::translate(model, glm::vec3(1.0f, 0.2f, -0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pataTD.RenderModel();

		// ====================== Cabecita del conejito ======================
		model = modelauxConejo2;
		model = glm::translate(model, glm::vec3(0.0f, 1.2f, 0.8f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		cara.RenderModel();

		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// ====================== Cristal transparente ======================
		/*model = modelaux;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		cristal_M.RenderModel();*/



		// ======================= Instanciando primitiva =======================	
		//Primitiva piramide
		model = modelaux;
		model = glm::translate(model, glm::vec3(113.0f, 202.0f, -33.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -85 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));//Se la envio al shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piedra.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		model = modelaux;
		model = glm::translate(model, glm::vec3(113.0f, 202.0f, -110.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -85 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));//Se la envio al shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		piedra.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[7]->RenderMesh();

		//Primitiva triangulo
		toffsetflechau += 0.005;
		toffsetflechav += 0.0;
		if (toffsetflechau > 1.0)
			toffsetflechau = 0.0;
		toffset = glm::vec2(toffsetflechau, toffsetflechav);
		model = modelaux;
		model = glm::translate(model, glm::vec3(353.0f, 182.0f, -72.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -85 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));//Se la envio al shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		model = modelaux;
		model = glm::translate(model, glm::vec3(250.0f, 192.0f, -110.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, -85 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));//Se la envio al shader
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		FlechaTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[9]->RenderMesh();

		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	} // ======================================== Fin del loop  ========================================


	archivo.close();
	return 0;
} 


void inputKeyframes(bool* keys)
{
	// guarda un frame
	if (keys[GLFW_KEY_O])
	{
		if (guardoFrame < 1)
		{
			saveFrame();
			printf("movPelotaX es: %f\n", movPelotaX);
			printf("movPelotaY es: %f\n", movPelotaY);
			printf("movPelotaZ es: %f\n", movPelotaZ);
			printf("presiona P para habilitar guardar otro frame'\n");
			guardoFrame++;
			reinicioFrame = 0;
		}
	}

	// habilita para guardar otro frame
	if (keys[GLFW_KEY_P])
	{
		if (reinicioFrame < 1)
		{
			guardoFrame = 0;
			printf("Ya puedes guardar otro frame presionando la tecla O'\n");
		}
	}

	// tecla H desplaza en x negativo
	if (keys[GLFW_KEY_H])
	{
		if (ciclo < 1)
		{
			movPelotaX -= 1.0f;
			printf("\n movPelotaX es: %f\n", movPelotaX);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	// tecla J desplaza en x positivo
	if (keys[GLFW_KEY_J])
	{
		if (ciclo < 1)
		{
			movPelotaX += 1.0f;
			printf("\n movPelotaX es: %f\n", movPelotaX);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	// tecla N para y positivas
	if (keys[GLFW_KEY_N])
	{
		if (ciclo < 1)
		{
			movPelotaY += 1.0f;
			printf("\n movPelotaY es: %f\n", movPelotaY);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	// tecla M desplaza en y negativo
	if (keys[GLFW_KEY_M])
	{
		if (ciclo < 1)
		{
			movPelotaY -= 1.0f;
			printf("\n movPelotaY es: %f\n", movPelotaY);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	// tecla K desplaza en z positivo
	if (keys[GLFW_KEY_K])
	{
		if (ciclo < 1)
		{
			movPelotaZ += 1.0f;
			printf("\n movPelotaZ es: %f\n", movPelotaZ);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	// tecla L desplaza en Z negativo
	if (keys[GLFW_KEY_L])
	{
		if (ciclo < 1)
		{
			movPelotaZ -= 1.0f;
			printf("\n movPelotaZ es: %f\n", movPelotaZ);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	// tecla L desplaza en Z negativo
	if (keys[GLFW_KEY_B])
	{
		if (ciclo < 1)
		{
			rotaPelota += 10.0f;
			printf("\n rotaPelota es: %f\n", rotaPelota);
			ciclo++;
			ciclo2 = 0;
			printf("\n Presiona la tecla I para poder habilitar la variable\n");
		}

	}

	if (keys[GLFW_KEY_I])
	{
		if (ciclo2 < 1)
		{
			ciclo = 0;
			printf("\n Ya puedes modificar la posicion\n");
		}
	}

}

void iniciaAnimacionKeyframe() {
	if (reproduciranimacion < 1)
	{
		if (play == false && (FrameIndex > 1))
		{
			resetElements();
			//First Interpolation				
			interpolation();
			play = true;
			playIndex = 0;
			i_curr_steps = 0;
			reproduciranimacion++;
			//printf("\n acciona la palanca con el click derecho para habilitar la animaci�n'\n");
			habilitaranimacion = 0;

		}
		else
		{
			play = false;

		}
	}
}

void reseteaAnimacionKeyframe() {
	if (habilitaranimacion < 1 && reproduciranimacion>0)
	{
		//printf("Ya puedes reproducir de nuevo la animacion soltando el click derecho'\n");
		reproduciranimacion = 0;
	}
}