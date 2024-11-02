#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Engine/Shader/ShaderProgram.h"
#include "Engine/UI/UIManager.h"
#include "stb/stb_image.h"
#include "Mesh/Model.h"
#include "Lighting/LightingManager.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* InWindow, double InXpos, double InYpos);

void Application::Run()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* Window = glfwCreateWindow(800, 600, "Engine", NULL, NULL);
    if (Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    // Set the user pointer to 'this'
    glfwSetWindowUserPointer(Window, this);

    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);
    glfwSetCursorPosCallback(Window, mouse_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);

    ShaderProgram EngineShaderManager("shaders/ObjectVertexShader.vert", "shaders/ObjectFragmentShader.frag");

    // load models
    // -----------
    Model BackpackModel("resources/Objects/Backpack/backpack.obj");

    // User Interface
    UIManager UserInterface;

    UserInterface.Intialise(Window);

    unsigned int lights_index = glGetUniformBlockIndex(EngineShaderManager.ID, "LightBlock");
    glUniformBlockBinding(EngineShaderManager.ID, lights_index, 2);

    // Lighting
    LightManager LightingManager;

    Light TestLight;
    TestLight.LightPosition = glm::vec3(5.0f, 0.0f, 5.0f);
    TestLight.LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    TestLight.Intensity = 15.0f;
    TestLight.LightType = 1;
    TestLight.LightRadius = 50000.0f;
    TestLight.LightDirection = glm::vec3(0.0f, 0.0f, 0.0f);
    TestLight.LightCutOff = 100.0f;

    LightingManager.AddLight(TestLight);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(Window))
    {
        float currentFrame = glfwGetTime();
        DeltaTime = currentFrame - LastFrame;
        LastFrame = currentFrame;

        // input
        // -----
        ProcessInput(Window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        UserInterface.NewFrame();

        EngineShaderManager.Use();

        // Bind the UBO containing lights before rendering
        LightingManager.UpdateLights(EngineShaderManager);

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
        glm::mat4 view = Camera.GetViewMatrix();
        EngineShaderManager.SetMat4("ProjectionMatrix", projection);
        EngineShaderManager.SetMat4("ViewMatrix", view);
        EngineShaderManager.SetVec3("ViewPos", Camera.GetPosition());

        // render the loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        EngineShaderManager.SetMat4("ModelMatrix", model);
        BackpackModel.Draw(EngineShaderManager);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        EngineShaderManager.SetMat4("ModelMatrix", model);
        BackpackModel.Draw(EngineShaderManager);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(10.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        EngineShaderManager.SetMat4("ModelMatrix", model);
        BackpackModel.Draw(EngineShaderManager);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(15.0f, 0.0f, 0.0f)); 
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	
        EngineShaderManager.SetMat4("ModelMatrix", model);
        BackpackModel.Draw(EngineShaderManager);

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error: " << err << std::endl;
        }

        UserInterface.AddDebugWindow();
        UserInterface.Render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    UserInterface.Shutdown();

    // terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
}

void Application::ProcessInput(GLFWwindow* InWindow)
{
    if (glfwGetKey(InWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(InWindow, true);
    }
    else if (glfwGetKey(InWindow, GLFW_KEY_F3) == GLFW_PRESS)
    {
        if (bWireframeMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            bWireframeMode = false;
        }
        else
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            bWireframeMode = true;
        }
    }

    const float cameraSpeed = 5.0f * DeltaTime;
    if (glfwGetKey(InWindow, GLFW_KEY_W) == GLFW_PRESS)
    {
        Camera.ProcessKeyboardInput(Forward, DeltaTime);
    }
    if (glfwGetKey(InWindow, GLFW_KEY_S) == GLFW_PRESS)
    {
        Camera.ProcessKeyboardInput(Backward, DeltaTime);
    }
    if (glfwGetKey(InWindow, GLFW_KEY_A) == GLFW_PRESS)
    {
        Camera.ProcessKeyboardInput(Left, DeltaTime);
    }
    if (glfwGetKey(InWindow, GLFW_KEY_D) == GLFW_PRESS)
    {
        Camera.ProcessKeyboardInput(Right, DeltaTime);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* InWindow, double InXpos, double InYpos)
{
    Application* App = static_cast<Application*>(glfwGetWindowUserPointer(InWindow));

    float xpos = static_cast<float>(InXpos);
    float ypos = static_cast<float>(InYpos);

    if (App->FirstMouseMove)
    {
        App->LastX = xpos;
        App->LastY = ypos;
        App->FirstMouseMove = false;
    }

    float xoffset = xpos - App->LastX;
    float yoffset = App->LastY - ypos; // reversed since y-coordinates go from bottom to top

    App->LastX = xpos;
    App->LastY = ypos;

    App->Camera.ProcessMouseMovement(xoffset, yoffset);
}