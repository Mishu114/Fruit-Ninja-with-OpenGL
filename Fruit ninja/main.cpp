//
//  main.cpp
//  3D Object Drawing
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "cube.h"
#include "stb_image.h"
#include "Sphere2.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float run_speed = 0.004f;
float shoot_angle = 0.0f;
int fruit_count = 0;
//initial values

float gun_x=0.0f;
float gun_y=0.7f;
float balloon_x=1.5f;
float balloon_y = 1.2f;

bool fruit = true;
float fruit_y = 0.0f;
float inc = 1.0f;
float sc_x = 0.0;
float high_score = 123;

// camera
float camera_z = 0.0f;
Camera camera(glm::vec3(0.0f, 1.0f, -6.5f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 10.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 10.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);
SpotLight spotlight(
    0.0f,1.0f,-3.0f,
    0.0f, -1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    30.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);

DirectionLight directlight(
    0.0f, 1.0f, 0.0f,
    .0f, .0f, .0f,
    .0f, .0f, .0f,
    0.0f, 0.0f, 0.0f,
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f //k_q
);
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.50f,  1.50f,  -3.0f),
    glm::vec3(1.5f,  1.5f,  10.0f),
    glm::vec3(-1.5f,  1.5f,  -3.0f),
    glm::vec3(-1.5f,  1.5f,  10.0f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool directionLighton = true;
bool spotlighton = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool shoot = false;
float bullet_z = -5.2f;
int score = 3;
bool touched = false;
float scale_sc = 2.0f;
// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;
float speed = 0.003;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();
    Sphere point = Sphere();
    point.setRadius(0.001f);
    float inc2 = 0.0f;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    string diffuseMapPath = "grass.jpg";
    string specularMapPath = "grass.jpg";
    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath2 = "game_over.png";
    string specularMapPath2 = "game_over.png";
    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath3 = "watermelon_out.jpg";
    string specularMapPath3 = "watermelon_out.jpg";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 waterOut = Sphere2(0.1f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap3, specMap3, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPath3 = "watermelon_in.jpg";
    specularMapPath3 = "watermelon_in.jpg";
    diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere2 waterIn = Sphere2(0.09f, 144, 72, glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.898f, 0.459f, 0.125f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f, diffMap3, specMap3, 0.0f, 0.0f, 1.0f, 1.0f);


    string diffuseMapPathnum = "0.png";
    string specularMapPathnum = "0.png";
    unsigned int diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube zero = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);


    diffuseMapPathnum = "1.png";
    specularMapPathnum = "1.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube one = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "2.png";
    specularMapPathnum = "2.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube two = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "3.png";
    specularMapPathnum = "3.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube three = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "4.png";
    specularMapPathnum = "4.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube four = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "5.png";
    specularMapPathnum = "5.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube five = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "6.png";
    specularMapPathnum = "6.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube six = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "7.png";
    specularMapPathnum = "7.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube seven = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "8.png";
    specularMapPathnum = "8.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube eight = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "9.png";
    specularMapPathnum = "9.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube nine = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "your_score.png";
    specularMapPathnum = "your_score.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube yourScore = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    diffuseMapPathnum = "high_score.png";
    specularMapPathnum = "high_score.png";
    diffMapnum = loadTexture(diffuseMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMapnum = loadTexture(specularMapPathnum.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube highScore = Cube(diffMapnum, specMapnum, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    Cube score_block[10] = { zero,one,two,three,four,five,six,seven,eight,nine };
    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------

    std::random_device rd;  // Seed for the random number engine
    std::mt19937 gen(rd());  // Mersenne Twister engine
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);
    float randomValue = dis(gen);


    int count = 1;
    int num = 0;
    float angle = 90.0f;
    float extra = 0.0f;
    float draw = true;
    camera.Position = glm::vec3(0.0f, 1.0f, -3.0f);
    while (!glfwWindowShouldClose(window))
    {
        
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        

        

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        directlight.setUpDirectionLight(lightingShader);

        spotlight.setUpSpotLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);



        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        func(cubeVAO, lightingShader, model);


        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

       
        //if (balloon)
        //{
        //    glm::mat4 modelForSphere = glm::mat4(1.0f);
        //    modelForSphere = glm::translate(model, glm::vec3(balloon_x, balloon_y, 5.0f));
        //    sphere.drawSphere(lightingShader, modelForSphere);
        //}
        //glm::mat4 modelForSphere = glm::mat4(1.0f);
        //modelForSphere = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
        //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //modelForSphere = modelForSphere * rotateZMatrix;
        //sphere.drawSphere(lightingShader, modelForSphere);

        //speed = score * 0.001f;
        fruit_y += speed;
        if (score == 3)
        {
            draw = true;
        }

        if (2.5f - fruit_y <= 0.0f)
        {
            fruit_y = 0.0f;
            randomValue = dis(gen);
            angle = 90.0f;
            extra = 0.0f;
            if (count != num && draw)
            {
                score--;
            }
            count++;
            num = count - 1;
            speed = score * 0.001f;
        }

        if (shoot)
        {
            if (randomValue + 0.1f >= gun_x && randomValue - 0.1f <= gun_x)
            {
                float yy = 2.5f - fruit_y;
                if (gun_y + 0.5f >= yy - 0.1f && yy + 0.1f >= gun_y)
                {

                    touched = true;
                    angle = 110.0f;
                    extra = 0.1;
                    if (num != count)
                    {
                        if (score < 5)
                        {
                            score++;
                        }
                        fruit_count++;
                        num = count;
                    }
                        
                }
            }
        }

            
        
        float xx = 0.03f;
        for (int i = 0; i < score; i++)
        {
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(-0.5f-xx, 1.5f, -1.2f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
            modelForSphere = modelForSphere * translateMatrix * rotateZMatrix * scaleMatrix;
            point.drawSphere(lightingShader, modelForSphere);

            glm::mat4 modelForSphere2 = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(-0.5f - xx, 1.5f, -1.2f));
            rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            modelForSphere2 = modelForSphere2 * translateMatrix * rotateZMatrix * scaleMatrix;
            point.drawSphere(lightingShader, modelForSphere2);
            xx += 0.1f;
        }
            

        if (score>0 && fruit)
        {
            // fruit-1
            glm::mat4 modelForSphere = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(randomValue+extra, 2.5f-fruit_y, -1.2f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelForSphere = modelForSphere * translateMatrix * rotateXMatrix * rotateYMatrix;
            waterOut.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);
            waterIn.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);

            glm::mat4 modelForSphere2 = glm::mat4(1.0f);
            translateMatrix = glm::translate(model, glm::vec3(randomValue - extra, 2.5f -fruit_y, -1.2f));
            rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-angle), glm::vec3(0.0f, 1.0f, 0.0f));
            rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            modelForSphere2 = modelForSphere2 * translateMatrix * rotateXMatrix * rotateYMatrix;
            waterOut.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);
            waterIn.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere2);
        }


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        float cr, cg, cb;
        if (pointLightOn)
        {
            cr = 0.8f;
            cg = 0.8f;
            cb = 0.8f;
        }
        else
        {
            cr = 0.0f;
            cg = 0.0f;
            cb = 0.0f;
        }
        // point lights


        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(cr, cg, cb));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();
        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);



        //scoreboard
        int A[3];
        A[0] = 0;
        A[1] = 0;
        A[2] = 0;
        int ff = fruit_count;
        int inddd = 2;
        while (ff > 0) {
            A[inddd] = ff % 10;
            ff /= 10;
            inddd--;
        }



        translateMatrix = glm::translate(identityMatrix, glm::vec3(sc_x, 5.3f, -45.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_sc, scale_sc, 0.05f));
        glm::mat4 modelForScore1 = model * translateMatrix * scaleMatrix;
        score_block[A[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore1);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(sc_x- scale_sc, 5.3f, -45.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_sc, scale_sc, 0.05f));
        glm::mat4 modelForScore2 = model * translateMatrix * scaleMatrix;
        score_block[A[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore2);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(sc_x- (2* scale_sc), 5.3f, -45.5f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_sc, scale_sc, 0.05f));
        glm::mat4 modelForScore3 = model * translateMatrix * scaleMatrix;
        score_block[A[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForScore3);






        //grass
        translateMatrix = glm::translate(identityMatrix, glm::vec3(-2.5f, 0.1f, -5.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(5.0f, 0.2f, 10.0f));
        glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
        cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        //score = 0;
        if (score == 0)
        {
            draw = false;
            sc_x = 2.5f;
            scale_sc = 1.5f;



            //high score
            int Arr[3];
            Arr[0] = 0;
            Arr[1] = 0;
            Arr[2] = 0;
            int ff = high_score;
            int inddd = 2;
            while (ff > 0) {
                Arr[inddd] = ff % 10;
                ff /= 10;
                inddd--;
            }
            translateMatrix = glm::translate(identityMatrix, glm::vec3(sc_x, 7.8f, -45.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_sc, scale_sc, 0.05f));
            glm::mat4 modelForHScore1 = model * translateMatrix * scaleMatrix;
            score_block[Arr[0]].drawCubeWithTexture(lightingShaderWithTexture, modelForHScore1);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(sc_x - scale_sc, 7.8f, -45.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_sc, scale_sc, 0.05f));
            glm::mat4 modelForHScore2 = model * translateMatrix * scaleMatrix;
            score_block[Arr[1]].drawCubeWithTexture(lightingShaderWithTexture, modelForHScore2);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(sc_x - (2 * scale_sc), 7.8f, -45.5f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_sc, scale_sc, 0.05f));
            glm::mat4 modelForHScore3 = model * translateMatrix * scaleMatrix;
            score_block[Arr[2]].drawCubeWithTexture(lightingShaderWithTexture, modelForHScore3);







            //game over
            translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, -0.7f, 1.9f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 5.0f, 0.1f));
            glm::mat4 modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            cube2.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7f, 2.5f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.5f, 0.5f, 0.1f));
            modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            yourScore.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);

            translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.7f, 3.0f, 1.0f));
            scaleMatrix = glm::scale(identityMatrix, glm::vec3(3.5f, 0.5f, 0.1f));
            modelMatrixForContainer3 = translateMatrix * scaleMatrix;
            highScore.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer3);
        }




        //translateMatrix = glm::translate(identityMatrix, glm::vec3(2.05f, 0.1f, grass2));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.0f, 0.2f, 10.0f));
        //glm::mat4 modelMatrixForContainer4 = translateMatrix * scaleMatrix;
        //cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer4);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();
    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 22.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void func(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.1;
    float width = 5;
    float length = 10;

    //base-1
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.1, 0.5, 0.3);


    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix;


    //wall
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-3.0f, 0.0f, 2.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(6.0f, 5.0f, 0.1f));
    model = alTogether * translateMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.8, 0.8, 0.5);

    float an = 90.0f + shoot_angle;
    if (shoot)
    {
        shoot_angle += inc;
        an = 90.0f + shoot_angle;
        printf("%f\n", an);
        if (an == 90.0f)
        {
            shoot = false;
            shoot_angle = 0.0f;
            inc = 1.0f;
        }

    }
     

    if (an >= 180.0)
        inc = - 1.0f;

    //gun
    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x, gun_y+0.05f, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.05f, 0.5f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(an), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translateMatrix * rotateXMatrix * rotateYMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0,1.0,0.5);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(gun_x-0.03f, gun_y+0.05, -1.5f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.05f, 0.15f, 0.05f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(an-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = alTogether * translateMatrix * rotateXMatrix  * scaleMatrix;
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0, 1.0, 0.5);


    //shoot
    




}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {

        camera.ProcessKeyboard(FORWARD, deltaTime);

    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
        
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        gun_y += 0.05f;

    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        gun_y -= 0.05f;
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        gun_x += 0.05f;

    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        gun_x -= 0.05f;
    }


    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
        if(!shoot)
            shoot = true;
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if(score==0)
            score = 3, speed=.003;
        //count = 1;
        //num = 0;
    }

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(YAW_R, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(PITCH_U, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(PITCH_D, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ROLL_L, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(ROLL_R, deltaTime);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{   
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (directionLighton)
        {
            directlight.turnOff();
            directionLighton = !directionLighton;
        }
        else
        {
            directlight.turnOn();
            directionLighton = !directionLighton;
        }
    }
    else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    else if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotlighton)
        {
            spotlight.turnOff();
            spotlighton = !spotlighton;
        }
        else
        {
            spotlight.turnOn();
            spotlighton = !spotlighton;
        }
    }
    else if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            directlight.turnAmbientOff();
            spotlight.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            directlight.turnAmbientOn();
            spotlight.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    else if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            directlight.turnDiffuseOff();
            spotlight.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            directlight.turnDiffuseOn();
            spotlight.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    else if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            directlight.turnSpecularOff();
            spotlight.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            directlight.turnSpecularOn();
            spotlight.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}