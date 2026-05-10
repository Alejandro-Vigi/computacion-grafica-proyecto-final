// Proyecto Final Equipo #10
// Integrantes: 
// No. Cuenta 1: 320301159
// No. Cuenta 2: 319159709
// No. Cuenta 3: 319033515
// Fecha de entrega: 13 de mayo 2026
// Computación gráfica / Laboratorio de computación gráfica
// Profesor: Arturo Pérez de la Cruz

// Std. Includes
#include <string>
#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"
#include "Texture.h"

// Properties
const GLuint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// Modo día/noche — tecla P para cambiar entre ambos modos
bool lightsOn = false;  // false = modo día (ambient alto, luces apagadas)
                        // true  = modo noche (ambient bajo, luces encendidas)

// Control de animación del FBX del robot
bool playAnimationRobot = false;
// Tiempo acumulado de animación del robot
float robotAnimTime = 0.0f;
// Constante exacta de frames hecha en blender
const double animDuration = 41.4167;

// Control de animación del FBX de las personas caminando
bool playWalking = false;
// Tiempo acumulado de animación de las personas caminando
float walkingAnimTime = 0.0f;
// Constante exacta de frames hecha en blender
const double walkingDuration = 16.1333;

// Control de animación talking
bool playTalking = false;
// Tiempo acumulado de animación talking
float talkingAnimTime = 0.0f;
// Constante exacta de frames hecha en blender
const double talkingDuration = 21.9667;

// Control de animación Woman
bool playWoman = false;
// Tiempo acumulado de animación Woman
float womanAnimTime = 0.0f;
// Constante exacta de frames hecha en blender
const double womanDuration = 42.4333;

// Posiciones de las 3 point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(2.05f, 6.3f,  32.75f),        // Lámpara foco #1
    glm::vec3(2.05f, 6.3f,  17.55f),        // Lámpara foco #2
    glm::vec3(2.05f, 6.3f,  2.33f)          // Lámpara foco #3
};

//Posiciones de las 11 spotlights
glm::vec3 spotLightPositions[] = {
    glm::vec3(2.05f,    6.5f,   31.5f),     // Tubo #1
    glm::vec3(2.05f,    6.5f,   24.0f),     // Tubo #2
    glm::vec3(2.05f,    6.5f,   16.5f),     // Tubo #3
    glm::vec3(2.05f,    6.5f,   9.0f),      // Tubo #4
    glm::vec3(2.05f,    6.5f,   1.5f),      // Tubo #5
    glm::vec3(2.05f,    6.5f,   -6.0f),     // Tubo #6
    glm::vec3(2.05f,    6.5f,   -13.5f),    // Tubo #7
    glm::vec3(-7.75f,   6.5f,   -13.0f),    // Tubo #8
    glm::vec3(-7.75f,   6.5f,   -6.0f),     // Tubo #9
    glm::vec3(-7.75f,   6.5f,   1.5f),      // Tubo #10
    glm::vec3(-7.75f,   6.5f,   9.0f)       // Tubo #11
};

// Vértices del cubo (para visualizar posiciones de luces)
float vertices[] = {
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,-1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f,-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
    -0.5f, 0.5f, 0.5f,-1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 1.0f, 0.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
     0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f, 0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f,-0.5f,-0.5f, 0.0f,-1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
    -0.5f, 0.5f,-0.5f, 0.0f, 1.0f, 0.0f
};



int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Equipo #10: Carga de modelos y camara sintetica", nullptr, nullptr);
    
    // Pantalla completa para ver mejor el proyecto
    //GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    //const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    //GLFWwindow* window = glfwCreateWindow(mode->width, mode->height,
    //    "Proyecto Final Equipo #10", monitor, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader skyboxshader("Shader/skybox.vs", "Shader/skybox.frag");
    Shader skinnedShader("Shader/_skin_runtime.vs", "Shader/_tex_runtime.frag");

    // Load models
    Model CashierArea((char*)"Models/CashierArea/CashierArea.obj");
	Model Stand1((char*)"Models/Stand1/Stand1.obj");
	Model Stand2((char*)"Models/Stand2/Stand2.obj");
	Model Stand3((char*)"Models/Stand3/Stand3.obj");
	Model Stand4((char*)"Models/Stand4/Stand4.obj");
	Model Stand5((char*)"Models/Stand5/Stand5.obj");
    Model Robot((char*)"Models/Robot/Robot.fbx");
    Model Walking1((char*)"Models/Walking/Walking1.fbx");
    Model Walking2((char*)"Models/Walking/Walking2.fbx");
    Model Walking3((char*)"Models/Walking/Walking3.fbx");
    Model Talking((char*)"Models/Talking/Exhibitor.fbx");
    Model Talking2((char*)"Models/Talking/Reader.fbx");
    Model Talking3((char*)"Models/Talking/Clipboard.fbx");
    Model Woman((char*)"Models/Woman/Woman.fbx");

    GLfloat skyboxVertices[] = {
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };


    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,2,3,
        4,5,6,7,
        8,9,10,11,
        12,13,14,15,
        16,17,18,19,
        20,21,22,23,
        24,25,26,27,
        28,29,30,31,
        32,33,34,35
    };

    // VAO/VBO para los cubos indicadores de luces
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    
	// Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    //Skybox
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    //Load textures
    // Skybox de día
    std::vector<const GLchar*> faces;
    faces.push_back("SkyBox/day/right.png");
    faces.push_back("SkyBox/day/left.png");
    faces.push_back("SkyBox/day/top.png");
    faces.push_back("SkyBox/day/bottom.png");
    faces.push_back("SkyBox/day/back.png");
    faces.push_back("SkyBox/day/front.png");
    GLuint cubemapDay = TextureLoading::LoadCubemap(faces);

    // Skybox de noche
    std::vector<const GLchar*> facesNight;
    facesNight.push_back("SkyBox/night/right.png");
    facesNight.push_back("SkyBox/night/left.png");
    facesNight.push_back("SkyBox/night/top.png");
    facesNight.push_back("SkyBox/night/bottom.png");
    facesNight.push_back("SkyBox/night/back.png");
    facesNight.push_back("SkyBox/night/front.png");
    GLuint cubemapNight = TextureLoading::LoadCubemap(facesNight);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    
    // Impresion en pantalla para obtener la duración de la animación y que el modelo se mantenga en el último frame
    // std::cout << "Duracion: " << Robot.GetAnimationDuration() << std::endl;
    // std::cout << "Walking duration: " << Walking1.GetAnimationDuration() << std::endl;
    // std::cout << "Talking duration: " << Talking.GetAnimationDuration() << std::endl;
       std::cout << "Woman duration: " << Woman.GetAnimationDuration() << std::endl;

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Factor de intensidad: 0.0 = luces apagadas, 1.0 = luces encendidas
        float intensity = lightsOn ? 1.0f : 0.0f;
        // Ambient: alto en día (luces apagadas), bajo en noche (luces encendidas)
        float ambientStr = lightsOn ? 0.05f : 0.5f;

        lightingShader.Use();

        glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glm::mat4 view = camera.GetViewMatrix();
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Luz direccional — modo de día simula luz solar que entra como en la vida real de forma diagonal
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.5f, -1.0f, -0.3f); // En diagonal, como sol desde afuera
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), ambientStr, ambientStr * 0.95f, ambientStr * 0.85f);  // Usando un tono cálido amarillento del día
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f * (1.0f - intensity), 0.55f * (1.0f - intensity), 0.4f * (1.0f - intensity));  // Difusa solar solo en día
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.2f * (1.0f - intensity), 0.2f * (1.0f - intensity), 0.1f * (1.0f - intensity));  // Algo de brillo solar

        // Insersión de las 3 point lights — lámparas de foco normales
        // Luz blanco-cálido: se apagan o prenden con P
        for (int i = 0; i < 3; i++) {
            std::string base = "pointLights[" + std::to_string(i) + "].";
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "position").c_str()), pointLightPositions[i].x, pointLightPositions[i].y, pointLightPositions[i].z);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "ambient").c_str()), 0.05f * intensity, 0.05f * intensity, 0.04f * intensity);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "diffuse").c_str()), 1.0f * intensity, 0.95f * intensity, 0.8f * intensity);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "specular").c_str()), 1.0f * intensity, 1.0f * intensity, 1.0f * intensity);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "linear").c_str()), 0.09f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "quadratic").c_str()), 0.032f);
        }

        // Inserción de las 11 SPOT LIGHTS — tubos de luz lez, todos apuntan hacia abajo desde su posición
        // Luz blanco-fría, ligeramente azulada: se apagan o prenden con P
        for (int i = 0; i < 11; i++) {
            std::string base = "spotLights[" + std::to_string(i) + "].";
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "position").c_str()),spotLightPositions[i].x, spotLightPositions[i].y, spotLightPositions[i].z); // Dirección hacia abajo (0, -1, 0)
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "direction").c_str()), 0.0f, -1.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "ambient").c_str()), 0.0f, 0.0f, 0.0f); // Blanco-frío como un tubo led real
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "diffuse").c_str()), 0.9f * intensity, 0.9f * intensity, 1.0f * intensity);
            glUniform3f(glGetUniformLocation(lightingShader.Program, (base + "specular").c_str()), 0.5f * intensity, 0.5f * intensity, 0.5f * intensity);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "constant").c_str()), 1.0f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "linear").c_str()), 0.07f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "quadratic").c_str()), 0.017f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "cutOff").c_str()), glm::cos(glm::radians(80.0f)));
            glUniform1f(glGetUniformLocation(lightingShader.Program, (base + "outerCutOff").c_str()), glm::cos(glm::radians(90.0f)));
        }

        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the loaded model
        // Cajas
        glm::mat4 model(1);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        CashierArea.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-7.5f, 0.0f, -13.5f));
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand1.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.25f, 0.0f, -17.5f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.245f, 0.35f, 0.175f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand2.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -8.25f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.315f, 0.4f, 0.21f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand3.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -1.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.36f, 0.45f, 0.22f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand4.Draw(lightingShader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.1f, 0.0f, 6.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.4f, 0.21f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand5.Draw(lightingShader);

        skinnedShader.Use();
        GLint bonesLoc = glGetUniformLocation(skinnedShader.Program, "bones");
        std::vector<glm::mat4> identity(100, glm::mat4(1.0f));

        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        if (playWalking)
        {
            walkingAnimTime += deltaTime;

            if (walkingAnimTime >= (float)walkingDuration)
            {
                walkingAnimTime = (float)walkingDuration - 0.001f;
                playWalking = false;
            }
        }
        Walking1.UpdateAnimation(walkingAnimTime);
        std::vector<glm::mat4> walkingBones;
        Walking1.GetBoneMatrices(walkingBones, 100);
        if (bonesLoc >= 0 && !walkingBones.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)walkingBones.size(),
                GL_FALSE,
                &walkingBones[0][0][0]
            );
        }
        glm::mat4 modelWalk(1.0f);
        modelWalk = glm::scale(modelWalk, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelWalk)
        );
        Walking1.Draw(skinnedShader);


        // Walking 2
        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        Walking2.UpdateAnimation(walkingAnimTime);
        std::vector<glm::mat4> walkingBones2;
        Walking2.GetBoneMatrices(walkingBones2, 100);
        if (bonesLoc >= 0 && !walkingBones2.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)walkingBones2.size(),
                GL_FALSE,
                &walkingBones2[0][0][0]
            );
        }

        glm::mat4 modelWalk2(1.0f);
        modelWalk2 = glm::scale(modelWalk2, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelWalk2)
        );
        Walking2.Draw(skinnedShader);

        // Walking 3
        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        Walking3.UpdateAnimation(walkingAnimTime);
        std::vector<glm::mat4> walkingBones3;
        Walking3.GetBoneMatrices(walkingBones3, 100);
        if (bonesLoc >= 0 && !walkingBones3.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)walkingBones3.size(),
                GL_FALSE,
                &walkingBones3[0][0][0]
            );
        }

        glm::mat4 modelWalk3(1.0f);
        modelWalk3 = glm::scale(modelWalk3, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelWalk3)
        );
        Walking3.Draw(skinnedShader);

        // Talking 1
        if (playTalking)
        {
            talkingAnimTime += deltaTime;

            if (talkingAnimTime >= (float)talkingDuration)
            {
                talkingAnimTime = (float)talkingDuration - 0.001f;
                playTalking = false;
            }
        }
        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        Talking.UpdateAnimation(talkingAnimTime);
        std::vector<glm::mat4> talkingBones1;
        Talking.GetBoneMatrices(talkingBones1, 100);
        if (bonesLoc >= 0 && !talkingBones1.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)talkingBones1.size(),
                GL_FALSE,
                &talkingBones1[0][0][0]
            );
        }
        glm::mat4 modelTalk1(1.0f);
        modelTalk1 = glm::scale(modelTalk1, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelTalk1)
        );
        Talking.Draw(skinnedShader);

        // Talking 2
        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        Talking2.UpdateAnimation(talkingAnimTime);
        std::vector<glm::mat4> talkingBones2;
        Talking2.GetBoneMatrices(talkingBones2, 100);
        if (bonesLoc >= 0 && !talkingBones2.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)talkingBones2.size(),
                GL_FALSE,
                &talkingBones2[0][0][0]
            );
        }
        glm::mat4 modelTalk2(1.0f);
        modelTalk2 = glm::scale(modelTalk2, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelTalk2)
        );

        Talking2.Draw(skinnedShader);

        // Talking 3
        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        Talking3.UpdateAnimation(talkingAnimTime);
        std::vector<glm::mat4> talkingBones3;
        Talking3.GetBoneMatrices(talkingBones3, 100);
        if (bonesLoc >= 0 && !talkingBones3.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)talkingBones3.size(),
                GL_FALSE,
                &talkingBones3[0][0][0]
            );
        }
        glm::mat4 modelTalk3(1.0f);
        modelTalk3 = glm::scale(modelTalk3, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelTalk3)
        );

        Talking3.Draw(skinnedShader);


        // Woman
        if (playWoman)
        {
            womanAnimTime += deltaTime;

            if (womanAnimTime >= (float)womanDuration)
            {
                womanAnimTime = (float)womanDuration - 0.001f;
                playWoman = false;
            }
        }

        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);
        Woman.UpdateAnimation(womanAnimTime);
        std::vector<glm::mat4> womanBones;
        Woman.GetBoneMatrices(womanBones, 100);
        if (bonesLoc >= 0 && !womanBones.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)womanBones.size(),
                GL_FALSE,
                &womanBones[0][0][0]
            );
        }

        glm::mat4 modelWoman(1.0f);
        modelWoman = glm::scale(modelWoman, glm::vec3(0.01f));
        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelWoman)
        );
        Woman.Draw(skinnedShader);

        if (playAnimationRobot)
        {
            robotAnimTime += deltaTime;
            if (robotAnimTime >= (float)animDuration)
            {
                robotAnimTime = (float)animDuration - 0.001f;
                playAnimationRobot = false;
            }
        }

        Robot.UpdateAnimation(robotAnimTime);

        // Obtener matrices de huesos
        std::vector<glm::mat4> bones;
        Robot.GetBoneMatrices(bones, 100);

        // Mandar al shader
        skinnedShader.Use();

        if (bonesLoc >= 0 && !bones.empty())
        {
            glUniformMatrix4fv(bonesLoc, (GLsizei)bones.size(), GL_FALSE, &bones[0][0][0]);
        }

        // Matrices de cámara
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "view"), 1, GL_FALSE,glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Posicionar y dibujar
        glm::mat4 modelAnim(1.0f);
        modelAnim = glm::scale(modelAnim, glm::vec3(0.01f));
        glUniformMatrix4fv(glGetUniformLocation(skinnedShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAnim));

        // Dibujar siempre
        Robot.Draw(skinnedShader);
        

        // Cubos indicadores de luces
		// Comentados para que no se muestren al final, pues solo los usamos para verificar posiciones de luces durante el desarrollo
        // shader.Use();
        // GLint lModelLoc = glGetUniformLocation(shader.Program, "model");
        // GLint lViewLoc = glGetUniformLocation(shader.Program, "view");
        // GLint lProjLoc = glGetUniformLocation(shader.Program, "projection");
        // glUniformMatrix4fv(lViewLoc, 1, GL_FALSE, glm::value_ptr(view));
        // glUniformMatrix4fv(lProjLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // glBindVertexArray(VAO);
        // Cubos de lamparas
        // glUniform3f(glGetUniformLocation(shader.Program, "lampColor"), 1.0f, 1.0f, 0.0f);
        // for (int i = 0; i < 3; i++) {
        //     model = glm::mat4(1);
        //     model = glm::translate(model, pointLightPositions[i]);
        //     model = glm::scale(model, glm::vec3(0.2f));
        //     glUniformMatrix4fv(lModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }
        // Cubos de tubos led
        // glUniform3f(glGetUniformLocation(shader.Program, "lampColor"), 0.0f, 0.5f, 1.0f);
        // for (int i = 0; i < 11; i++) {
        //     model = glm::mat4(1);
        //     model = glm::translate(model, spotLightPositions[i]);
        //     model = glm::scale(model, glm::vec3(0.3f, 0.05f, 0.3f));
        //     glUniformMatrix4fv(lModelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //     glDrawArrays(GL_TRIANGLES, 0, 36);
        // }

        glBindVertexArray(0);

        //Draw SkyBox
        glDepthFunc(GL_LEQUAL);
        skyboxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        GLuint currentSkybox = lightsOn ? cubemapNight : cubemapDay; // Se alterna el skybox segun sea modo día o modo noche
        glBindTexture(GL_TEXTURE_CUBE_MAP, currentSkybox);
        glUniform1i(glGetUniformLocation(skyboxshader.Program, "skybox"), 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;

            // Funcionamiento de modo día o modo noche con la tecla P
            if (key == GLFW_KEY_P)
            {
                lightsOn = !lightsOn;
            }
            // Activar/desactivar animación del robot con G 
            if (key == GLFW_KEY_G)
            {
                if (robotAnimTime >= (float)animDuration - 0.01f)
                {
                    // Terminó: reiniciar
                    robotAnimTime = 0.0f;
                    playAnimationRobot = true;
                }
                else
                {
                    // En curso: pausar/reanudar
                    playAnimationRobot = !playAnimationRobot;
                }
            }

            // Activar/desactivar animación walking con H
            if (key == GLFW_KEY_H)
            {
                if (walkingAnimTime >= (float)walkingDuration - 0.01f)
                {
                    // Terminó: reiniciar
                    walkingAnimTime = 0.0f;
                    playWalking = true;
                }
                else
                {
                    // En curso: pausar/reanudar
                    playWalking = !playWalking;
                }
            }

            // Activar/desactivar animación talking con O
            if (key == GLFW_KEY_O)
            {
                if (talkingAnimTime >= (float)talkingDuration - 0.01f)
                {
                    // Terminó: reiniciar
                    talkingAnimTime = 0.0f;
                    playTalking = true;
                }
                else
                {
                    // En curso: pausar/reanudar
                    playTalking = !playTalking;
                }
            }

            // Activar/desactivar animación Woman con I
            if (key == GLFW_KEY_I)
            {
                if (womanAnimTime >= (float)womanDuration - 0.01f)
                {
                    // Terminó: reiniciar
                    womanAnimTime = 0.0f;
                    playWoman = true;
                }
                else
                {
                    // En curso: pausar/reanudar
                    playWoman = !playWoman;
                }
            }
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}