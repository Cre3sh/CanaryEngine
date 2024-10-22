#include "Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "Engine/ShaderManager/ShaderManager.h"
#include "Engine/UIManager/UIManager.h"
#include "stb/stb_image.h"

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

    // Configure global opengl state
    glEnable(GL_DEPTH_TEST);

    ShaderManager EngineShaderManager("shaders/VertexShader.vert", "shaders/FragmentShader.frag");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions
    glm::vec3 CubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);

    // We need to store our vertex data on the GPU's memory, which is done using Vertex Buffer Objects (VBOs)
    glGenBuffers(1, &VBO);

    // For EBOs
    //glGenBuffers(1, &EBO);

    //////////////////////////////////////
    // VERTEX ARAY OBJECT (VBO)         //
    /////////////////////////////////////

    // a Vertex Array Object (VAO) encapsulates the entire "state" (In OpenGL, "state" refers to all the settings, configurations,
    // and data that OpenGL uses to manage how rendering is done) necessary for describing vertex data, so you don’t need to 
    // repeatedly bind buffers and set vertex attributes (position, color etc.) every 
    // time you draw something. Once you configure the VAO, you can bind 
    // it anytime and all the previously set configurations are remembered.

    // In OpenGL, binding means making an object (like a buffer or a texture) active
    // or current so that any operations you perform apply to that object.
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // In this case, we are specifying the target of the buffer (GL_ARRAY_BUFFER), the size of the data (in bytes),
    // The actual data (our array of vertices) and a usage hint telling OpenGL that the data will not change often
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // An EBO is a buffer, just like a vertex buffer object, that stores indices that OpenGL uses to decide what vertices to draw.
    // If we want to draw a square using two triangles. Instead of defining each corner of the square multiple times,
    // we can define each corner once and then use indices to refer to these corners.
    // This makes our program more memory efficient as we don’t need to repeat vertex data for vertices that are shared between shapes.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer defines how OpenGL should interpret the vertex data stored in a Vertex Buffer Object (VBO).

    // 0: This is the attribute location. In our vertex shader, we wrote: layout(location = 0) in vec3 aPos;
    // This tells OpenGL that this vertex attribute corresponds to the vertex position and should be accessed using index 0.

    // 3: This parameter specifies the size of the vertex attribute, the attribute is a vec3 so it is composed of 3 values

    // GL_FLOAT: this specifies the type of the data (in this case float)

    // GL_FALSE: This specifies whether fixed-point data values should be normalized. 
    // Since we are using floating-point data for positions, we set this to GL_FALSE

    // 8 * sizeof(float): This argument is known as the stride and tells us the space between consecutive vertex attributes.
    // Since the next set of position data is located exactly 8 times the size of a float away (3 floats for position, 3 floats for color, 2 floats for tex coordinates), we specify that value as the stride.
    // https://stackoverflow.com/questions/22296510/what-does-stride-mean-in-opengles#:~:text=Amount%20of%20bytes%20from%20the,means%20they%20are%20tightly%20packed.

    // (void*)0): This parameter is the offset within the buffer where this attribute begins.
    // Here, it is set to (void*)0, meaning that the position data starts at the beginning of the buffer (offset 0).

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // (void*)(3* sizeof(float))) : The color attribute starts after the position data so the
    // offset is 3 * sizeof(float) in bytes (= 12 bytes).

    // Texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int Texture, Texture2;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    // load image, create texture and generate mipmaps
    int Width, Height, NumChannels;
    unsigned char* Data = stbi_load("resources/textures/container.jpg", &Width, &Height, &NumChannels, 0);

    if (Data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(Data);

    // Texture2
    glGenTextures(1, &Texture2);
    glBindTexture(GL_TEXTURE_2D, Texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    Data = stbi_load("resources/textures/awesomeface.png", &Width, &Height, &NumChannels, 0);
    if (Data)
    {
        // note that the our image is a PNG and has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(Data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    EngineShaderManager.Use(); // activate/use the shader before setting uniforms
    // either set it manually like so:
    glUniform1i(glGetUniformLocation(EngineShaderManager.ID, "texture1"), 0);
    glUniform1i(glGetUniformLocation(EngineShaderManager.ID, "texture2"), 1);

    // unbind the currently bound buffer and vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // User Interface
    UIManager UserInterface;

    UserInterface.Intialise(Window);

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer

        UserInterface.NewFrame();

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Texture2);

        EngineShaderManager.Use();

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
        EngineShaderManager.SetMat4("ProjectionMatrix", projection);

        // camera/view transformation
        glm::mat4 view = Camera.GetViewMatrix();
        EngineShaderManager.SetMat4("ViewMatrix", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, CubePositions[i]);
            float angle = 20.0f * i;
            if (i % 4 == 0)
            {
                // every 3rd iteration (including the first) we set the angle using GLFW's time function.
                angle = glfwGetTime() * 25.0f;
            }

            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            EngineShaderManager.SetMat4("ModelMatrix", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        UserInterface.AddDebugWindow();
        UserInterface.Render();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    UserInterface.Shutdown();

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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