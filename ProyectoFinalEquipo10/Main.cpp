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
#include <vector>

#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
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

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void ActualizarHoverStand(GLFWwindow* window);
glm::vec3 ObtenerCentroStand(int numeroStand);
bool StandDisponible(int numeroStand);
// Camera
Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

glm::mat4 projection;

//Variables para que el usuario tenga visivibilidad del área en donde se arman los stands
GLuint circuloVAO, circuloVBO;

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

// Control de animación Karaoke
bool playKaraoke = false;

// Tiempo acumulado de la animación Karaoke
float karaokeAnimTime = 0.0f;

// Duración aproximada. Luego la ajustamos con GetAnimationDuration()
const double karaokeDuration = 8.3;
bool mostrarPantallaKaraoke = false;

bool musicaKaraokeAbierta = false;
bool musicaKaraokePausada = false;

const char* rutaMusicaKaraoke = "Sounds\\Karaoke.mp3";

// Control de animación de la persona lanzando
bool playPruebaAnim = false;

//Mostrar objetos de lanzamiento 
bool mostrarLanzamiento = false;

// Tiempo acumulado de la animación de la persona
float pruebaAnimTime = 0.0f;

// Blender: frame 1 al 180 a 24 FPS = 7.5 segundos
const float pruebaAnimFPS = 24.0f;
const float pruebaAnimDuration = 180.0f / pruebaAnimFPS;

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
// ANIMACION GENERAL DE STANDS


#define STAND_FRAMES 5

struct StandPose
{
    float baseY;

    float columnasY;
    float colIzqRotZ;
    float colDerRotZ;

    float mostradorY;
    float mostradorIzqX;
    float mostradorDerX;
    float mostradorRotZ;

    float panelY;
    float panelZ;
    float panelRotZ;

    float techoY;
};

struct StandAnimacion
{
    StandPose keyFrame[STAND_FRAMES];

    StandPose actual;
    StandPose incremento;

    int playIndex = 0;
    int currSteps = 0;
    int maxSteps = 120;

    bool play = false;
    bool visible = false;
};

// Variables para animaciones de stands 
StandAnimacion stand1Anim;
StandAnimacion stand2Anim;
StandAnimacion stand3Anim;
StandAnimacion stand4Anim;
StandAnimacion stand5Anim;

//Configuracion de Clics para activacion de cada stand 
const glm::vec3 STAND1_CENTER = glm::vec3(-7.5f, 0.0f, -13.5f);
const glm::vec3 STAND2_CENTER = glm::vec3(1.25f, 0.0f, -17.5f);
const glm::vec3 STAND3_CENTER = glm::vec3(-3.0f, 0.0f, -8.25f);
const glm::vec3 STAND4_CENTER = glm::vec3(-3.0f, 0.0f, -1.0f);
const glm::vec3 STAND5_CENTER = glm::vec3(-3.1f, 0.0f, 6.5f);

int standEnHover = 0;

const float RADIO_DETECCION_STAND = 3.5f;
const float RADIO_CIRCULO_STAND = 1.2f;
const int SEGMENTOS_CIRCULO = 64;

//Prototipos de funciones de animación de los stands
void InicializarKeyFramesStand(StandAnimacion& stand);
void ResetearStand(StandAnimacion& stand);
void InterpolarStand(StandAnimacion& stand);
void IniciarAnimacionStand(StandAnimacion& stand);
void ActualizarAnimacionStand(StandAnimacion& stand);
void ConfigurarStand2(StandAnimacion& stand);
void ConfigurarStand3(StandAnimacion& stand);
void ConfigurarStand4(StandAnimacion& stand);
void ConfigurarStand5(StandAnimacion& stand);

bool ObtenerPuntoClickEnPiso(GLFWwindow* window, glm::vec3& punto);
bool PuntoCercaDeStand(glm::vec3 punto, glm::vec3 centroStand, float radio);

float Clamp01(float t)
{
    if (t < 0.0f) return 0.0f;
    if (t > 1.0f) return 1.0f;
    return t;
}

float EaseInOut(float t)
{
    t = Clamp01(t);
    return t * t * (3.0f - 2.0f * t);
}

glm::vec3 LerpVec3(glm::vec3 a, glm::vec3 b, float t)
{
    t = Clamp01(t);
    return a + (b - a) * t;
}

glm::vec3 Bezier3(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2, float t)
{
    t = Clamp01(t);

    float u = 1.0f - t;

    return (u * u * p0) +
        (2.0f * u * t * p1) +
        (t * t * p2);
}
void MostrarErrorMCI(MCIERROR error)
{
    if (error == 0)
        return;

    char textoError[256];

    if (mciGetErrorStringA(error, textoError, sizeof(textoError)))
    {
        std::cout << "MCI ERROR: " << textoError << std::endl;
    }
    else
    {
        std::cout << "MCI ERROR desconocido: " << error << std::endl;
    }
}

void AbrirMusicaKaraoke()
{
    if (musicaKaraokeAbierta)
        return;

    std::string comando = "open \"" + std::string(rutaMusicaKaraoke) + "\" alias karaokeSong";

    MCIERROR error = mciSendStringA(comando.c_str(), NULL, 0, NULL);

    if (error != 0)
    {
        std::cout << "No se pudo abrir la musica: " << rutaMusicaKaraoke << std::endl;
        MostrarErrorMCI(error);
        return;
    }

    musicaKaraokeAbierta = true;
    musicaKaraokePausada = false;
}

void ReproducirMusicaKaraokeDesdeInicio()
{
    AbrirMusicaKaraoke();

    if (!musicaKaraokeAbierta)
        return;

    MCIERROR error;

    error = mciSendStringA("seek karaokeSong to start", NULL, 0, NULL);
    MostrarErrorMCI(error);

    error = mciSendStringA("play karaokeSong", NULL, 0, NULL);
    MostrarErrorMCI(error);

    musicaKaraokePausada = false;
}

void PausarMusicaKaraoke()
{
    if (!musicaKaraokeAbierta)
        return;

    MCIERROR error = mciSendStringA("pause karaokeSong", NULL, 0, NULL);
    MostrarErrorMCI(error);

    musicaKaraokePausada = true;
}

void ReanudarMusicaKaraoke()
{
    if (!musicaKaraokeAbierta)
        return;

    MCIERROR error = mciSendStringA("play karaokeSong", NULL, 0, NULL);
    MostrarErrorMCI(error);

    musicaKaraokePausada = false;
}

void DetenerMusicaKaraoke()
{
    if (!musicaKaraokeAbierta)
        return;

    MCIERROR error;

    error = mciSendStringA("stop karaokeSong", NULL, 0, NULL);
    MostrarErrorMCI(error);

    error = mciSendStringA("seek karaokeSong to start", NULL, 0, NULL);
    MostrarErrorMCI(error);

    musicaKaraokePausada = false;
}

void CerrarMusicaKaraoke()
{
    if (!musicaKaraokeAbierta)
        return;

    mciSendStringA("stop karaokeSong", NULL, 0, NULL);
    mciSendStringA("close karaokeSong", NULL, 0, NULL);

    musicaKaraokeAbierta = false;
    musicaKaraokePausada = false;
}

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Equipo #10", nullptr, nullptr);
    
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
    glfwSetMouseButtonCallback(window, MouseButtonCallback);

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
    Shader colorShader("Shader/color.vs", "Shader/color.frag");
    Shader personaShader("Shader/persona_skin.vs", "Shader/persona_tex.frag");

    // Load models
    Model CashierArea((char*)"Models/CashierArea/CashierArea.obj");



    //STAND 1 
    //Model Stand1((char*)"Models/Stand1/Stand1.obj");
    Model BaseS1((char*)"Models/Stand1/BaseS1.obj");
    Model ColumIzqS1((char*)"Models/Stand1/ColumIzqS1.obj");
    Model ColumDerS1((char*)"Models/Stand1/ColumDerS1.obj");
    Model MostradorDerS1((char*)"Models/Stand1/MostradorDerS1.obj");
    Model MostradorIzqS1((char*)"Models/Stand1/MostradorIzqS1.obj");
    Model PanelTrasero((char*)"Models/Stand1/PanelTrasero.obj");
    Model TechoS1((char*)"Models/Stand1/TechoS1.obj");

    //STAND 2
    //Model Stand2((char*)"Models/Stand2/Stand2.obj");
    Model BaseS2((char*)"Models/Stand2/BaseS2.obj");
    Model ParedDerS2((char*)"Models/Stand2/ParedDerS2.obj");
    Model ParedTraseraS2((char*)"Models/Stand2/ParedTraseraS2.obj");
    Model MostradorS2((char*)"Models/Stand2/MostradorS2.obj");
    Model TechoS2((char*)"Models/Stand2/TechoS2.obj");

    //STAND 3
    //Model Stand3((char*)"Models/Stand3/Stand3.obj");
    Model BaseS3((char*)"Models/Stand3/BaseS3.obj");
    Model ColumFrontS3((char*)"Models/Stand3/ColumFrontS3.obj");
    Model ParedDerS3((char*)"Models/Stand3/ParedDerS3.obj");
    Model ParedTraseraS3((char*)"Models/Stand3/ParedTraseraS3.obj");
    Model AnuncioS3((char*)"Models/Stand3/AnuncioS3.obj");
    Model MostradorS3((char*)"Models/Stand3/MostradorS3.obj");
    Model TechoS3((char*)"Models/Stand3/TechoS3.obj");
    //STAND 4
    //Model Stand4((char*)"Models/Stand4/Stand4.obj");
    Model BaseS4((char*)"Models/Stand4/BaseS4.obj");
    Model ColumnaS4((char*)"Models/Stand4/ColumnaS4.obj");
    Model ParedPantallaS4((char*)"Models/Stand4/ParedPantallaS4.obj");
    Model ParedTechoS4((char*)"Models/Stand4/ParedTechoS4.obj");
    Model MesaS4((char*)"Models/Stand4/MesaS4.obj");
    Model Silla1S4((char*)"Models/Stand4/Silla1S4.obj");
    Model Silla2S4((char*)"Models/Stand4/Silla2S4.obj");
    Model TechoS4((char*)"Models/Stand4/TechoS4.obj");

    //STAND 5
    //Model Stand5((char*)"Models/Stand5/Stand5.obj"); 
    Model BaseS5((char*)"Models/Stand5/BaseS5.obj");
    Model ParedLateralS5((char*)"Models/Stand5/ParedLateralS5.obj");
    Model ParedTraseraS5((char*)"Models/Stand5/ParedTraseraS5.obj");
    Model ParedTechoS5((char*)"Models/Stand5/ParedTechoS5.obj");
    Model MostradorS5((char*)"Models/Stand5/MostradorS5.obj");
    Model MesaS5((char*)"Models/Stand5/MesaS5.obj");
    Model BancoS5((char*)"Models/Stand5/BancoS5.obj");
    Model Banco2S5((char*)"Models/Stand5/Banco2S5.obj");


    Model Robot((char*)"Models/Robot/Robot.fbx");
    Model Walking1((char*)"Models/Walking/Walking1.fbx");
    Model Walking2((char*)"Models/Walking/Walking2.fbx");
    Model Walking3((char*)"Models/Walking/Walking3.fbx");
    Model Talking((char*)"Models/Talking/Exhibitor.fbx");
    Model Talking2((char*)"Models/Talking/Reader.fbx");
    Model Talking3((char*)"Models/Talking/Clipboard.fbx");
    Model Woman((char*)"Models/Woman/Woman.fbx");
    Model PruebaAnim((char*)"Models/PruebaAnim/PruebaAnim.fbx");
    Model Karaoke((char*)"Models/Karaoke/Karaoke.fbx");

    Model Aro((char*)"Models/PruebaAnim/Aro.obj");
    Model Pelota((char*)"Models/PruebaAnim/Pelota.obj");
    Model PantallaKaraoke1((char*)"Models/KaraokeScreen/PantallaKaraoke1.obj");
    Model PantallaKaraoke2((char*)"Models/KaraokeScreen/PantallaKaraoke2.obj");
    Model PantallaKaraoke3((char*)"Models/KaraokeScreen/PantallaKaraoke3.obj");
    Model PantallaKaraoke4((char*)"Models/KaraokeScreen/PantallaKaraoke4.obj");
	
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

    //glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
    // Impresion en pantalla para obtener la duración de la animación y que el modelo se mantenga en el último frame
    // std::cout << "Duracion: " << Robot.GetAnimationDuration() << std::endl;
    // std::cout << "Walking duration: " << Walking1.GetAnimationDuration() << std::endl;
    // std::cout << "Talking duration: " << Talking.GetAnimationDuration() << std::endl;
      // std::cout << "Woman duration: " << Woman.GetAnimationDuration() << std::endl;
    std::cout << "Karaoke duration: " << Karaoke.GetAnimationDuration() << std::endl;

    InicializarKeyFramesStand(stand1Anim);
    InicializarKeyFramesStand(stand2Anim);
    InicializarKeyFramesStand(stand3Anim);
    InicializarKeyFramesStand(stand4Anim);
    InicializarKeyFramesStand(stand5Anim);

    ConfigurarStand2(stand2Anim);
    ConfigurarStand3(stand3Anim);
    ConfigurarStand4(stand4Anim);
    ConfigurarStand5(stand5Anim);

    ResetearStand(stand1Anim);
    ResetearStand(stand2Anim);
    ResetearStand(stand3Anim);
    ResetearStand(stand4Anim);
    ResetearStand(stand5Anim);

    std::vector<glm::vec3> verticesCirculo; //Circulo que indica los stands

    // Centro del círculo, necesario para poder rellenarlo
    verticesCirculo.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

    // Borde del círculo
    for (int i = 0; i <= SEGMENTOS_CIRCULO; i++)
    {
        float angulo = 2.0f * 3.14159265f * i / SEGMENTOS_CIRCULO;

        float x = cos(angulo) * RADIO_CIRCULO_STAND;
        float z = sin(angulo) * RADIO_CIRCULO_STAND;

        verticesCirculo.push_back(glm::vec3(x, 0.0f, z));
    }

    glGenVertexArrays(1, &circuloVAO);
    glGenBuffers(1, &circuloVBO);

    glBindVertexArray(circuloVAO);

    glBindBuffer(GL_ARRAY_BUFFER, circuloVBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        verticesCirculo.size() * sizeof(glm::vec3),
        verticesCirculo.data(),
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

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

        ActualizarHoverStand(window);

        ActualizarAnimacionStand(stand1Anim);
        ActualizarAnimacionStand(stand2Anim);
        ActualizarAnimacionStand(stand3Anim);
        ActualizarAnimacionStand(stand4Anim);
        ActualizarAnimacionStand(stand5Anim);

        // Actualizar tiempo de karaoke
        if (playKaraoke)
        {
            karaokeAnimTime += deltaTime;

            if (karaokeAnimTime >= (float)karaokeDuration)
            {
                karaokeAnimTime = (float)karaokeDuration - 0.001f;
                playKaraoke = false;
                mostrarPantallaKaraoke = false;

                DetenerMusicaKaraoke();
            }
        }

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


        //STAND1

      /*  model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-7.5f, 0.0f, -13.5f));
        model = glm::rotate(model, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.8f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand1.Draw(lightingShader);*/

        if (stand1Anim.visible)
        {
            glm::mat4 stand1Base = glm::mat4(1.0f);
            stand1Base = glm::translate(stand1Base, glm::vec3(-7.5f, 0.0f, -13.5f));
            stand1Base = glm::rotate(stand1Base, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            stand1Base = glm::scale(stand1Base, glm::vec3(0.6f, 0.8f, 0.6f));

            bool standTerminado = stand1Anim.visible && !stand1Anim.play;

            bool mostrarBase = stand1Anim.visible;

            bool mostrarColumnas = stand1Anim.visible && (standTerminado || stand1Anim.playIndex >= 1);

            bool mostrarMostradoresYPanel = stand1Anim.visible && (standTerminado || stand1Anim.playIndex >= 2);

            bool mostrarTecho = stand1Anim.visible && (standTerminado || stand1Anim.playIndex >= 3);

            // BASE
            if (mostrarBase)
            {
                model = stand1Base;
                model = glm::translate(model, glm::vec3(0.0f, stand1Anim.actual.baseY, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                BaseS1.Draw(lightingShader);
            }


            // COLUMNAS
            if (mostrarColumnas)
            {
                model = stand1Base;
                model = glm::translate(model, glm::vec3(0.0f, stand1Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand1Anim.actual.colDerRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ColumDerS1.Draw(lightingShader);

                model = stand1Base;
                model = glm::translate(model, glm::vec3(0.0f, stand1Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand1Anim.actual.colIzqRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ColumIzqS1.Draw(lightingShader);
            }


            // MOSTRADORES Y PANEL TRASERO
            if (mostrarMostradoresYPanel)
            {
                model = stand1Base;
                model = glm::translate(model, glm::vec3(stand1Anim.actual.mostradorDerX, stand1Anim.actual.mostradorY, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MostradorDerS1.Draw(lightingShader);


                model = stand1Base;
                model = glm::translate(model, glm::vec3(stand1Anim.actual.mostradorIzqX, stand1Anim.actual.mostradorY, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MostradorIzqS1.Draw(lightingShader);


                model = stand1Base;
                model = glm::translate(model, glm::vec3(0.0f, stand1Anim.actual.panelY, stand1Anim.actual.panelZ));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                PanelTrasero.Draw(lightingShader);
            }

            // TECHO
            if (mostrarTecho)
            {
                model = stand1Base;
                model = glm::translate(model, glm::vec3(0.0f, stand1Anim.actual.techoY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                TechoS1.Draw(lightingShader);
            }
        }

        //STAND 2 
     /*model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(1.25f, 0.0f, -17.5f));
        model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.245f, 0.35f, 0.175f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand2.Draw(lightingShader);*/

        if (stand2Anim.visible)
        {
            glm::mat4 stand2Base = glm::mat4(1.0f);
            stand2Base = glm::translate(stand2Base, glm::vec3(1.25f, 0.0f, -17.5f));
            stand2Base = glm::rotate(stand2Base, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            stand2Base = glm::scale(stand2Base, glm::vec3(0.245f, 0.35f, 0.175f));

            bool standTerminado = stand2Anim.visible && !stand2Anim.play;

            bool mostrarBase = stand2Anim.visible;

            bool mostrarParedes = stand2Anim.visible && (standTerminado || stand2Anim.playIndex >= 1);

            bool mostrarMostrador = stand2Anim.visible && (standTerminado || stand2Anim.playIndex >= 2);

            bool mostrarTecho = stand2Anim.visible && (standTerminado || stand2Anim.playIndex >= 3);

            // BASE
            if (mostrarBase)
            {
                model = stand2Base;
                model = glm::translate(model, glm::vec3(0.0f, stand2Anim.actual.baseY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                BaseS2.Draw(lightingShader);
            }


            // PAREDES
            if (mostrarParedes)
            {
                // PARED DERECHA
                model = stand2Base;
                model = glm::translate(model, glm::vec3(0.0f, stand2Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand2Anim.actual.colDerRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedDerS2.Draw(lightingShader);


                // PARED TRASERA
                model = stand2Base;
                model = glm::translate(model, glm::vec3(0.0f, stand2Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand2Anim.actual.colIzqRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedTraseraS2.Draw(lightingShader );
            }

            // MOSTRADOR
            if (mostrarMostrador)
            {
                model = stand2Base;
                model = glm::translate(model, glm::vec3(stand2Anim.actual.mostradorIzqX, stand2Anim.actual.mostradorY, 0.0f));
                model = glm::rotate(model, glm::radians(stand2Anim.actual.mostradorRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MostradorS2.Draw(lightingShader);
            }


            // TECHO
            if (mostrarTecho)
            {
                model = stand2Base;
                model = glm::translate(model, glm::vec3(0.0f, stand2Anim.actual.techoY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                TechoS2.Draw(lightingShader);
            }
        }


        //STAND 3
        /* model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -8.25f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.315f, 0.4f, 0.21f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand3.Draw(lightingShader);*/

        if (stand3Anim.visible)
        {
            glm::mat4 stand3Base = glm::mat4(1.0f);
            stand3Base = glm::translate(stand3Base, glm::vec3(-3.0f, 0.0f, -8.25f));
            stand3Base = glm::rotate(stand3Base, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            stand3Base = glm::scale(stand3Base, glm::vec3(0.315f, 0.4f, 0.21f));

            bool standTerminado = stand3Anim.visible && !stand3Anim.play;

            bool mostrarBase = stand3Anim.visible;

            bool mostrarEstructura = stand3Anim.visible && (standTerminado || stand3Anim.playIndex >= 1);

            bool mostrarMostrador = stand3Anim.visible && (standTerminado || stand3Anim.playIndex >= 2);

            bool mostrarTechoYAnuncio = stand3Anim.visible && (standTerminado || stand3Anim.playIndex >= 3);


            // BASE
            if (mostrarBase)
            {
                model = stand3Base;
                model = glm::translate(model, glm::vec3(0.0f, stand3Anim.actual.baseY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                BaseS3.Draw(lightingShader);
            }


            // COLUMNA FRONTAL, PARED DERECHA Y PARED TRASERA
            if (mostrarEstructura)
            {
                // COLUMNA FRONTAL
                model = stand3Base;
                model = glm::translate(model, glm::vec3(0.0f, stand3Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand3Anim.actual.colDerRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ColumFrontS3.Draw(lightingShader);


                // PARED DERECHA
                model = stand3Base;
                model = glm::translate(model, glm::vec3(0.0f, stand3Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand3Anim.actual.colIzqRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedDerS3.Draw(lightingShader);


                // PARED TRASERA
                model = stand3Base;
                model = glm::translate(model, glm::vec3(0.0f, stand3Anim.actual.panelY, stand3Anim.actual.panelZ));
                model = glm::rotate(model, glm::radians(stand3Anim.actual.panelRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedTraseraS3.Draw(lightingShader);
            }


            // MOSTRADOR
            if (mostrarMostrador)
            {
                model = stand3Base;
                model = glm::translate(model, glm::vec3(stand3Anim.actual.mostradorIzqX, stand3Anim.actual.mostradorY, 0.0f));
                model = glm::rotate(model, glm::radians(stand3Anim.actual.mostradorRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MostradorS3.Draw(lightingShader);
            }


            // TECHO Y ANUNCIO
            if (mostrarTechoYAnuncio)
            {
                model = stand3Base;
                model = glm::translate(model, glm::vec3(0.0f, stand3Anim.actual.techoY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                TechoS3.Draw(lightingShader);


                model = stand3Base;
                model = glm::translate(model, glm::vec3(0.0f, stand3Anim.actual.techoY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                AnuncioS3.Draw(lightingShader);
            }
        }


        //STAND 4
        /*model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.0f, 0.0f, -1.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.36f, 0.45f, 0.22f));

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand4.Draw(lightingShader);*/

        if (stand4Anim.visible)
        {
            glm::mat4 stand4Base = glm::mat4(1.0f);
            stand4Base = glm::translate(stand4Base, glm::vec3(-3.0f, 0.0f, -1.0f));
            stand4Base = glm::rotate(stand4Base, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            stand4Base = glm::scale(stand4Base, glm::vec3(0.36f, 0.45f, 0.22f));

            bool standTerminado = stand4Anim.visible && !stand4Anim.play;

            bool mostrarBase = stand4Anim.visible;

            bool mostrarEstructura = stand4Anim.visible && (standTerminado || stand4Anim.playIndex >= 1);

            bool mostrarMobiliario = stand4Anim.visible && (standTerminado || stand4Anim.playIndex >= 2);

            bool mostrarTecho = stand4Anim.visible && (standTerminado || stand4Anim.playIndex >= 3);

            // BASE
            if (mostrarBase)
            {
                model = stand4Base;
                model = glm::translate(model, glm::vec3(0.0f, stand4Anim.actual.baseY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                BaseS4.Draw(lightingShader);
            }


            // COLUMNA, PARED PANTALLA Y PARED TECHO
            if (mostrarEstructura)
            {
                // COLUMNA
                model = stand4Base;
                model = glm::translate(model, glm::vec3(0.0f, stand4Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand4Anim.actual.colDerRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ColumnaS4.Draw(lightingShader);


                // PARED PANTALLA
                model = stand4Base;
                model = glm::translate(model, glm::vec3(0.0f, stand4Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand4Anim.actual.colIzqRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedPantallaS4.Draw(lightingShader);


                // PARED TECHO
                model = stand4Base;
                model = glm::translate(model, glm::vec3(0.0f, stand4Anim.actual.panelY, stand4Anim.actual.panelZ));
                model = glm::rotate(model, glm::radians(stand4Anim.actual.panelRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedTechoS4.Draw(lightingShader);
            }


            // MESA / MOSTRADOR Y SILLAS
            if (mostrarMobiliario)
            {
                // MESA / MOSTRADOR
                model = stand4Base;
                model = glm::translate(model, glm::vec3(stand4Anim.actual.mostradorIzqX, stand4Anim.actual.mostradorY, 0.0f));
                model = glm::rotate(model, glm::radians(stand4Anim.actual.mostradorRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MesaS4.Draw(lightingShader);

                // SILLA 1
                model = stand4Base;
                model = glm::translate(model, glm::vec3(stand4Anim.actual.mostradorIzqX, stand4Anim.actual.mostradorY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                Silla1S4.Draw(lightingShader);

                // SILLA 2
                model = stand4Base;
                model = glm::translate(model, glm::vec3(stand4Anim.actual.mostradorIzqX, stand4Anim.actual.mostradorY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                Silla2S4.Draw(lightingShader);
            }

            // TECHO
            if (mostrarTecho)
            {
                model = stand4Base;
                model = glm::translate(model, glm::vec3(0.0f, stand4Anim.actual.techoY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                TechoS4.Draw(lightingShader);
            }
        }

        //STAND 5
        /*model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-3.1f, 0.0f, 6.5f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.4f, 0.21f));

        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Stand5.Draw(lightingShader);*/

        if (stand5Anim.visible)
        {
            glm::mat4 stand5Base = glm::mat4(1.0f);
            stand5Base = glm::translate(stand5Base, glm::vec3(-3.1f, 0.0f, 6.5f));
            stand5Base = glm::rotate(stand5Base, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            stand5Base = glm::scale(stand5Base, glm::vec3(0.35f, 0.4f, 0.21f));

            bool standTerminado = stand5Anim.visible && !stand5Anim.play;

            bool mostrarBase = stand5Anim.visible;

            bool mostrarEstructura = stand5Anim.visible && (standTerminado || stand5Anim.playIndex >= 1);

            bool mostrarMostrador = stand5Anim.visible && (standTerminado || stand5Anim.playIndex >= 2);

            bool mostrarMobiliario = stand5Anim.visible && (standTerminado || stand5Anim.playIndex >= 3);


            // BASE
            if (mostrarBase)
            {
                model = stand5Base;
                model = glm::translate(model, glm::vec3(0.0f, stand5Anim.actual.baseY, 0.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                BaseS5.Draw(lightingShader);
            }


            // PARED LATERAL, PARED TRASERA Y PARED TECHO
            if (mostrarEstructura)
            {
                // PARED LATERAL
                model = stand5Base;
                model = glm::translate(model, glm::vec3(0.0f, stand5Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand5Anim.actual.colDerRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedLateralS5.Draw(lightingShader);


                // PARED TRASERA
                model = stand5Base;
                model = glm::translate(model, glm::vec3(0.0f, stand5Anim.actual.columnasY, 0.0f));
                model = glm::rotate(model, glm::radians(stand5Anim.actual.colIzqRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedTraseraS5.Draw(lightingShader);


                // PARED TECHO
                model = stand5Base;
                model = glm::translate(model, glm::vec3(0.0f, stand5Anim.actual.panelY, stand5Anim.actual.panelZ));
                model = glm::rotate(model, glm::radians(stand5Anim.actual.panelRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                ParedTechoS5.Draw(lightingShader);
            }


            // MOSTRADOR
            if (mostrarMostrador)
            {
                model = stand5Base;
                model = glm::translate(model, glm::vec3(stand5Anim.actual.mostradorIzqX, stand5Anim.actual.mostradorY, 0.0f));
                model = glm::rotate(model, glm::radians(stand5Anim.actual.mostradorRotZ), glm::vec3(0.0f, 0.0f, 1.0f));

                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MostradorS5.Draw(lightingShader);
            }


            // MESA Y BANCOS
            if (mostrarMobiliario)
            {
                model = stand5Base;
                model = glm::translate(model, glm::vec3(stand5Anim.actual.mostradorIzqX, stand5Anim.actual.mostradorY, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                MesaS5.Draw(lightingShader);


                model = stand5Base;
                model = glm::translate(model, glm::vec3(stand5Anim.actual.mostradorIzqX, stand5Anim.actual.mostradorY, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                BancoS5.Draw(lightingShader);


                model = stand5Base;
                model = glm::translate(model, glm::vec3(stand5Anim.actual.mostradorIzqX, stand5Anim.actual.mostradorY, 0.0f));
                glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
                Banco2S5.Draw(lightingShader);
            }
        }
        // Pantalla cambiante tipo karaoke
        
        if (mostrarPantallaKaraoke)
        {
            shader.Use();

            glUniform1i(
                glGetUniformLocation(shader.Program, "texture_diffuse1"),
                0
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shader.Program, "view"),
                1,
                GL_FALSE,
                glm::value_ptr(view)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shader.Program, "projection"),
                1,
                GL_FALSE,
                glm::value_ptr(projection)
            );

            glm::mat4 modelPantallaKaraoke(1.0f);

            modelPantallaKaraoke = glm::translate(
                modelPantallaKaraoke,
                glm::vec3(-0.4f, 1.13f, -9.5f)
            );

            modelPantallaKaraoke = glm::rotate(
                modelPantallaKaraoke,
                glm::radians(90.0f),
                glm::vec3(1.0f, 0.0f, 0.0f)
            );

            modelPantallaKaraoke = glm::scale(
                modelPantallaKaraoke,
                glm::vec3(0.3f)
            );

            glUniformMatrix4fv(
                glGetUniformLocation(shader.Program, "model"),
                1,
                GL_FALSE,
                glm::value_ptr(modelPantallaKaraoke)
            );

            float tiempoPorPantalla = (float)karaokeDuration / 4.0f;
            int pantallaActual = (int)(karaokeAnimTime / tiempoPorPantalla);

            if (pantallaActual < 0)
                pantallaActual = 0;

            if (pantallaActual > 3)
                pantallaActual = 3;

            if (pantallaActual == 0)
                PantallaKaraoke1.Draw(shader);
            else if (pantallaActual == 1)
                PantallaKaraoke2.Draw(shader);
            else if (pantallaActual == 2)
                PantallaKaraoke3.Draw(shader);
            else
                PantallaKaraoke4.Draw(shader);
        }
   

        //Animación de Persona lanzando 
        personaShader.Use();

        glUniform1i(glGetUniformLocation(personaShader.Program, "texture_diffuse1"), 0);
        GLint personaBonesLoc = glGetUniformLocation(personaShader.Program, "bones");

        if (playPruebaAnim)
        {
            pruebaAnimTime += deltaTime;

            if (pruebaAnimTime >= pruebaAnimDuration)
            {
                pruebaAnimTime = pruebaAnimDuration - 0.001f;
                playPruebaAnim = false;
                mostrarLanzamiento = false;
            }
        }

        PruebaAnim.UpdateAnimation(pruebaAnimTime);

        std::vector<glm::mat4> pruebaBones;
        PruebaAnim.GetBoneMatrices(pruebaBones, 100);

        if (personaBonesLoc >= 0 && !pruebaBones.empty())
        {
            glUniformMatrix4fv(
                personaBonesLoc,
                (GLsizei)pruebaBones.size(),
                GL_FALSE,
                &pruebaBones[0][0][0]
            );
        }

        glUniformMatrix4fv(glGetUniformLocation(personaShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view)
        );

        glUniformMatrix4fv(glGetUniformLocation(personaShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)
        );

        glm::mat4 modelPruebaAnim(1.0f);

        modelPruebaAnim = glm::translate(modelPruebaAnim, glm::vec3(2.0f, 0.0f, 5.2f));

        modelPruebaAnim = glm::scale(modelPruebaAnim, glm::vec3(1.3f));

        glUniformMatrix4fv(glGetUniformLocation(personaShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPruebaAnim));

        PruebaAnim.Draw(personaShader);
        if (mostrarLanzamiento)
        {
            // Dibujo de Aro con red 


            shader.Use();

            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

            glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glm::mat4 modelAro(1.0f);

            glm::vec3 aroPos = glm::vec3(0.3f, 0.6f, -0.6f);
            float aroScale = 1.0f;
            float aroRotY = 0.0f;

            modelAro = glm::translate(modelAro, aroPos);

            modelAro = glm::rotate(
                modelAro,
                glm::radians(aroRotY),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            modelAro = glm::scale(modelAro, glm::vec3(aroScale));

            glUniformMatrix4fv(
                glGetUniformLocation(shader.Program, "model"),
                1,
                GL_FALSE,
                glm::value_ptr(modelAro)
            );

            Aro.Draw(shader);


            // PELOTA  ANIMADA

            // Frame actual equivalente al timeline de Blender
            float frameBasquet = 1.0f + pruebaAnimTime * pruebaAnimFPS;

            // Frames reales según tu animación de Blender
            const float framePelotaMostradorFin = 89.0f;
            const float framePelotaEnManoInicio = 90.0f;
            const float frameManoSubiendo = 98.0f;
            const float frameManoMedia = 105.0f;
            const float frameManoIntermedia = 112.0f;
            const float frameBrazoAtras = 120.0f;
            const float frameSueltaPelota = 122.0f;

            // Para pruebas lo dejamos lento.
            // Cuando ya se vea bien, puedes regresar frameEncimaAro a 127.0f.
            const float frameEncimaAro = 127.0f; //150.0f
            const float framePelotaPiso = 170.0f;

            // Posición inicial de la pelota sobre el mostrador

            glm::vec3 posMostrador = glm::vec3(-5.65f, 1.85f, -14.95f);


            // Centro real del aro

            glm::vec3 posCentroAro = glm::vec3(-6.75f, 2.3f, -12.0f);

            // Punto al que llega la pelota antes de caer por el aro
            glm::vec3 posEncimaAro = posCentroAro + glm::vec3(0.0f, 0.35f, 0.0f);

            // Punto debajo de la red
            glm::vec3 posDebajoRed = posCentroAro + glm::vec3(0.0f, -0.65f, 0.0f);

            // Punto final en el piso, debajo del aro
            glm::vec3 posPiso = glm::vec3(posCentroAro.x, 0.3f, posCentroAro.z);

            // Frame 90 
            glm::vec3 posManoInicio = posMostrador;

            glm::vec3 posManoSubiendo = glm::vec3(-5.50f, 2.05f, -15.05f);

            // Frame 105 aproximadamente, para que el movimiento hacia atrás sea más suave
            glm::vec3 posManoMedia = glm::vec3(-5.45f, 2.2f, -15.2f);

            glm::vec3 posManoIntermedia = glm::vec3(-5.5f, 2.35f, -15.55f);

            // Frame 120 mano hacia atrás para lanzar
            glm::vec3 posManoAtras = glm::vec3(-5.55f, 2.8f, -16.455f);

            // Frame 122 punto donde la mano suelta la pelota
            glm::vec3 posSuelta = glm::vec3(-5.58f, 3.05f, -15.75f);

            // Offset pequeño para que no atraviese la mano
            glm::vec3 offsetPelotaMano = glm::vec3(0.04f, 0.03f, 0.02f);
            glm::vec3 pelotaPos = posMostrador;

            if (frameBasquet <= framePelotaMostradorFin)
            {
                // Frame 1–89: pelota quieta sobre el mostrador
                pelotaPos = posMostrador;
            }
            else if (frameBasquet < frameManoSubiendo)
            {
                // Frame 90–98: la pelota pasa del mostrador a la mano
                float t = (frameBasquet - framePelotaEnManoInicio) / (frameManoSubiendo - framePelotaEnManoInicio);

                pelotaPos = LerpVec3(posManoInicio, posManoSubiendo, EaseInOut(t));
            }
            else if (frameBasquet < frameManoMedia)
            {
                // Frame 98–105: la pelota ya sube con la mano
                float t = (frameBasquet - frameManoSubiendo) / (frameManoMedia - frameManoSubiendo);

                pelotaPos = LerpVec3(posManoSubiendo, posManoMedia, EaseInOut(t));
            }
            else if (frameBasquet < frameManoIntermedia)
            {
                // Frame 105–113: transición hacia atrás más suave
                float t = (frameBasquet - frameManoMedia) / (frameManoIntermedia - frameManoMedia);

                pelotaPos = LerpVec3(posManoMedia, posManoIntermedia, EaseInOut(t));
            }
            else if (frameBasquet < frameBrazoAtras)
            {
                // Frame 113–120: mano hacia atrás
                float t = (frameBasquet - frameManoIntermedia) / (frameBrazoAtras - frameManoIntermedia);

                pelotaPos = LerpVec3(posManoIntermedia, posManoAtras, EaseInOut(t));
            }
            else if (frameBasquet < frameSueltaPelota)
            {
                // Frame 120–122: movimiento corto antes de soltar
                float t = (frameBasquet - frameBrazoAtras) / (frameSueltaPelota - frameBrazoAtras);

                pelotaPos = LerpVec3(posManoAtras, posSuelta, EaseInOut(t));
            }
            else if (frameBasquet < frameEncimaAro)
            {
                // Frame 122–150: lanzamiento hacia el aro
                float t = (frameBasquet - frameSueltaPelota) / (frameEncimaAro - frameSueltaPelota);

                t = Clamp01(t);

                // Avance directo hacia el aro
                pelotaPos = LerpVec3(posSuelta, posEncimaAro, t);

                // Arco vertical del lanzamiento
                float alturaArco = 0.75f;
                pelotaPos.y += sin(t * 3.14159265f) * alturaArco;
            }
            else if (frameBasquet < framePelotaPiso)
            {
                float t = (frameBasquet - frameEncimaAro) /
                    (framePelotaPiso - frameEncimaAro);

                t = Clamp01(t);

                if (t < 0.45f)
                {
                    float tRed = t / 0.45f;
                    pelotaPos = LerpVec3(posEncimaAro, posDebajoRed, EaseInOut(tRed));
                }
                else
                {
                    float tPiso = (t - 0.45f) / 0.55f;
                    pelotaPos = LerpVec3(posDebajoRed, posPiso, EaseInOut(tPiso));
                }
            }
            else
            {
                //la pelota se queda en el piso 
                pelotaPos = posPiso;
            }

            glm::mat4 modelPelota(1.0f);

            float pelotaScale = 1.0f;

            modelPelota = glm::translate(modelPelota, pelotaPos);
            modelPelota = glm::scale(modelPelota, glm::vec3(pelotaScale));

            glUniformMatrix4fv(
                glGetUniformLocation(shader.Program, "model"),
                1,
                GL_FALSE,
                glm::value_ptr(modelPelota)
            );

            Pelota.Draw(shader);
        }



   /*     skinnedShader.Use();
        GLint bonesLoc = glGetUniformLocation(skinnedShader.Program, "bones");
        std::vector<glm::mat4> identity(100, glm::mat4(1.0f));*/

        skinnedShader.Use();

        glUniform1i(
            glGetUniformLocation(skinnedShader.Program, "texture_diffuse1"),
            0
        );

        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "view"),
            1,
            GL_FALSE,
            glm::value_ptr(view)
        );

        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "projection"),
            1,
            GL_FALSE,
            glm::value_ptr(projection)
        );

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

      

        //glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);

        //Karaoke.UpdateAnimation(karaokeAnimTime);

        //std::vector<glm::mat4> karaokeBones;
        //Karaoke.GetBoneMatrices(karaokeBones, 100);
        //static bool imprimioKaraokeBones = false;

        //if (!imprimioKaraokeBones)
        //{
        //    std::cout << "Karaoke bones enviados: " << karaokeBones.size() << std::endl;
        //    imprimioKaraokeBones = true;
        //}

        //if (bonesLoc >= 0 && !karaokeBones.empty())
        //{
        //    glUniformMatrix4fv(
        //        bonesLoc,
        //        (GLsizei)karaokeBones.size(),
        //        GL_FALSE,
        //        &karaokeBones[0][0][0]
        //    );
        //}

        //glm::mat4 modelKaraoke(1.0f);

        //// Ajusta estos valores según dónde quieras que aparezca
        //modelKaraoke = glm::translate(modelKaraoke, glm::vec3(0.0f, 0.0f, 0.0f));
        ////modelKaraoke = glm::rotate(modelKaraoke, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //modelKaraoke = glm::scale(modelKaraoke, glm::vec3(0.01f));

        //glUniformMatrix4fv(
        //    glGetUniformLocation(skinnedShader.Program, "model"),
        //    1,
        //    GL_FALSE,
        //    glm::value_ptr(modelKaraoke)
        //);

        //Karaoke.Draw(skinnedShader);

    // Animación Karaoke
   

        glUniformMatrix4fv(bonesLoc, 100, GL_FALSE, &identity[0][0][0]);

        Karaoke.UpdateAnimation(karaokeAnimTime);

        std::vector<glm::mat4> karaokeBones;
        Karaoke.GetBoneMatrices(karaokeBones, 100);
     

        if (bonesLoc >= 0 && !karaokeBones.empty())
        {
            glUniformMatrix4fv(
                bonesLoc,
                (GLsizei)karaokeBones.size(),
                GL_FALSE,
                &karaokeBones[0][0][0]
            );
        }

        glm::mat4 modelKaraoke(1.0f);

       
        modelKaraoke = glm::translate(modelKaraoke, glm::vec3(0.0f, 0.0f, 0.0f));
        //modelKaraoke = glm::rotate(modelKaraoke, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        modelKaraoke = glm::scale(modelKaraoke, glm::vec3(0.01f));

        glUniformMatrix4fv(
            glGetUniformLocation(skinnedShader.Program, "model"),
            1,
            GL_FALSE,
            glm::value_ptr(modelKaraoke)
        );

        Karaoke.Draw(skinnedShader);

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

        if (standEnHover != 0)
        {
            glm::vec3 centro = ObtenerCentroStand(standEnHover);

            colorShader.Use();

            glUniformMatrix4fv(glGetUniformLocation(colorShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

            glUniformMatrix4fv(glGetUniformLocation(colorShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

            glm::mat4 modelCirculo = glm::mat4(1.0f);
            modelCirculo = glm::translate(modelCirculo, glm::vec3(centro.x, 0.08f, centro.z));

            glUniformMatrix4fv(glGetUniformLocation(colorShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCirculo));

            glBindVertexArray(circuloVAO);

            // Activar transparencia
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glUniform3f(glGetUniformLocation(colorShader.Program, "objectColor"), 0.55f, 0.85f, 1.0f);

            glUniform1f(glGetUniformLocation(colorShader.Program, "alpha"), 0.22f);

            glDrawArrays(GL_TRIANGLE_FAN, 0, SEGMENTOS_CIRCULO + 2);

            glUniform3f(glGetUniformLocation(colorShader.Program, "objectColor"), 0.35f, 0.75f, 1.0f);

            glUniform1f(glGetUniformLocation(colorShader.Program, "alpha"), 1.0f);

            glLineWidth(3.0f);
            glDrawArrays(GL_LINE_LOOP, 1, SEGMENTOS_CIRCULO);
            glLineWidth(1.0f);

            glDisable(GL_BLEND);

            glBindVertexArray(0);
        }

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

    CerrarMusicaKaraoke();

    glfwTerminate();
    return 0;
}

void InicializarKeyFramesStand(StandAnimacion& stand)
{
    // Frame 0: stand sin armar
    stand.keyFrame[0].baseY = -3.0f;

    stand.keyFrame[0].columnasY = -3.0f;
    stand.keyFrame[0].colIzqRotZ = 90.0f;
    stand.keyFrame[0].colDerRotZ = -90.0f;

    stand.keyFrame[0].mostradorY = -3.0f;
    stand.keyFrame[0].mostradorIzqX = -5.0f;
    stand.keyFrame[0].mostradorDerX = 5.0f;

    stand.keyFrame[0].panelY = -3.0f;
    stand.keyFrame[0].panelZ = -5.0f;

    stand.keyFrame[0].techoY = 6.0f;

    // Frame 1: sube la base
    stand.keyFrame[1] = stand.keyFrame[0];
    stand.keyFrame[1].baseY = 0.0f;

    // Frame 2: aparecen y se levantan columnas
    stand.keyFrame[2] = stand.keyFrame[1];
    stand.keyFrame[2].columnasY = 0.0f;
    stand.keyFrame[2].colIzqRotZ = 0.0f;
    stand.keyFrame[2].colDerRotZ = 0.0f;

    // Frame 3: entran mostradores y panel
    stand.keyFrame[3] = stand.keyFrame[2];
    stand.keyFrame[3].mostradorY = 0.0f;
    stand.keyFrame[3].mostradorIzqX = 0.0f;
    stand.keyFrame[3].mostradorDerX = 0.0f;

    stand.keyFrame[3].panelY = 0.0f;
    stand.keyFrame[3].panelZ = 0.0f;

    // Frame 4: baja el techo
    stand.keyFrame[4] = stand.keyFrame[3];
    stand.keyFrame[4].techoY = 0.0f;
}
void ConfigurarStand2(StandAnimacion& stand)
{
    // FRAME 0: todo desarmado
    stand.keyFrame[0].baseY = -3.0f;

    // Paredes: abajo y tiradas
    stand.keyFrame[0].columnasY = -2.5f;
    stand.keyFrame[0].colDerRotZ = 90.0f;
    stand.keyFrame[0].colIzqRotZ = -90.0f;

    // Mostrador: abajo, un poco desplazado y tirado
    stand.keyFrame[0].mostradorY = -2.0f;
    stand.keyFrame[0].mostradorIzqX = -2.0f;
    stand.keyFrame[0].mostradorRotZ = 90.0f;

    stand.keyFrame[0].techoY = 5.0f;


    // FRAME 1: sube la base
    stand.keyFrame[1] = stand.keyFrame[0];
    stand.keyFrame[1].baseY = 0.0f;


    // FRAME 2: suben y rotan paredes
    stand.keyFrame[2] = stand.keyFrame[1];
    stand.keyFrame[2].columnasY = 0.0f;
    stand.keyFrame[2].colDerRotZ = 0.0f;
    stand.keyFrame[2].colIzqRotZ = 0.0f;


    // FRAME 3: entra y rota mostrador
    stand.keyFrame[3] = stand.keyFrame[2];
    stand.keyFrame[3].mostradorY = 0.0f;
    stand.keyFrame[3].mostradorIzqX = 0.0f;
    stand.keyFrame[3].mostradorRotZ = 0.0f;


    // FRAME 4: baja techo
    stand.keyFrame[4] = stand.keyFrame[3];
    stand.keyFrame[4].techoY = 0.0f;
}
void ConfigurarStand3(StandAnimacion& stand)
{
    // FRAME 0: stand desarmado
    stand.keyFrame[0].baseY = -3.0f;

    // Columna frontal y pared derecha: abajo y tiradas
    stand.keyFrame[0].columnasY = -2.5f;
    stand.keyFrame[0].colDerRotZ = 90.0f;   // ColumFrontS3
    stand.keyFrame[0].colIzqRotZ = -90.0f;  // ParedDerS3

    // Pared trasera: abajo y tirada
    stand.keyFrame[0].panelY = -2.5f;
    stand.keyFrame[0].panelZ = -2.0f;
    stand.keyFrame[0].panelRotZ = 90.0f;

    // Mostrador: abajo, desplazado y tirado
    stand.keyFrame[0].mostradorY = -2.0f;
    stand.keyFrame[0].mostradorIzqX = -2.5f;
    stand.keyFrame[0].mostradorRotZ = 90.0f;

    // Techo/anuncio arriba
    stand.keyFrame[0].techoY = 5.0f;


    // FRAME 1: sube la base
    stand.keyFrame[1] = stand.keyFrame[0];
    stand.keyFrame[1].baseY = 0.0f;


    // FRAME 2: suben y rotan columna frontal, pared derecha y pared trasera
    stand.keyFrame[2] = stand.keyFrame[1];

    stand.keyFrame[2].columnasY = 0.0f;
    stand.keyFrame[2].colDerRotZ = 0.0f;
    stand.keyFrame[2].colIzqRotZ = 0.0f;

    stand.keyFrame[2].panelY = 0.0f;
    stand.keyFrame[2].panelZ = 0.0f;
    stand.keyFrame[2].panelRotZ = 0.0f;


    // FRAME 3: entra y rota el mostrador
    stand.keyFrame[3] = stand.keyFrame[2];

    stand.keyFrame[3].mostradorY = 0.0f;
    stand.keyFrame[3].mostradorIzqX = 0.0f;
    stand.keyFrame[3].mostradorRotZ = 0.0f;


    // FRAME 4: baja techo y anuncio
    stand.keyFrame[4] = stand.keyFrame[3];
    stand.keyFrame[4].techoY = 0.0f;
}

void ConfigurarStand4(StandAnimacion& stand)
{
    // FRAME 0: stand desarmado
    stand.keyFrame[0].baseY = -3.0f;

    // Columna y pared pantalla: abajo y tiradas
    stand.keyFrame[0].columnasY = -2.5f;
    stand.keyFrame[0].colDerRotZ = 90.0f;    // ColumnaS4
    stand.keyFrame[0].colIzqRotZ = -90.0f;   // ParedPantallaS4

    // Pared techo: abajo y tirada
    stand.keyFrame[0].panelY = -2.5f;
    stand.keyFrame[0].panelZ = -2.0f;
    stand.keyFrame[0].panelRotZ = 90.0f;

    // Mesa / mostrador: abajo, desplazada y tirada
    stand.keyFrame[0].mostradorY = -2.0f;
    stand.keyFrame[0].mostradorIzqX = -2.0f;
    stand.keyFrame[0].mostradorRotZ = 90.0f;

    // Techo 
    stand.keyFrame[0].techoY = 5.0f;


    // FRAME 1: sube la base
    stand.keyFrame[1] = stand.keyFrame[0];
    stand.keyFrame[1].baseY = 0.0f;


    // FRAME 2: suben y rotan columna, pared pantalla y pared techo
    stand.keyFrame[2] = stand.keyFrame[1];

    stand.keyFrame[2].columnasY = 0.0f;
    stand.keyFrame[2].colDerRotZ = 0.0f;
    stand.keyFrame[2].colIzqRotZ = 0.0f;

    stand.keyFrame[2].panelY = 0.0f;
    stand.keyFrame[2].panelZ = 0.0f;
    stand.keyFrame[2].panelRotZ = 0.0f;


    // FRAME 3: entra y rota la mesa / mostrador
    stand.keyFrame[3] = stand.keyFrame[2];

    stand.keyFrame[3].mostradorY = 0.0f;
    stand.keyFrame[3].mostradorIzqX = 0.0f;
    stand.keyFrame[3].mostradorRotZ = 0.0f;


    // FRAME 4: baja el techo
    stand.keyFrame[4] = stand.keyFrame[3];
    stand.keyFrame[4].techoY = 0.0f;
}

void ConfigurarStand5(StandAnimacion& stand)
{
    // FRAME 0: stand desarmado
    stand.keyFrame[0].baseY = -3.0f;

    // Pared lateral y pared trasera: abajo y tiradas
    stand.keyFrame[0].columnasY = -2.5f;
    stand.keyFrame[0].colDerRotZ = 90.0f;    // ParedLateralS5
    stand.keyFrame[0].colIzqRotZ = -90.0f;   // ParedTraseraS5

    // Pared techo: abajo y tirada
    stand.keyFrame[0].panelY = -2.5f;
    stand.keyFrame[0].panelZ = -2.0f;
    stand.keyFrame[0].panelRotZ = 90.0f;

    // Mostrador: abajo, desplazado y tirado
    stand.keyFrame[0].mostradorY = -2.0f;
    stand.keyFrame[0].mostradorIzqX = -2.0f;
    stand.keyFrame[0].mostradorRotZ = 90.0f;

    // En este stand no usaremos techoY como pieza separada,
    // porque el "techo" es ParedTechoS5 y ya lo estamos animando con panelRotZ.
    stand.keyFrame[0].techoY = 0.0f;


    // FRAME 1: sube la base
    stand.keyFrame[1] = stand.keyFrame[0];
    stand.keyFrame[1].baseY = 0.0f;


    // FRAME 2: suben y rotan pared lateral, pared trasera y pared techo
    stand.keyFrame[2] = stand.keyFrame[1];

    stand.keyFrame[2].columnasY = 0.0f;
    stand.keyFrame[2].colDerRotZ = 0.0f;
    stand.keyFrame[2].colIzqRotZ = 0.0f;

    stand.keyFrame[2].panelY = 0.0f;
    stand.keyFrame[2].panelZ = 0.0f;
    stand.keyFrame[2].panelRotZ = 0.0f;


    // FRAME 3: entra y rota el mostrador
    stand.keyFrame[3] = stand.keyFrame[2];

    stand.keyFrame[3].mostradorY = 0.0f;
    stand.keyFrame[3].mostradorIzqX = 0.0f;
    stand.keyFrame[3].mostradorRotZ = 0.0f;


    // FRAME 4: aparece el mobiliario restante
    stand.keyFrame[4] = stand.keyFrame[3];
}
void ResetearStand(StandAnimacion& stand)
{
    stand.actual = stand.keyFrame[0];
}

void InterpolarStand(StandAnimacion& stand)
{
    stand.incremento.baseY = (stand.keyFrame[stand.playIndex + 1].baseY - stand.keyFrame[stand.playIndex].baseY) / stand.maxSteps;

    stand.incremento.columnasY = (stand.keyFrame[stand.playIndex + 1].columnasY - stand.keyFrame[stand.playIndex].columnasY) / stand.maxSteps;

    stand.incremento.colIzqRotZ = (stand.keyFrame[stand.playIndex + 1].colIzqRotZ - stand.keyFrame[stand.playIndex].colIzqRotZ) / stand.maxSteps;

    stand.incremento.colDerRotZ = (stand.keyFrame[stand.playIndex + 1].colDerRotZ - stand.keyFrame[stand.playIndex].colDerRotZ) / stand.maxSteps;

    stand.incremento.mostradorY = (stand.keyFrame[stand.playIndex + 1].mostradorY - stand.keyFrame[stand.playIndex].mostradorY) / stand.maxSteps;

    stand.incremento.mostradorIzqX = (stand.keyFrame[stand.playIndex + 1].mostradorIzqX - stand.keyFrame[stand.playIndex].mostradorIzqX) / stand.maxSteps;

    stand.incremento.mostradorDerX = (stand.keyFrame[stand.playIndex + 1].mostradorDerX - stand.keyFrame[stand.playIndex].mostradorDerX) / stand.maxSteps;

    stand.incremento.mostradorRotZ = (stand.keyFrame[stand.playIndex + 1].mostradorRotZ - stand.keyFrame[stand.playIndex].mostradorRotZ) / stand.maxSteps;

    stand.incremento.panelY = (stand.keyFrame[stand.playIndex + 1].panelY - stand.keyFrame[stand.playIndex].panelY) / stand.maxSteps;

    stand.incremento.panelZ = (stand.keyFrame[stand.playIndex + 1].panelZ - stand.keyFrame[stand.playIndex].panelZ) / stand.maxSteps;

    stand.incremento.panelRotZ = (stand.keyFrame[stand.playIndex + 1].panelRotZ - stand.keyFrame[stand.playIndex].panelRotZ) / stand.maxSteps;

    stand.incremento.techoY = (stand.keyFrame[stand.playIndex + 1].techoY - stand.keyFrame[stand.playIndex].techoY) / stand.maxSteps;

}

void IniciarAnimacionStand(StandAnimacion& stand)
{
    stand.visible = true;
    stand.play = true;

    stand.playIndex = 0;
    stand.currSteps = 0;

    ResetearStand(stand);
    InterpolarStand(stand);
}

void ActualizarAnimacionStand(StandAnimacion& stand)
{
    if (!stand.play)
        return;

    if (stand.currSteps >= stand.maxSteps)
    {
        stand.playIndex++;

        if (stand.playIndex > STAND_FRAMES - 2)
        {
            stand.actual = stand.keyFrame[STAND_FRAMES - 1];
            stand.play = false;
            return;
        }

        stand.actual = stand.keyFrame[stand.playIndex];
        stand.currSteps = 0;
        InterpolarStand(stand);
    }
    else
    {
        stand.actual.baseY += stand.incremento.baseY;

        stand.actual.columnasY += stand.incremento.columnasY;
        stand.actual.colIzqRotZ += stand.incremento.colIzqRotZ;
        stand.actual.colDerRotZ += stand.incremento.colDerRotZ;

        stand.actual.mostradorY += stand.incremento.mostradorY;
        stand.actual.mostradorIzqX += stand.incremento.mostradorIzqX;
        stand.actual.mostradorDerX += stand.incremento.mostradorDerX;
        stand.actual.mostradorRotZ += stand.incremento.mostradorRotZ;

        stand.actual.panelY += stand.incremento.panelY;
        stand.actual.panelZ += stand.incremento.panelZ;
        stand.actual.panelRotZ += stand.incremento.panelRotZ;

        stand.actual.techoY += stand.incremento.techoY;

        stand.currSteps++;
    }
}

//Detección de clics
bool ObtenerPuntoClickEnPiso(GLFWwindow* window, glm::vec3& punto)
{
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    float x = (2.0f * (float)mouseX) / (float)SCREEN_WIDTH - 1.0f;
    float y = 1.0f - (2.0f * (float)mouseY) / (float)SCREEN_HEIGHT;

    glm::vec4 rayClip = glm::vec4(x, y, -1.0f, 1.0f);

    glm::vec4 rayEye = glm::inverse(projection) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);

    glm::mat4 view = camera.GetViewMatrix();
    glm::vec3 rayWorld = glm::normalize(glm::vec3(glm::inverse(view) * rayEye));

    glm::vec3 rayOrigin = camera.GetPosition();

    if (std::abs(rayWorld.y) < 0.0001f)
        return false;

    float t = -rayOrigin.y / rayWorld.y;

    if (t < 0.0f)
        return false;

    punto = rayOrigin + rayWorld * t;
    return true;
}

bool PuntoCercaDeStand(glm::vec3 punto, glm::vec3 centroStand, float radio)
{
    float distanciaXZ = glm::length(glm::vec2(
        punto.x - centroStand.x,
        punto.z - centroStand.z
    ));

    return distanciaXZ <= radio;
}

glm::vec3 ObtenerCentroStand(int numeroStand)
{
    switch (numeroStand)
    {
    case 1:
        return STAND1_CENTER;
    case 2:
        return STAND2_CENTER;
    case 3:
        return STAND3_CENTER;
    case 4:
        return STAND4_CENTER;
    case 5:
        return STAND5_CENTER;
    default:
        return glm::vec3(0.0f);
    }
}

bool StandDisponible(int numeroStand)
{
    switch (numeroStand)
    {
    case 1:
        return !stand1Anim.visible;
    case 2:
        return !stand2Anim.visible;
    case 3:
        return !stand3Anim.visible;
    case 4:
        return !stand4Anim.visible;
    case 5:
        return !stand5Anim.visible;
    default:
        return false;
    }
}

void ActualizarHoverStand(GLFWwindow* window)
{
    standEnHover = 0;

    glm::vec3 puntoCursor;

    if (!ObtenerPuntoClickEnPiso(window, puntoCursor))
        return;

    if (StandDisponible(1) && PuntoCercaDeStand(puntoCursor, STAND1_CENTER, RADIO_DETECCION_STAND))
    {
        standEnHover = 1;
    }
    else if (StandDisponible(2) && PuntoCercaDeStand(puntoCursor, STAND2_CENTER, RADIO_DETECCION_STAND))
    {
        standEnHover = 2;
    }
    else if (StandDisponible(3) && PuntoCercaDeStand(puntoCursor, STAND3_CENTER, RADIO_DETECCION_STAND))
    {
        standEnHover = 3;
    }
    else if (StandDisponible(4) && PuntoCercaDeStand(puntoCursor, STAND4_CENTER, RADIO_DETECCION_STAND))
    {
        standEnHover = 4;
    }
    else if (StandDisponible(5) && PuntoCercaDeStand(puntoCursor, STAND5_CENTER, RADIO_DETECCION_STAND))
    {
        standEnHover = 5;
    }
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
            // Activar/desactivar animación de la persona con L
            if (key == GLFW_KEY_L)
            {
                if (!mostrarLanzamiento || pruebaAnimTime >= pruebaAnimDuration - 0.01f)
                {
                    pruebaAnimTime = 0.0f;
                    playPruebaAnim = true;
                    mostrarLanzamiento = true;
                }
                else
                {
                    playPruebaAnim = !playPruebaAnim;
                    mostrarLanzamiento = true;
                }
            }

            // Activar/desactivar animación Karaoke con K
        // Activar / pausar karaoke con K
            if (key == GLFW_KEY_K)
            {
                // Si no se está mostrando o ya terminó, inicia desde cero
                if (!mostrarPantallaKaraoke || karaokeAnimTime >= (float)karaokeDuration - 0.01f)
                {
                    karaokeAnimTime = 0.0f;
                    playKaraoke = true;
                    mostrarPantallaKaraoke = true;

                    ReproducirMusicaKaraokeDesdeInicio();
                }
                else
                {
                    // Si ya estaba iniciado, K pausa o reanuda
                    playKaraoke = !playKaraoke;

                    if (playKaraoke)
                    {
                        ReanudarMusicaKaraoke();
                    }
                    else
                    {
                        PausarMusicaKaraoke();
                    }
                }
            }
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

 
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    {
        ActualizarHoverStand(window);

        if (standEnHover == 1)
        {
            IniciarAnimacionStand(stand1Anim);
        }
        else if (standEnHover == 2)
        {
            IniciarAnimacionStand(stand2Anim);
        }
        else if (standEnHover == 3)
        {
            IniciarAnimacionStand(stand3Anim);
        }
        else if (standEnHover == 4)
        {
            IniciarAnimacionStand(stand4Anim);
        }
        else if (standEnHover == 5)
        {
            IniciarAnimacionStand(stand5Anim);
        }

        standEnHover = 0;
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