////
////  main.cpp
////  OpenGL_Shader_Example_step1
////
////  Created by CGIS on 30/11/15.
////  Copyright © 2015 CGIS. All rights reserved.
//

#define GLEW_STATIC

#include <iostream>
#include "glm/glm.hpp"//core glm functionality
#include "glm/gtc/matrix_transform.hpp"//glm extension for generating common transformation matrices
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLEW/glew.h"
#include "GLFW/glfw3.h"
#include <string>
#include "Shader.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#define TINYOBJLOADER_IMPLEMENTATION

typedef std::basic_string<char> string;

#include "Model3D.hpp"
#include "Mesh.hpp"

#include "Config.h"
#include "LightSource.hpp"
#include "Terrain.hpp"
#include "FrameTimer.h"
#include "Player.hpp"
#include "Water.h"
#include "WaterFrameBuffers.h"
//#include "ParticleRenderer.hpp"
#include "Particle.hpp"
#include "ParticleMaster.hpp"
#include "ParticleTexture.hpp"
#include "ShadowBox.hpp"
#include "ShadowFrameBuffer.hpp"

// vezi ca aparent nu se reflecta obiectele, doar skyboxul

std::vector<light::LightSource> lights;
std::vector<glm::vec3> lightsPos;

glm::vec3 skyColor(FOG_RED, FOG_GREEN, FOG_BLUE);
gps::Camera myCamera(glm::vec3(0.0f, 3.0f, 2.5f), glm::vec3(0.0f, 0.0f, -10.0f));
sec::FrameTimer frameTimer;
enty::Player player(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0));
enty::Terrain terrain(glm::vec3(-0.5f * TERRAIN_SIZE, 0.0f, -0.5f * TERRAIN_SIZE));
std::vector<enty::Entity> lamps;
sky::SkyBox skyBox;
wat::Water water;
wat::WaterFrameBuffers waterFrameBuffers;
wat::WaterFrameBuffers waterFrameBuffers2;

std::vector<glm::vec3> treesPos;
std::vector<enty::Entity> trees;

parti::ParticleMaster particleMaster;

shadow::ShadowFrameBuffer shadowFrameBuffer(SHADOW_WIDTH, SHADOW_HEIGHT);

bool cameraFocusObject = true;
bool cameraFree = false;
bool leftMouseButtonPress = false;
bool pressedKeys[POWER_2_10];
float angle = 0.0f;
float yaw = 0.0f;
float pitch = 0.0f;
double xOldMouse = 0.0f;
double yOldMouse = 0.0f;
float scrollValue = 0.0f;
bool scroll = false;

// transformation matrix
glm::mat4 model;
GLuint modelLocGeneral;
GLuint modelLocTerrain;
glm::mat4 view;
GLuint viewLocGeneral;
GLuint viewLocTerrain;
glm::mat4 projection;
GLuint projectionLocGeneral;
GLuint projectionLocTerrain;
GLuint skyColorLocGeneral;
GLuint skyColorLocTerrain;
GLuint cameraLocGeneral;
GLuint cameraLocTerrain;
GLuint planeLocGeneral;
GLuint planeLocTerrain;

// light
GLuint lightsPositionLocGeneral;
GLuint lightsPositionLocTerrain;
GLuint lightsColorLocGeneral;
GLuint lightsColorLocTerrain;
GLuint lightsAttenuationLocGeneral;
GLuint lightsAttenuationLocTerrain;

// shadow
GLuint lightSpaceTrMatrixShadowLoc;
GLuint lightSpaceTrMatrixTerrainLoc;

GLuint modelShadowLoc;

// Skybox
glm::mat4 skyBoxProjection;

glm::mat3 normalMatrix;
GLuint normalMatrixLocGeneral;
GLuint normalMatrixLocTerrain;

int retina_width, retina_height;
GLFWwindow* glWindow = NULL;

GLuint verticesVBO;
GLuint verticesEBO;
GLuint objectVAO;

gps::Shader myCustomShader;
gps::Shader terrainShader;
gps::Shader skyboxShader;
gps::Shader waterShader;
gps::Shader shadowMapShader;

GLuint shadowMapFBO;
GLuint depthMapTexture;

gps::Shader particleShader;

GLenum glCheckError_(const char *file, int line);
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseScollCallback(GLFWwindow * window, double xoffset, double yoffset);
void mouseButtonCallbask(GLFWwindow * window, int button, int action, int mods);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void windowResizeCallback(GLFWwindow* window, int width, int height);
void initOpenGLState();


enty::Entity rock(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));

///*
//*   -- IN PROGRESS --
//*   forma ramana asta
//*/
//
std::vector<const GLchar*> getSkyboxDayFaces()
{
    std::vector<const GLchar*> faces; 
    faces.push_back(SKYBOX_DAY_RIGHT); 
    faces.push_back(SKYBOX_DAY_LEFT); 
    faces.push_back(SKYBOX_DAY_TOP); 
    faces.push_back(SKYBOX_DAY_DOWN); 
    faces.push_back(SKYBOX_DAY_BACK); 
    faces.push_back(SKYBOX_DAY_FRONT);

    return faces;
}

std::vector<const GLchar*> getSkyboxNightFaces()
{
    std::vector<const GLchar*> faces;
    faces.push_back(SKYBOX_NIGHT_RIGHT);
    faces.push_back(SKYBOX_NIGHT_LEFT);
    faces.push_back(SKYBOX_NIGHT_TOP);
    faces.push_back(SKYBOX_NIGHT_DOWN);
    faces.push_back(SKYBOX_NIGHT_BACK);
    faces.push_back(SKYBOX_NIGHT_FRONT);

    return faces;
}

std::vector<str::TextureInfo> getTexturesInfo()
{
    std::vector<str::TextureInfo> texturesInfo;
    str::TextureInfo texture = { TERRAIN_TEXTURE_1 , TERRAIN_TEXTURE_1_TYPE };
    texturesInfo.push_back(texture);
    texture = { TERRAIN_TEXTURE_2 , TERRAIN_TEXTURE_2_TYPE };
    texturesInfo.push_back(texture);
    texture = { TERRAIN_TEXTURE_3 , TERRAIN_TEXTURE_3_TYPE };
    texturesInfo.push_back(texture);
    texture = { TERRAIN_TEXTURE_4 , TERRAIN_TEXTURE_4_TYPE };
    texturesInfo.push_back(texture);
    texture = { TERRAIN_BLEND_TEXTURE , TERRAIN_BLEND_TEXTURE_TYPE };
    texturesInfo.push_back(texture);

    return texturesInfo;
}

/*
*   -- IN PROGRESS --
*/
void initLights()
{
    lights.push_back(light::LightSource(glm::vec3(-1.0f, 100.0f, 140.0f), glm::vec3(1.0f, 1.0f, 1.0f)));

    for (int i = 0; i < lightsPos.size(); i++)
    {
        lights.push_back(light::LightSource(glm::vec3(lightsPos[i].x, lightsPos[i].y + 5, lightsPos[i].z), glm::vec3(2.0f, 2.0f, 0.0f), glm::vec3(1.0f, 0.01f, 0.002f)));
    }
}

void initLamps()
{
    for (int i = 0; i < lightsPos.size(); i++)
    {
        enty::Entity entity(glm::vec3(lightsPos[i].x, lightsPos[i].y, lightsPos[i].z ), glm::vec3(1, 1, 1), glm::vec3(0, 0, 0));
        entity.setRenderObject(gps::Model3D("Objects/streetlamp/Models/streetlamp.obj", "Objects/streetlamp/Models/"));
        lamps.push_back(entity);
    }
}

/*
*    -- IN PORGRESS --
*/
void initModels()
{
    player.setRenderObject(gps::Model3D("Objects/Tree/Tree.obj", "Objects/Tree/"));
    rock.setRenderObject(gps::Model3D("Objects/Tree/Tree.obj", "Objects/Tree/"));

    std::vector<str::TextureInfo> texturesInfo = getTexturesInfo();
    terrain.generate(texturesInfo, TERRAIN_HEIGHT_MAP, PROCEDURAL_TERRAIN);
    skyBox.Load(getSkyboxDayFaces(), getSkyboxNightFaces());
}

/*
*    -- IN PORGRESS --
*/
void initShaders()
{
    myCustomShader.loadShader("shaders/entityShader.vert", "shaders/entityShader.frag");
    terrainShader.loadShader("shaders/terrainShader.vert", "shaders/terrainShader.frag");
    skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
    waterShader.loadShader("shaders/waterShader.vert", "shaders/waterShader.frag");
    shadowMapShader.loadShader("shaders/simpleDepthMap.vert", "shaders/simpleDepthMap.frag");
    particleShader.loadShader("shaders/particleShader.vert", "shaders/particleShader.frag");
}

/*
*    -- IN PORGRESS --
*/
void initObjects()
{
    shadowFrameBuffer.initFrameBuffer();
    waterFrameBuffers.init();
    waterFrameBuffers2.init();

    water = wat::Water(waterFrameBuffers, "Objects/Water/waterDUDV.png", "Objects/Water/normalMap.png");
    water.addPostion(glm::vec3(-100.0f, 0.0f, 100.0f));
    water.addPostion(glm::vec3(-100.0f, 0.0f, -100.0f));
    water.addPostion(glm::vec3(100.0f, 0.0f, -100.0f));
    water.addPostion(glm::vec3(100.0f, 0.0f, 100.0f));

    water.addPostion(glm::vec3(-300.0f, 0.0f, 300.0f));
    water.addPostion(glm::vec3(-100.0f, 0.0f, 300.0f));
    water.addPostion(glm::vec3(100.0f, 0.0f, 300.0f));
    water.addPostion(glm::vec3(300.0f, 0.0f, 300.0f));
    water.addPostion(glm::vec3(300.0f, 0.0f, 100.0f));
    water.addPostion(glm::vec3(300.0f, 0.0f, -100.0f));
    water.addPostion(glm::vec3(300.0f, 0.0f, -300.0f));
    water.addPostion(glm::vec3(100.0f, 0.0f, -300.0f));
    water.addPostion(glm::vec3(-100.0f, 0.0f, -300.0f));
    water.addPostion(glm::vec3(-300.0f, 0.0f, -300.0f));
    water.addPostion(glm::vec3(-300.0f, 0.0f, -100.0f));
    water.addPostion(glm::vec3(-300.0f, 0.0f, 100.0f));
}

/*
*    -- IN PORGRESS --
*/
void initUniforms()
{

    shadowMapShader.useShaderProgram();
    modelShadowLoc = glGetUniformLocation(shadowMapShader.shaderProgram, "model");
    lightSpaceTrMatrixShadowLoc = glGetUniformLocation(shadowMapShader.shaderProgram, "lightSpaceTrMatrix");

    // for general objects
    myCustomShader.useShaderProgram();

    model = glm::mat4(1.0f);
    modelLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "model");
    glUniformMatrix4fv(modelLocGeneral, 1, GL_FALSE, glm::value_ptr(model));

    view = myCamera.getViewMatrix();
    viewLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "view");
    glUniformMatrix4fv(viewLocGeneral, 1, GL_FALSE, glm::value_ptr(view));

    projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, NEAR_PLANE, FAR_PLANE);
    projectionLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocGeneral, 1, GL_FALSE, glm::value_ptr(projection));

    cameraLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "cameraPosition");

    // light
    for (int i = 0; i < lights.size(); i++)
    {
        string lightsPositionLocGeneralStr("lightsPosition[" + std::to_string(i));
        lightsPositionLocGeneralStr.append("]");
        lightsPositionLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, lightsPositionLocGeneralStr.c_str());
        glUniform3fv(lightsPositionLocGeneral, 1, glm::value_ptr(lights[i].getPosition()));

        string lightsColorLocGeneralStr("lightsColor[" + std::to_string(i));
        lightsColorLocGeneralStr.append("]");
        lightsColorLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, lightsColorLocGeneralStr.c_str());
        glUniform3fv(lightsColorLocGeneral, 1, glm::value_ptr(lights[i].getColor()));

        string lightsAttenuationLocGeneralStr("lightsAttenuation[" + std::to_string(i));
        lightsAttenuationLocGeneralStr.append("]");
        lightsAttenuationLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, lightsAttenuationLocGeneralStr.c_str());
        glUniform3fv(lightsAttenuationLocGeneral, 1, glm::value_ptr(lights[i].getAttenuation()));
    }
    normalMatrixLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
    skyColorLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "skyColor");
    glUniform3fv(skyColorLocGeneral, 1, glm::value_ptr(skyColor));
    planeLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "plane");
    // for terrain
    terrainShader.useShaderProgram();

    lightSpaceTrMatrixTerrainLoc = glGetUniformLocation(terrainShader.shaderProgram, "lightSpaceTrMatrix");

    GLuint shadowMapTerrainLoc = glGetUniformLocation(terrainShader.shaderProgram, "shadowMap");
    glUniform1i(shadowMapTerrainLoc, 5);

    model = glm::mat4(1.0f);
    modelLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "model");
    glUniformMatrix4fv(modelLocTerrain, 1, GL_FALSE, glm::value_ptr(model));

    view = myCamera.getViewMatrix();
    viewLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "view");
    glUniformMatrix4fv(viewLocTerrain, 1, GL_FALSE, glm::value_ptr(view));

    //projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
    projectionLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "projection");
    glUniformMatrix4fv(projectionLocTerrain, 1, GL_FALSE, glm::value_ptr(projection));
    cameraLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "cameraPosition");

    // light
    
    for (int i = 0; i < lights.size(); i++)
    {
        string lightsPositionLocTerrainStr("lightsPosition[" + std::to_string(i));
        lightsPositionLocTerrainStr.append("]");
        lightsPositionLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, lightsPositionLocTerrainStr.c_str());
        glUniform3fv(lightsPositionLocTerrain, 1, glm::value_ptr(lights[i].getPosition()));

        string lightsColorLocTerrainStr("lightsColor[" + std::to_string(i));
        lightsColorLocTerrainStr.append("]");
        lightsColorLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, lightsColorLocTerrainStr.c_str());
        glUniform3fv(lightsColorLocTerrain, 1, glm::value_ptr(lights[i].getColor()));

        string lightsAttenuationLocTerrainStr("lightsAttenuation[" + std::to_string(i));
        lightsAttenuationLocTerrainStr.append("]");
        lightsAttenuationLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, lightsAttenuationLocTerrainStr.c_str());
        glUniform3fv(lightsAttenuationLocTerrain, 1, glm::value_ptr(lights[i].getAttenuation()));
    }

    normalMatrixLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "normalMatrix");
    skyColorLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "skyColor");
    glUniform3fv(skyColorLocTerrain, 1, glm::value_ptr(skyColor));
    planeLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "plane");
    //skybox
    skyBoxProjection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, NEAR_PLANE, FAR_PLANE);
    skyBox.init(skyboxShader, skyColor);

    //water
    water.setShader(waterShader);
    water.setProjection(projection);
    water.setSkyColor(skyColor);
    water.setLightPos(glm::vec3(0.0f, 10000.0f, 0.0f));

    // particle
}

/*
*    -- FINISH --
*/
bool initOpenGLWindow()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return false;
	}

	//for Mac OS X
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Shader Example", NULL, NULL);
	if (!glWindow) {
		fprintf(stderr, "ERROR: could not open window with GLFW3\n");
		glfwTerminate();
		return false;
	}

	glfwSetWindowSizeCallback(glWindow, windowResizeCallback);
	glfwMakeContextCurrent(glWindow);

	glfwWindowHint(GLFW_SAMPLES, 4);

	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

    glfwSetKeyCallback(glWindow, keyboardCallback);
    glfwSetCursorPosCallback(glWindow, mouseCallback);
    glfwSetScrollCallback(glWindow, mouseScollCallback);
    glfwSetMouseButtonCallback(glWindow, mouseButtonCallbask);
    //for RETINA display
	glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

	return true;
}

/*
*    camera movement, player movement, camera focus player, show ass triangles
*    -- FINISH --
*/ 
void processMovement()
{
    if (pressedKeys[GLFW_KEY_P])
    {
        cameraFocusObject = true;
        cameraFree = false;
    }
    else if (pressedKeys[GLFW_KEY_O])
    {
        cameraFree = true;
        cameraFocusObject = false;
    }

    if (pressedKeys[GLFW_KEY_9])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else if(pressedKeys[GLFW_KEY_8])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    }
    else if (pressedKeys[GLFW_KEY_0])
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    if (true == cameraFree)
    {
        myCamera.move(pressedKeys[GLFW_KEY_W], pressedKeys[GLFW_KEY_S], pressedKeys[GLFW_KEY_A], pressedKeys[GLFW_KEY_D], pressedKeys[GLFW_KEY_R], pressedKeys[GLFW_KEY_F]);
        if (true == leftMouseButtonPress)
        {
            myCamera.rotate(pitch, yaw);
        }
    }
    else if (true == cameraFocusObject)
    {
        player.update(pressedKeys[GLFW_KEY_W], pressedKeys[GLFW_KEY_S], pressedKeys[GLFW_KEY_A], pressedKeys[GLFW_KEY_D], pressedKeys[GLFW_KEY_SPACE]);
        if (true == scroll)
        {
            myCamera.addDistanceToObj(scrollValue);
            scroll = false;
        }

        if (true == leftMouseButtonPress)
        {
            myCamera.focusObject(player.getPosition(), player.getRotation(), yaw, pitch);
        }
        else
        {
            myCamera.focusObject(player.getPosition(), player.getRotation(), 0, 0);
        }
    }
    
    yaw = 0.0f;
    pitch = 0.0f;
}

//glm::mat4 computeLightSpaceTrMatrix()
//{
//    const GLfloat near_plane = 1.0f, far_plane = 10.0f;
//    glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
//    
//    //glm::vec3 lightDirTr = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightDir, 1.0f));
//    glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
//        glm::vec3(0.0f, 0.0f, 0.0f),
//        glm::vec3(0.0f, 1.0f, 0.0f));//glm::lookAt(player.getPosition() + glm::vec3(0, 1, 2), player.getPosition(), glm::vec3(0.0f, 1.0f, 0.0f));
//
//    return lightProjection * lightView;
//}

glm::mat4 computeLightSpaceTrMatrix()
{
    const GLfloat near_plane = 1.0f, far_plane = 20.0f;
    glm::mat4 lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);


    glm::mat4 lightView = myCamera.getViewMatrix();//glm::lookAt(glm::vec3(0, 1, 2), glm::vec3(0, 0, 0), glm::vec3(0.0f, 1.0f, 0.0f));

    return lightProjection * lightView;
}

void renderShadow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shadowMapShader.useShaderProgram();

    glUniformMatrix4fv(lightSpaceTrMatrixShadowLoc, 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));

    for (int i = 0; i < lamps.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, lamps[i].getPosition());
        model = glm::scale(model, lamps[i].getScale());
        glUniformMatrix4fv(modelShadowLoc, 1, GL_FALSE, glm::value_ptr(model));
        lamps[i].draw(shadowMapShader);
    }

    model = player.getModelMatrix();
    glUniformMatrix4fv(modelShadowLoc, 1, GL_FALSE, glm::value_ptr(model));
    player.draw(shadowMapShader);

    model = glm::mat4(1.0f);
    model = glm::translate(model, terrain.getPosition());
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    glUniformMatrix4fv(modelLocTerrain, 1, GL_FALSE, glm::value_ptr(model));
    //terrain.draw(shadowMapShader);

    //glBindFramebuffer(GL_FRAMEBUFFER, 0);
    //glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

/*
*    -- IN PORGRESS --
*   
*/
void renderScene(glm::vec4 plane)
{
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    myCustomShader.useShaderProgram();

    glUniform4fv(planeLocGeneral, 1, glm::value_ptr(plane));
    lightsColorLocGeneral = glGetUniformLocation(myCustomShader.shaderProgram, "lightsColor[0]");
    glUniform3fv(lightsColorLocGeneral, 1, glm::value_ptr(skyBox.getSkyLight()));
    //initialize the view matrix
    view = myCamera.getViewMatrix();
    glUniformMatrix4fv(viewLocGeneral, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3fv(cameraLocGeneral, 1, glm::value_ptr(myCamera.getPosition()));
    
    // rocks

    model = glm::mat4(1.0f);
    model = glm::translate(model, rock.getPosition());
    model = glm::scale(model, rock.getScale());
    glUniformMatrix4fv(modelLocGeneral, 1, GL_FALSE, glm::value_ptr(model));
    rock.draw(myCustomShader);
    // lamps
    for (int i = 0; i < lamps.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, lamps[i].getPosition());
        model = glm::scale(model, lamps[i].getScale());
        glUniformMatrix4fv(modelLocGeneral, 1, GL_FALSE, glm::value_ptr(model));
        lamps[i].draw(myCustomShader);
    }

    for (int i = 0; i < treesPos.size(); i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, treesPos[i]);
        model = glm::scale(model, rock.getScale());
        glUniformMatrix4fv(modelLocGeneral, 1, GL_FALSE, glm::value_ptr(model));
        rock.draw(myCustomShader);
    }

    model = player.getModelMatrix();
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    glUniformMatrix3fv(normalMatrixLocGeneral, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(modelLocGeneral, 1, GL_FALSE, glm::value_ptr(model));
    player.draw(myCustomShader);

    terrainShader.useShaderProgram();

    
    //glUniform1i(glGetUniformLocation(terrainShader.shaderProgram, "shadowMap"), 5);

    //glm::mat4 projection1 = computeLightSpaceTrMatrix();//shadow::getLightSpaceTrMatrix(myCamera.getPosition(), myCamera.getUp(), myCamera.getCameraTarget());
    glUniformMatrix4fv(lightSpaceTrMatrixTerrainLoc, 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));

    glUniform4fv(planeLocTerrain, 1, glm::value_ptr(plane));
    lightsColorLocTerrain = glGetUniformLocation(terrainShader.shaderProgram, "lightsColor[0]");
    glUniform3fv(lightsColorLocTerrain, 1, glm::value_ptr(skyBox.getSkyLight()));
    glUniform3fv(cameraLocTerrain, 1, glm::value_ptr(myCamera.getPosition()));
    //initialize the view matrix
    glUniformMatrix4fv(viewLocTerrain, 1, GL_FALSE, glm::value_ptr(view));
    //initialize the model matrix
    model = glm::mat4(1.0f);
    model = glm::translate(model, terrain.getPosition());
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0, 1, 0));
    glUniformMatrix4fv(modelLocTerrain, 1, GL_FALSE, glm::value_ptr(model));
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
    glUniformMatrix3fv(normalMatrixLocTerrain, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glActiveTexture(GL_TEXTURE5);
    //glUniform1i(glGetUniformLocation(terrainShader.shaderProgram, "shadowMap"), 5);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);

    terrain.draw(terrainShader);
    // Skybox
    skyboxShader.useShaderProgram();
    skyBox.Draw(skyboxShader, view, skyBoxProjection, frameTimer.getFrameDuration());
}

/*
*    -- IN PORGRESS --
*/

void generateLightPos()
{
    srand(time(NULL));
    for (int i = 0; i < MAX_LIGHT_SOURCES - 1; i++)
    {
        float x, y, z;
        do
        {
            x = rand() % (RANDOM_TERRAIN_LIGHT_GEN_MAX - RANDOM_TERRAIN_LIGHT_GEN_MIN + 1) + RANDOM_TERRAIN_LIGHT_GEN_MIN;
            z = rand() % (RANDOM_TERRAIN_LIGHT_GEN_MAX - RANDOM_TERRAIN_LIGHT_GEN_MIN + 1) + RANDOM_TERRAIN_LIGHT_GEN_MIN;
            y = terrain.getTerrainHeight(x, z);
        } while (y < 0.0f);
        lightsPos.push_back(glm::vec3(x, y, z));
    }
}

void generateTreePos()
{
    srand(time(NULL));
    for (int i = 0; i < RANDOM_MAX_TRESS; i++)
    {
        float x, y, z;
        do
        {
            x = rand() % (RANDOM_TERRAIN_TREE_GEN_MAX - RANDOM_TERRAIN_TREE_GEN_MIN + 1) + RANDOM_TERRAIN_TREE_GEN_MIN;
            z = rand() % (RANDOM_TERRAIN_TREE_GEN_MAX - RANDOM_TERRAIN_TREE_GEN_MIN + 1) + RANDOM_TERRAIN_TREE_GEN_MIN;
            y = terrain.getTerrainHeight(x, z);
        } while (y < -3.0f);
        treesPos.push_back(glm::vec3(x, y, z));
        //printf("%f %f %f\n", x, y, z);
    }
}

void initFBOs()
{
    //generate FBO ID
    glGenFramebuffers(1, &shadowMapFBO);
    //create depth texture for FBO
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
        SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT32, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    //attach texture to FBO
    glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int main(int argc, const char * argv[])
{


	initOpenGLWindow();
    initOpenGLState();
    initShaders();
    initModels();
	initObjects();
    initFBOs();

    // ** start init static objects
    generateLightPos();
    initLights();
    initLamps();

    generateTreePos();
    // ** end init static objects

    initUniforms();



    frameTimer = sec::FrameTimer();

    /*
    
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        vezi ca daca e render apelat de mai multe ori in while unele chestii isi maresc viteaza, dau update mai des, repara dupa ce faci apa
        apa nu a re ceata
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    */
    parti::ParticleTexture pTexture("Objects/Particles/star.png", 1);

    particleMaster = parti::ParticleMaster(pTexture, particleShader, projection, 30, 4, 0.3f, 8, 1);
    particleMaster.randomizeRotation();
    particleMaster.setDirection(glm::vec3(0, 1, 0), 1.0f);
    particleMaster.setLifeError(0.1f);
    particleMaster.setSpeedError(0.4f);
    particleMaster.setScaleError(0.8f);

	while (!glfwWindowShouldClose(glWindow)) {
        processMovement();
        player.move(frameTimer.getFrameDuration(), terrain);
        //printf("%f %f %f\n", player.getPosition().x, player.getPosition().y, player.getPosition().z);
        particleMaster.generateParticles(frameTimer.getFrameDuration(), glm::vec3(1, 80, 140));
        particleMaster.update(myCamera.getPosition(), frameTimer.getFrameDuration());
        //particleMaster.count();
        
        //shadowFrameBuffer.bindFrameBuffer();
        //waterFrameBuffers2.bindRefractionFrameBuffer();
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        renderShadow();
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        waterFrameBuffers2.unbindCurrentFrameBuffer();
        //shadowFrameBuffer.unbindFrameBuffer();
        
        glEnable(GL_CLIP_DISTANCE0); // pentru clip plane la apa


        //reder reflection texture
        float distance = 2 * (myCamera.getPosition().y - water.getPosition().y);
        myCamera.addHeight(-distance);
        myCamera.invertCameraTarget();
        waterFrameBuffers.bindReflectionFrameBuffer();
        renderScene(glm::vec4(0.0f, 1.0f, 0.0f, -water.getPosition().y) + 0.8f);

        // render refraction texture
        myCamera.addHeight(distance);
        myCamera.invertCameraTarget();
        
        waterFrameBuffers.bindRefractionFrameBuffer();
        renderScene(glm::vec4(0.0f, -1.0f, 0.0f, water.getPosition().y));
        waterFrameBuffers.unbindCurrentFrameBuffer();

        // render to screne
        glDisable(GL_CLIP_DISTANCE0);

        renderScene(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
        water.setView(view, myCamera.getPosition());
        water.draw(frameTimer.getFrameDuration(), skyBox.getSkyLight(), depthMapTexture);
        
        particleMaster.renderParticles(myCamera.getViewMatrix());

        frameTimer.update();

		glfwPollEvents();
		glfwSwapBuffers(glWindow);
	}

	//close GL context and any other GLFW resources
    glfwTerminate();

	return 0;
}


/*
*   -- FINISH --
*/
GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

// callbacks

/*
*    keyboard imput
*    -- FINISH --
*/
void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < POWER_2_10)
    {
        if (action == GLFW_PRESS)
            pressedKeys[key] = true;
        else if (action == GLFW_RELEASE)
            pressedKeys[key] = false;
    }
}

/*
*   mouse scroll
*    -- FINISH --
*/
void mouseScollCallback(GLFWwindow * window, double xoffset, double yoffset)
{
    scrollValue = (float)yoffset * 0.5f;
    scroll = true;
}

/*
*   -- FINISH --
*/
void mouseButtonCallbask(GLFWwindow * window, int button, int action, int mods)
{
    if (GLFW_MOUSE_BUTTON_RIGHT == button)
    {
        if (GLFW_PRESS == action)
        {
            leftMouseButtonPress = true;
        }
        else
        {
            leftMouseButtonPress = false;
        }
    }
}


/*
*   mouse imput
*    -- FINISH --
*/
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    double xDelta = xPos - xOldMouse;
    double yDelta = yPos - yOldMouse;
    double mouseDistance = glm::sqrt(xDelta * xDelta + yDelta * yDelta);

    // update only if the mouse didn't jump
    if (mouseDistance < MOUSE_DELTA_JUMP)
    {
        yaw = float(xPos - xOldMouse) *MOUSE_MOVEMENT_SPEED;
        pitch = float(yPos - yOldMouse) *MOUSE_MOVEMENT_SPEED;
    }
    xOldMouse = xPos;
    yOldMouse = yPos;
}

/*
*   -- FINISH --
*/
void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
    //TODO
    //for RETINA display
    glfwGetFramebufferSize(glWindow, &retina_width, &retina_height);

    //set projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)retina_width / (float)retina_height, 0.1f, 1000.0f);
    //send matrix data to shader
    GLint projLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    //set Viewport transform
    glViewport(0, 0, retina_width, retina_height);
}

/*
*    -- FINISH --
*/
void initOpenGLState()
{
    glClearColor(BACKGROUND_RED, BACKGROUND_GREEN, BACKGROUND_BLUE, BACKGROUND_ALFA);
    glViewport(0, 0, retina_width, retina_height);

    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    glEnable(GL_CULL_FACE); // cull face
    glCullFace(GL_BACK); // cull back face
    glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}
