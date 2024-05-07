#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/teapot.h"
#include "glm/glm.hpp"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    GLuint sprites;
	int numSprites;
    float* locations;

    float tPrev;
    float angle;
    float Ytranslation;

    Plane plane;
    Torus rockRing, lavaPool;
    std::unique_ptr<ObjMesh> mesh, cow;
    GLuint Rock, Lava;
    SkyBox Sky;

    GLSLProgram prog, skyProg, spriteProg, PBRprog;
    void setMatrices(GLSLProgram &p, int progType);
    void compile();

public:
    SceneBasic_Uniform();

    void initScene();
    void update( float t );
    void render();
    void resize(int, int);

    void ProcessInput();
	void CameraMovement();
    void initSprites();
    void drawPBRmesh(const glm::vec3& pos, float rough, int metal, const glm::vec3 & color);
};

#endif // SCENEBASIC_UNIFORM_H
