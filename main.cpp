/*
Semestre 2024-1
Animaci�n:
Sesi�n 1:
Simple o b�sica:Por banderas y condicionales (m�s de 1 transformaci�n geom�trica se ve modificada
Sesi�n 2
Compleja: Por medio de funciones y algoritmos.
Adicional.- Textura Animada
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <time.h>
#include <map>
#include <array>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

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

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture FlechaTexture;
Texture NumerosTexture;
Texture Numero1Texture;
Texture Numero2Texture;



Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;
Model torsoGojo;

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
		3, 6, 0,
		3, 2, 6,
		3, 5, 6,
	};

	GLfloat piramideVertices[] = {
		0.0f, 0.0f, 4.0,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //G - 0
		1.0f, 1.52f, 0.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f, //F - 1
		2.0f, 0.0f, 0.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f, //E - 2

		0.0f, 3.0f, 0.0,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //H - 3

		-1.0f, 1.5f, 0.0f,   0.0f, 0.0f,	0.0f, -1.0f, 0.0f, //I - 4

		-2.0f, 0.0f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //J - 5

		0.0f, 0.0f, 0.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //K - 6

	};

	unsigned int cuboIndices[] = {
		0,1,2,
		0,1,3,
		0,3,5,
		3,4,5,
		4,5,7,
		4,6,7,
		1,2,7,
		2,6,7,
		0,7,5,
		0,5,1, 
		2,3,6,
		3,6,4,
	};

	GLfloat cuboVertices[] = {
		0.0f, 2.0f, 0.0,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //A - 0
		0.0f, -2.0f, 0.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f, //B - 1
		0.0f, -2.0f, 4.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f, //C - 2
		0.0f, 2.0f, 4.0,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //D - 3

		4.0f, 2.0f, 4.0f,   0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //E - 4
		4.0f, 2.0f, 0.0f,   0.0f, 0.0f,		0.0f, -1.0f, 0.0f, //F - 5

		4.0f, -2.0f, 4.0f,   0.0f, 0.0f,	0.0f, -1.0f, 0.0f, //G - 6
		4.0f, -2.0f, 0.0f,   0.0f, 0.0f,	0.0f, -1.0f, 0.0f, //H - 7

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
	obj9->CreateMesh(cuboVertices, cuboIndices, 64, 36);
	meshList.push_back(obj9);

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

int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 10.0f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	FlechaTexture = Texture("Textures/flechas.tga");
	FlechaTexture.LoadTextureA();
	NumerosTexture = Texture("Textures/numerosbase.tga");
	NumerosTexture.LoadTextureA();
	Numero1Texture = Texture("Textures/numero1.tga");
	Numero1Texture.LoadTextureA();
	Numero2Texture = Texture("Textures/numero2.tga");
	Numero2Texture.LoadTextureA();



	Model bola_M = Model();
	bola_M.LoadModel("Models/Bola/bola.obj");


	Model pinball_M = Model();
	pinball_M.LoadModel("Models/Pinball/pinball.obj");

	Model palanca_M = Model();
	palanca_M.LoadModel("Models/Pinball/palanca.obj");

	Model resorte_M = Model();
	resorte_M.LoadModel("Models/Pinball/resorte.obj");

	Model santuario_M = Model();
	santuario_M.LoadModel("Models/SantuarioMalevolo/SantuarioMalevolo.obj");
	
	Model torsoGojo_M = Model();
	torsoGojo_M.LoadModel("Models/Gojo/torsoSuperior.obj");

	Model cabezaGojo_M = Model();
	cabezaGojo_M.LoadModel("Models/Gojo/cara.obj");

	Model piernasGojo_M = Model();
	piernasGojo_M.LoadModel("Models/Gojo/piernasGojo.obj");


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
		0.5f, 0.5f,
		0.0f, 0.0f, -1.0f);
	
	// contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;
	
	// ================== Mapas de luces ==================
	std::map<std::string, PointLight> pointLightsMap;
	std::map<std::string, SpotLight> spotLightsMap;

	// mapas auxiliares para guardar todas las luces
	std::map<std::string, PointLight> pointLightsMapAux;
	std::map<std::string, SpotLight> spotLightsMapAux;

	// ================== Linterna ==================
	spotLightsMap["linterna"] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.8f, 0.05f, 0.01f,
		5.0f);
	spotLightCount++;

	// ================== Luz Santuario ==================
	pointLightsMap["santuario"] = PointLight(
		1.0f, 0.0f, 0.0f,
		4.0f, 4.0f,
		220.0f, 195.0f, -75.0f,
		0.005f, 0.003f, 0.004f
	);
	pointLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	// ========= variables para cronometro del 00 al 99 =========
	time_t start = time(NULL);

	std::map<int, std::array<float, 2>> coordsNumeros;

	coordsNumeros[0] = { 0.25f, -0.66f };
	coordsNumeros[1] = { 0.0f,	0.00f };
	coordsNumeros[2] = { 0.25f, 0.0f };
	coordsNumeros[3] = { 0.50f, 0.0f };
	coordsNumeros[4] = { 0.75f, 0.0f };
	coordsNumeros[5] = { 0.0f,	-0.33f };
	coordsNumeros[6] = { 0.25f, -0.33f };
	coordsNumeros[7] = { 0.50f, -0.33f };
	coordsNumeros[8] = { 0.75f, -0.33f };
	coordsNumeros[9] = { 0.0f,	-0.66f };

	int decenas = 0, unidades = 0;

	float unidadesCoordU = 0.0f, unidadesCoordV = 0.0f;
	float decenasCoordU = 0.0f, decenasCoordV = 0.0f;

	// ========= variables para animaci�n de la bola =========
	float movOffset = 0.05f;
	float movBola = 0.0f;
	float tiempoTranscurrido = 0.0f;


	// variables para la animacion del resorte/palanca
	bool animacionResorteContrae = false;
	const float contraeResorteOffset = 0.5f;
	const float muevePalancaOffset = 0.5f;
	float contraeResorte = 100.0f;
	float muevePalanca = 0.0f;


	const float sueltaPalancaOffset = 2.0f;
	const float expandeResorteOffset = 2.0f;
	bool animacionResorteExpande = true;


	// variables para la camara del avatar
	bool cambioPersonaje = false;
	glm::vec3 posicionGojo = glm::vec3(-500.0f, 0.0f, 0.0f);
	//float rotacionGojo = 180.0f;
	
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());


		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
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
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLightsMap["linterna"].SetFlash(lowerLight, camera.getCameraDirection());

		// =================== Paso de map a array =================== 
		// pasamos el contenido de los mapas a las listas
		Map_T::toArray(pointLightsMap, pointLights, MAX_POINT_LIGHTS);
		Map_T::toArray(spotLightsMap, spotLights, MAX_SPOT_LIGHTS);

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);

		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
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


		//==================================Instanciando primitiva===================================
		model = glm::mat4(1.0);
		//color = glm::vec3(0.0f, 0.0f, 1.0f);
		model = glm::translate(model, glm::vec3(-50.0f, 6.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[8]->RenderMesh();

		
		// ======================== bola 1 ========================
		 
		movBola -= movOffset * deltaTime;
		tiempoTranscurrido += movOffset * deltaTime;
		
		// Actualiza la posici�n de la pelota en funci�n del tiempo transcurrido
		float nuevaAltura = calcularPosicionY(tiempoTranscurrido);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + movBola, -1.5f + nuevaAltura, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		bola_M.RenderModel();
		
		// Para posicionar la camara en el avatar
		if (mainWindow.getCamara()) {
			if (!cambioPersonaje) {
				camera.setPosition(posicionGojo.x, posicionGojo.y + 250.0f, posicionGojo.z + 300.0f);
				cambioPersonaje = true;
			}

			posicionGojo =  glm::vec3(camera.getCameraPosition().x, camera.getCameraPosition().y - 250.0f, camera.getCameraPosition().z - 300.0f);


		}
		else {
			cambioPersonaje = false;
		}

		// ========================= Piernas gojo =========================

		model = glm::mat4(1.0);
		model = glm::translate(model, posicionGojo);
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
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
		model = glm::translate(model, glm::vec3(15.0f, -1.0f, 0.0f));
		modelaux = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		pinball_M.RenderModel();

		// ======================== palanca ========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(400.0f + muevePalanca, 179.0f, -155.0f));
		//model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		color = glm::vec3(1.0f, 0.0f, 0.0f);
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		palanca_M.RenderModel();

		// colocamos de nuevo el color en blanco para las siguientes texturas
		color = glm::vec3(1.0f, 1.0f, 1.0f);

		// ======================== resorte ========================
		model = modelaux;
		model = glm::translate(model, glm::vec3(460.0f, 178.0f, -155.0f));
		model = glm::scale(model, glm::vec3((1.0f * contraeResorte) / 100, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		resorte_M.RenderModel();

		// ======================== animacion resorte/palanca ========================
		if (mainWindow.getClickDerecho()) {
			animacionResorteContrae = true;
			animacionResorteExpande = false;
		}
		else {
			animacionResorteContrae = false;
			animacionResorteExpande = true;
		}

		if (animacionResorteContrae) {
			
			if (muevePalanca <= 35.0f) {
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
		}

		// ======================== Santuario Malevolo ========================
		model = modelaux;
		glm::vec3 posicionSantuario = glm::vec3(220.0f, 215.0f, -75.0f);
		model = glm::translate(model, glm::vec3(220.0f, 215.0f, -75.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		santuario_M.RenderModel();


		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Reinicia el salto cuando la pelota toca el suelo
		if (nuevaAltura <= 0.0f) {
			tiempoTranscurrido = 0.0f;
		}

		// =================== contador del 00 al 99 ciclico ===================

		if (difftime(time(NULL), start) >= 1.0f) {
			if (unidades == 9) {
				unidades = 0;
				
				if (decenas == 9) {
					decenas = 0;
				} else {
					decenas += 1;
				}
			} else {
				unidades += 1;
			}

			start = time(NULL);
		}

		// =================== Unidades ===================
		unidadesCoordU = coordsNumeros[unidades][0];
		unidadesCoordV = coordsNumeros[unidades][1];

		toffset = glm::vec2(unidadesCoordU, unidadesCoordV);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();

		// =================== Decenas ===================
		decenasCoordU = coordsNumeros[decenas][0];
		decenasCoordV = coordsNumeros[decenas][1];

		toffset = glm::vec2(decenasCoordU, decenasCoordV);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-13.0f, 10.0f, -6.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		NumerosTexture.UseTexture();
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[6]->RenderMesh();


		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
