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

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

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
    //GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final Equipo #10: Carga de modelos y camara sintetica", nullptr, nullptr);
    
    // Pantalla completa para ver mejor el proyecto
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height,
        "Proyecto Final Equipo #10", monitor, nullptr);

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

    // Load models
    Model CashierArea((char*)"Models/CashierArea/CashierArea.obj");
	Model Stand1((char*)"Models/Stand1/Stand1.obj");
	Model Stand2((char*)"Models/Stand2/Stand2.obj");
	Model Stand3((char*)"Models/Stand3/Stand3.obj");
	Model Stand4((char*)"Models/Stand4/Stand4.obj");
	Model Stand5((char*)"Models/Stand5/Stand5.obj");

    // VAO/VBO para los cubos indicadores de luces
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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

        // Swap the buffers
        glfwSwapBuffers(window);
    }

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