#include "scenebasic_uniform.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;



//camera movement variables
glm::vec3 cameraPos = glm::vec3(3.0f, 2.5f, 4.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.75f, 0.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//orientation
float yaw = -120.0f; //minus ninety so it faces forward on start
float pitch = 0.0f;
//timing
float deltaTime = 0.0f; //time between current frame and last frame
float lastFrame = 0.0f;
//camera speed
float cameraSpeed = 0.01f; // adjust for SPEED

SceneBasic_Uniform::SceneBasic_Uniform() :
	tPrev(0.0f),
	angle(0),
	rockRing(2.5f * 2.0f, 0.4f * 2.0f, 50, 10), //(float MajorRadius, float MinorRadius, int numMajor, int numMinor)
	lavaPool(0.2* 2.0f, 0.5f* 2.0f, 50, 10),
	plane(150.0f, 150.0f, 1, 1), //(float xsize, float zsize, int xdivs, int zdivs)
	Sky(150.0f)
{
	mesh = ObjMesh::load("media/Skeleton/Skelly.obj", true); //load custom model here
	cow = ObjMesh::load("media/spot/spot_triangulated.obj", true); //load custom model here
}

void SceneBasic_Uniform::initScene()
{
	compile();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	prog.use();
	model = mat4(1.0f);
	projection = mat4(1.0f);
	GLuint cubeTex = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

	//
	view = glm::lookAt(
		glm::vec3(2.0f, 5.0f, 5.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.75f, 0.0f)
	);
	/**/

	initSprites();

	Ytranslation = -0.5f; //for animation

	GLuint Rock = Texture::loadTexture("media/texture/cement.jpg");
	GLuint Lava = Texture::loadTexture("media/texture/fire.png");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Rock);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Lava);	
	/**/

	glDepthMask(GL_TRUE);

	//The Fog is coming
	prog.use();
		prog.setUniform("Fog.MaxDist", 30.0f); //zero means no fog //30 default
		prog.setUniform("Fog.MinDist", 1.0f);
		prog.setUniform("Fog.Color", vec3(0.3f, 0.2f, 0.2f)); //RGB higher is brighter
		/**/

		//PBR type beat
		PBRprog.use();
		
		PBRprog.setUniform("Light[0].L", glm::vec3(10.0f, 10.0f, 10.0f));
		PBRprog.setUniform("Light[0].Position", view * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		
		PBRprog.setUniform("Light[1].L", glm::vec3(0.3f, 0.0f, 0.0f));
		PBRprog.setUniform("Light[1].Position", glm::vec4(0, 0.15f, -1.0f, 0));
		PBRprog.setUniform("Light[2].L", glm::vec3(5.0f, 0.0f, 0.0f));
		PBRprog.setUniform("Light[2].Position", view * glm::vec4(-7, 3, 7, 1));
		/**/
		prog.use();
		/**/


	//generate 3 lights
		float x, z;
		for (int i = 0; i < 3; i++) {  
			std::stringstream name;
			name << "lights[" << i << "].Position";
			x = 0.5f * cosf((glm::two_pi<float>() / 3) * i);
			z = 0.5f * sinf((glm::two_pi<float>() / 3) * i);
			prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
		}

		//intensity
		prog.setUniform("lights[0].L", vec3(0.8f, 0.0f, 0.0f));
		prog.setUniform("lights[1].L", vec3(0.8f, 0.0f, 0.0f));
		prog.setUniform("lights[2].L", vec3(0.8f, 0.0f, 0.0f));

		//ambient RGB
		prog.setUniform("lights[0].La", vec3(0.2f, 0.0f, 0.2f));
		prog.setUniform("lights[1].La", vec3(0.2f, 0.2f, 0.0f));
		prog.setUniform("lights[2].La", vec3(0.1f, 0.0f, 0.0f));

		//spotlight
		prog.setUniform("Spot.L", vec3(0.8f));
		prog.setUniform("Spot.La", vec3(0.5f, 0.4f, 0.4f));
		prog.setUniform("Spot.Exponent", 100.0f);
		prog.setUniform("Spot.Cutoff", glm::radians(45.0f));
		/**/

		
}

void SceneBasic_Uniform::compile()
{
	try {
		//
		spriteProg.compileShader("shader/point_sprite.vert");
		spriteProg.compileShader("shader/point_sprite.frag");
		spriteProg.compileShader("shader/point_sprite.gs");
		spriteProg.link();
		spriteProg.use();
		/**/

		PBRprog.compileShader("shader/PBR.vert");
		PBRprog.compileShader("shader/PBR.frag");
		PBRprog.link();
		PBRprog.use();
		/**/
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
		//
		/**/

		skyProg.compileShader("shader/skybox.vert");
		skyProg.compileShader("shader/skybox.frag");
		skyProg.link();
		/**/
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
	//animation
	float deltaT = t - tPrev;
	if (tPrev == 0.0f) deltaT = 0.0f;
	tPrev = t;
	angle += 2.0f * deltaT;
	if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
	Ytranslation = (sin(0.8f*t) * 0.2f); //skeleton bob

	ProcessInput();
}

void SceneBasic_Uniform::render()
{
	float currentFrame = glfwGetTime(); 
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	CameraMovement();

	// Render the skybox
		//skyProg.use(); //this breaks everything 
		model = mat4(1.0f);
		skyProg.setUniform("MVP", projection * view * model);
		Sky.render();
		prog.use();
		//
	
	//Render Spotlight
		vec4 lightPos = vec4(1.0f * cos(angle), 10.0f, 1.0f * sin(angle), 1.0f);
		prog.setUniform("Spot.Position", vec3(view * lightPos));
		mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
		prog.setUniform("Spot.Direction", normalMatrix * vec3(-lightPos));
		//

	drawPBRmesh(glm::vec3(0.0f), 0.6f, 0, glm::vec3(0.4f, 0.4f, 0.4f)); //skelly bob
	

	// Render the rock ring
		prog.setUniform("Material.Kd", vec3(0.9f, 0.9f, 0.9f));
		prog.setUniform("Material.Ka", vec3(0.5f, 0.5f, 0.5f));
		prog.setUniform("Material.Ks", vec3(0.1f, 0.1f, 0.1f));
		prog.setUniform("Material.Shininess", 010.0f);
		prog.setUniform("Material.TexDetail", 2);

		model = mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
		setMatrices(prog, 1);
		rockRing.render();

	// Render the lava pool
		prog.setUniform("Material.Kd", vec3(0.99f, 0.99f, 0.99f));
		prog.setUniform("Material.Ka", vec3(0.8f, 0.8f, 0.8f));
		prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));
		prog.setUniform("Material.Shininess", 200.0f);
		prog.setUniform("Material.TexDetail", 3);

		model = mat4(1.0f);
		model = glm::rotate(model, glm::radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, vec3(4.0f, 4.0f, 0.5f));
		setMatrices(prog, 1);
		lavaPool.render();

	// Render the plane
		prog.setUniform("Material.Kd", vec3(0.1f, 0.1f, 0.1f));
		prog.setUniform("Material.Ka", vec3(0.7f, 0.7f, 0.7f));
		prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
		prog.setUniform("Material.Shininess", 180.0f);
		prog.setUniform("Material.TexDetail", 2);

		model = mat4(1.0f);
		model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
		setMatrices(prog, 1);
		plane.render();
		
		spriteProg.use();
		model = mat4(1.0f);
		mat4 mv = view * model;
		spriteProg.setUniform("ModelViewMatrix", mv);
		spriteProg.setUniform("ProjectionMatrix", projection);
		glBindVertexArray(sprites);
		glDrawArrays(GL_POINTS, 0, numSprites);
		glFinish();
		//
		prog.use();

		/**/

}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
	height = h;
	projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.5f, 1000.0f);
}

void SceneBasic_Uniform::setMatrices(GLSLProgram& p, int progType)
{
	mat4 mv = view * model;
	
		p.setUniform("ModelViewMatrix", mv);
		p.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
		p.setUniform("MVP", projection * mv);
	
}

void SceneBasic_Uniform::ProcessInput()
{
	GLFWwindow* window = glfwGetCurrentContext(); //get current window
	//inputs for camera movement
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) //w forward
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) //s backward
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) //a left
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) //d right
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	//up and down
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) // space up
		cameraPos += cameraSpeed * cameraUp;
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) // ctrl down
		cameraPos -= cameraSpeed * cameraUp;
	/**/
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraSpeed = 0.05f;
	else
		cameraSpeed = 0.01f;
	//inputs for camera orientation
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pitch += 0.25f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pitch -= 0.25f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		yaw -= 0.5f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		yaw += 0.5f;

	//mouse movement
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	glfwSetCursorPos(window, width / 2, height / 2);
	float xoffset = xpos - width / 2;
	float yoffset = height / 2 - ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); //hide cursor
}

void SceneBasic_Uniform::CameraMovement()
{
	//camera render
	glm::vec3 front; //for new camera orientation
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	// restart the camera position if it goes out of bounds >:)
	if (cameraPos.x <= -74.0f || cameraPos.x >= 74.0f ||
		cameraPos.z <= -74.0f || cameraPos.z >= 74.0f)
	{
		cameraPos = glm::vec3(10.0f, 2.5f, 10.0f);
	}
	cameraPos.y = 2.0f;
	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void SceneBasic_Uniform::initSprites()
{
	spriteProg.use();

	numSprites = 5000; //sprite count
	locations = new float[numSprites * 3];
	srand((unsigned int)(time(0)));

	float roomSize = 150.0f; //space the sprites fill
	for (int i = 0; i < numSprites; i++) {
		vec3 p(((float)rand() / RAND_MAX * roomSize) - roomSize * 0.5f,
			1.0f + ((float)rand() / RAND_MAX) * 2.0f,
			((float)rand() / RAND_MAX * roomSize) - roomSize * 0.5f);
		locations[i * 3] = p.x;
		locations[i * 3 + 1] = p.y;
		locations[i * 3 + 2] = p.z;
	}

	GLuint handle;
	glClear(GL_DEPTH_BUFFER_BIT); //CLEAR THE DEPTH BUFFER
	glGenBuffers(1, &handle);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glBufferData(GL_ARRAY_BUFFER, numSprites * 3 * sizeof(float), locations, GL_STATIC_DRAW);
	delete[] locations;
	glGenVertexArrays(1, &sprites);
	glBindVertexArray(sprites);
	glBindBuffer(GL_ARRAY_BUFFER, handle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, ((GLubyte*)NULL + (0)));
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	//
	GLuint Stex = Texture::loadTexture("media/texture/fire.png");
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, Stex);
	spriteProg.setUniform("SpriteTex", 0);
	spriteProg.setUniform("Size2", 0.15f);
	/**/
	prog.use();
}

void SceneBasic_Uniform::drawPBRmesh(const glm::vec3& pos, float rough, int metal, const glm::vec3& color)
{
	PBRprog.use();
	PBRprog.setUniform("Material.Rough", rough);
	PBRprog.setUniform("Material.Metal", metal);
	PBRprog.setUniform("Material.Color", color);
	//animation
	model = mat4(1.0f);
	//
	model = glm::rotate(model, glm::radians(-45.0f), vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(45.0f), vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(20.0f), vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, vec3(0.0f, Ytranslation - 5.0f, 0.0f));
	model = glm::scale(model, vec3(10.0f));
	/**/

	setMatrices(PBRprog, 3);
	mesh->render();
	prog.use();

}
