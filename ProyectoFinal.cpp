/*
Pr�ctica 7: Iluminaci�n 1 
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION


#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
/* pa windows */
#include <glew.h>
#include <glfw3.h>

///pa linux
//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
/* pa windows */
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>

//pa linux
//#include<glm/glm.hpp>
//#include<glm/gtc/matrix_transform.hpp>
//#include<glm/gtc/type_ptr.hpp>

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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

/////Proyectoo

/* Relieve */
Model relieve;
Model pisoSecundario;

/********* Hora de aventura *********/
Model castillo_rey_helado;
Model Montanas[2];

Model Arboles_HoraDeAventura[10]; // Por lo mientras 10 arboles
Model Rainicorn;
Model Fuente01;

/*************************************/

Model cubcesped;
Model cubpavim;
Model cubcarretera;
////dayly bugle
Model daylybugle;
///AVENGERS
Model avengers;
///STARK
Model stark;
///Sanctum

Model Sanctum;
///spiderman
Model Spiderman;
/// araña
Model arana;
////scooter deadpol vehiculo motorizado
Model scooterdeadpool;
///bote de basura
Model bote_basura;
///arbol pino
Model pino;
//lampara deadpool
Model lamp_deadpool;

///mesasillaplatico
Model mesasillaplastico;
///estatua dorada spider man
Model estatuaspidermandorada;

/// Bicicleta roja
Model bicicletaroja;
///BALON BASQUET
Model pelotabasquet;
////tienda comic ventana con transparencia
Model tiendacomic;
///tienda de dulce transparencia
Model candystore;
Model tiendavideojuegos;
///// modelos con spotlights
Model Farospot;
Model Torrecontrolspot;
Model proyectorspot;

Skybox skybox;
Skybox skybox2;

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


//funci�n de calculo de normales por promedio de v�rtices 
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

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);


	////Proyecto

	/* Relieve */

	relieve = Model();
	relieve.LoadModel("Models/relieve.obj");

	pisoSecundario = Model();
	pisoSecundario.LoadModel("Models/piso_secundario.obj");


	/*cubcarretera = Model();
	cubcarretera.LoadModel("Models/carratera.obj");

	cubcesped = Model();
	cubcesped.LoadModel("Models/cubocesped.obj");

	cubpavim = Model();
	cubpavim.LoadModel("Models/cubopavimento.obj");*/

	///bugle
	daylybugle = Model();
	daylybugle.LoadModel("Models/dailybugle.obj");
	///torre avengers
	avengers = Model();
	avengers.LoadModel("Models/avengers.obj");
	///stark
	stark = Model();
	stark.LoadModel("Models/STARK.obj");
	///sanctum
	Sanctum = Model();
	Sanctum.LoadModel("Models/sanctumroque.obj");
	///araña 
	arana = Model();
	arana.LoadModel("Models/arana.obj");
	//spider man
	Spiderman = Model();
	Spiderman.LoadModel("Models/spiderman.obj");

	///scooter deadpool
	scooterdeadpool = Model();
	scooterdeadpool.LoadModel("Models/scooterdead.obj");
	///bote de basura
	bote_basura = Model();
	bote_basura.LoadModel("Models/botebasura.obj");
	///pino
	pino = Model();
	pino.LoadModel("Models/pino.obj");

	//model lamp dead pool
	lamp_deadpool = Model();
	lamp_deadpool.LoadModel("Models/lamparadeadpoolcalle.obj");
	///mesa y silla
	mesasillaplastico = Model();
	mesasillaplastico.LoadModel("Models/mesasilla.obj");
	///estatua dorada spiderman
	estatuaspidermandorada = Model();
	estatuaspidermandorada.LoadModel("Models/estatuaspiderman.obj");
	
///fuente agua
	//fuenteagua=Model();
	//fuenteagua.LoadModel("Models/fuenteagua.obj");
	////bicicleta roja
	bicicletaroja=Model();
	bicicletaroja.LoadModel("Models/bicicletaroja.obj");
	///pelota basquet
	pelotabasquet=Model();
	pelotabasquet.LoadModel("Models/pelotbasquet.obj");

/************************** ventana transparente*****************************/

	///tienda comic ventana transparente

	tiendacomic=Model();
	tiendacomic.LoadModel("Models/tiendacomic.obj");

	candystore=Model();
	candystore.LoadModel("Models/candystore.obj");
	tiendavideojuegos=Model();
	tiendavideojuegos.LoadModel("Models/tiendavideojuegos.obj");

/************************** modelosspotlights*****************************/
Farospot=Model();
Farospot.LoadModel("Models/faro.obj");
Torrecontrolspot=Model();
Torrecontrolspot.LoadModel("Models/airtrafic.obj");
proyectorspot=Model();
proyectorspot.LoadModel("Models/proyector.obj");





	
	
	/************************** Hora de aventura *****************************/

	// Castillo del rey helado
	castillo_rey_helado = Model();
	castillo_rey_helado.LoadModel("Models/king_castle.obj");

	// Montañas
	Montanas[0] = Model();
	Montanas[0].LoadModel("Models/montanas.obj");
	Montanas[1] = Model();
	Montanas[1].LoadModel("Models/montanas.obj");

	// Entrada unicornio
	Rainicorn = Model();
	Rainicorn.LoadModel("Models/rainicorn_solo.obj");

	// Fuente del centro de la alameda
	Fuente01 = Model();
	Fuente01.LoadModel("Models/fuente_alameda_centro.obj");



	/********************** Vegetacion ************************/
	Arboles_HoraDeAventura[0] = Model();
	Arboles_HoraDeAventura[0].LoadModel("Models/arbol_algodon_horaaventura1.obj");
	Arboles_HoraDeAventura[1] = Model();
	Arboles_HoraDeAventura[1].LoadModel("Models/arbol_algodon2.obj");



	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

/////noche
	std::vector<std::string> skyboxFaces2;
skyboxFaces2.push_back("Textures/Skybox/luna_rt.tga");
	skyboxFaces2.push_back("Textures/Skybox/luna_lf.tga");
	skyboxFaces2.push_back("Textures/Skybox/luna_dn.tga");
	skyboxFaces2.push_back("Textures/Skybox/luna_up.tga");
	skyboxFaces2.push_back("Textures/Skybox/luna_bk.tga");
	skyboxFaces2.push_back("Textures/Skybox/luna_ft.tga");
	
skybox2=Skybox(skyboxFaces2);
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f,-1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		-6.0f, 1.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;

	unsigned int spotLightCount = 0;
	////al final tendr+ia que ser esta la uno del faro
	
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//faro
	///-100.0f, 30.0f, -40.0f,

	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		-30.0f, 27.0f, -40.0f,
		5.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
///1.0f,2.0f
	//torre de control
	spotLights[2] = SpotLight(1.0f, 1.0f, 1.0f,
		3.0f, 2.0f,
		-35.0f, 30.0f, 50.0f,
		5.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	//proyector

	///posicion-85.0f, 2.0f, 0.0f,

	spotLights[3] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		-86.0f, 2.0f, 0.0f,
		5.0f, 0.0f, 0.0f,
		4.0f, 0.0f, 0.0f,
		20.0f);
	spotLightCount++;


	//se crean mas luces puntuales y spotlight 

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	bool cicloDia = true;
		double tiempoInicial = glfwGetTime();
	double tiempoUltimoCambio = tiempoInicial;
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
		///luz de este a oeste
		
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		double tiempoActualskybox = glfwGetTime();
		float tiempo;

		if (tiempoActualskybox - tiempoUltimoCambio >= 30.0)
		{

			cicloDia = (cicloDia == true) ? false : true;
			tiempoUltimoCambio = tiempoActualskybox;
			
			


			

			
		}
		if (cicloDia == true)
		{
			skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
			///no descomentar, no funciona dejarlo comentado es para la luz de este a oeste
			//for (GLfloat z = -20; z <= 20; ++z) {
				    //printf("Valor de z: %.2f\n", z); // Imprimir el valor de z
					//GLfloat z=-1;

				    //mainLight.SetDirection(0.0f,0.0f,-1);
				//shaderList[0].SetDirectionalLight(&mainLight);


					//}

			
			

		}
		else
		{
			skybox2.DrawSkybox(camera.calculateViewMatrix(), projection);
		}
		//skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		//sirve para que en tiempo de ejecuci�n (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		///no descomentar no funciona, dejarlo comentado es para la luz de oeste a este pero no funciona
		//GLfloat z=-2;
		//for (GLfloat x = -100; x <= 0; ++x) {
				   // printf("Valor de z: %.2f\n", x); // Imprimir el valor de z
					
				    //mainLight.SetDirection(x,0.0f,-1.0f);
				//shaderList[0].SetDirectionalLight(&mainLight);


					//}
		//mainLight.SetDirection(0.0f,0.0f,z);
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);


		glm::mat4 model(1.0);
		glm::mat4 model_relieve_principal(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));


		////Proyecto
		
		/************************ RELIEVE **********************/
		model_relieve_principal = glm::mat4(1.0);
		model_relieve_principal = glm::translate(model_relieve_principal, glm::vec3(1.5f, 1.0f, 0.0f));
		model_relieve_principal = glm::scale(model_relieve_principal, glm::vec3(5.0f, 4.0f, 3.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model_relieve_principal));
		relieve.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 4.0f, 3.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoSecundario.RenderModel();


		///segundo piso
model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(1.5f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-53.0f, 1.0f, 0.0f));

		model = glm::scale(model, glm::vec3(5.0f, 4.0f, 3.9f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pisoSecundario.RenderModel();



		////pasto
		///model = glm::mat4(1.0);

		/*model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(10.0f, 1.0f, 15.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		cubcesped.RenderModel();*/
		///////pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -16.00f));
		//model = glm::scale(model, glm::vec3(10.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();

		//////segundo pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 16.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 1.0f, 1.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();
		/////tercer pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(11.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 17.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();


		////////cuarto pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-11.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 17.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();

		///////calles
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-17.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(17.0f, 1.0f, 5.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubcarretera.RenderModel();
		//////calle2
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(17.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(17.0f, 1.0f, 5.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubcarretera.RenderModel();

		/////calle 3
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 22.0f));
		////model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(22.0f, 1.0f, 5.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubcarretera.RenderModel();
		/////calle 4
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -22.0f));
		////model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		//model = glm::scale(model, glm::vec3(22.0f, 1.0f, 5.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubcarretera.RenderModel();


		////pavimentooo
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -37.00f));
		//model = glm::scale(model, glm::vec3(22.0f, 1.0f, 10.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();

		/////pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 37.00f));
		//model = glm::scale(model, glm::vec3(22.0f, 1.0f, 10.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();

		/////pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(32.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 1.0f, 47.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();

		/////pavimento
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-32.0f, 0.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(10.0f, 1.0f, 47.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//cubpavim.RenderModel();

		/******************************** EDIFICIOS **************************/

		///////// daily bugle
		//-15.0 y -32-0f
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-32.0f, 1.2f, -15.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.2f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		daylybugle.RenderModel();

		////// avengers
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-10.0f, 4.0f, 50.0f));
		///model = glm::scale(model, glm::vec3(22.0f, 1.0f, 67.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		avengers.RenderModel();
		////stark
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(15.0f, 1.0f, 50.0f));
		//model = glm::scale(model, glm::vec3(0.002f,0.002f, 0.002f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		stark.RenderModel();

		///sanctum
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-32.0f, 1.2f, 5.0f));
		model = glm::scale(model, glm::vec3(1.0f,1.2f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Sanctum.RenderModel();

		//araña
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		arana.RenderModel();
		//Spiderman 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 1.25f, -15.0f));
		model = glm::scale(model, glm::vec3(0.75f,0.75f, 0.75f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Spiderman.RenderModel();
		////deadpool scooter vehiculo motorizado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-20.0f, 1.4f, -15.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		scooterdeadpool.RenderModel();
		///falta que se mueva
		
		///bote de basura
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-9.0f, 1.3f, 3.0f));
		model = glm::scale(model, glm::vec3(0.8f,0.8f, 0.8f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bote_basura.RenderModel();

		///arboles tipo hora de aventura pino con hielo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, 13.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();

		////segundo arboles tipo hora de aventura pino con hielo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.3f, -13.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pino.RenderModel();
		
	

/*lamparas deadpool*/
//1
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, 1.3f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp_deadpool.RenderModel();

///2 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.0f, 1.3f, -20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp_deadpool.RenderModel();


///3
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, 1.3f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp_deadpool.RenderModel();
///4
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.0f, 1.3f, 20.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp_deadpool.RenderModel();

///5
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(14.0f, 1.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp_deadpool.RenderModel();	

///6 
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-14.0f, 1.3f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		lamp_deadpool.RenderModel();	

//////////////////////////////////////

		////mesaconsilla
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		mesasillaplastico.RenderModel();	
		///estatua de marvel spiderman dorada
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-3.0f, 1.0f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		estatuaspidermandorada.RenderModel();
		///bicicleta roja
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 1.2f, 8.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		bicicletaroja.RenderModel();
		///pelotabasquet
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 1.4f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pelotabasquet.RenderModel();
		


/**************************ventanas transparentes*****************************/

		////tienda comic con ventana transparente
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-55.0f, 0.0f, -40.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tiendacomic.RenderModel();
		///tienda de dulce

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 0.0f, -10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		candystore.RenderModel();		
		///tienda videojuegos
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-50.0f, 5.0f, 10.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		tiendavideojuegos.RenderModel();
		/************************** modelosspotlights*****************************/


		///faro
		
model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-30.0f, 0.0f, -40.0f));
		//model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Farospot.RenderModel();
//model = glm::translate(model, glm::vec3(-35.0f, 4.0f, 40.0f));

		///control tower
model = glm::mat4(1.0);
model = glm::translate(model, glm::vec3(-35.0f, 3.0f, 50.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Torrecontrolspot.RenderModel();
	//proyector
model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-85.0f, 0.5f, 0.0f));


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		proyectorspot.RenderModel();



		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);






		/************ Hora de Aventura **************/

		// Castillo Rey Helado
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(50.0f, 1.0f, -40.0f));
		model = glm::scale(model, glm::vec3(55.0f, 55.0f, 55.0f));
		model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		castillo_rey_helado.RenderModel();

		// Montañas
	// Primeras montañas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(35.0f, 1.0f, -75.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Montanas[0].RenderModel();
	// Segundas montañas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, 1.0f, -40.0f));
		model = glm::scale(model, glm::vec3(35.0f, 35.0f, 35.0f));
		//model = glm::rotate(model, 100 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Montanas[1].RenderModel();

		// Entrada de unicornio arcoiris
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Rainicorn.RenderModel();

		// Fuente de la alameda - Centro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.4f, 1.0f, 4.5f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Fuente01.RenderModel();

/************************ VEGETACION: Hora de aventura ***************************/
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-6.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.6f, 2.6f, 2.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles_HoraDeAventura[0].RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-8.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.6f, 2.6f, 2.6f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Arboles_HoraDeAventura[1].RenderModel();

		/*

		//Instancia del coche 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f + mainWindow.getmuevex(), 0.5f, -3.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Kitt_M.RenderModel();

		//Llanta delantera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		color = glm::vec3(0.5f, 0.5f, 0.5f);//llanta con color gris
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera izquierda
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 8.0f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta delantera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(7.0f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();

		//Llanta trasera derecha
		model = modelaux;
		model = glm::translate(model, glm::vec3(15.5f, -0.5f, 1.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Llanta_M.RenderModel();
	

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();

		//Agave �qu� sucede si lo renderizan antes del coche y el helic�ptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		*/
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//AgaveTexture.UseTexture();
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		///meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
