#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* VertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* FragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.7f, 0.1f, 0.1f, 1.0f);\n"
"}\n\0";

bool bWireframeMode = false;

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    GLFWwindow* Window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (Window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(Window);
    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // vertex shader
    unsigned int VertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(VertexShader);

    // fragment shader
    unsigned int FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(FragmentShader);

    // link shaders
    unsigned int ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);

    // We need to store our vertex data on the GPU's memory, which is done using Vertex Buffer Objects (VBOs)
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer defines how OpenGL should interpret the vertex data stored in a Vertex Buffer Object (VBO).

    // 0: This is the attribute location. In our vertex shader, we wrote: layout(location = 0) in vec3 aPos;
    // This tells OpenGL that this vertex attribute corresponds to the vertex position and should be accessed using index 0.

    // 3: This parameter specifies the size of the vertex attribute, the attribute is a vec3 so it is composed of 3 values

    // GL_FLOAT: this specifies the type of the data (in this case float)

    // GL_FALSE: This specifies whether fixed-point data values should be normalized. 
    // Since we are using floating-point data for positions, we set this to GL_FALSE

    // 3 * sizeof(float): This argument is known as the stride and tells us the space between consecutive vertex attributes.
    // Since the next set of position data is located exactly 3 times the size of a float away we specify that value as the stride.
    // https://stackoverflow.com/questions/22296510/what-does-stride-mean-in-opengles#:~:text=Amount%20of%20bytes%20from%20the,means%20they%20are%20tightly%20packed.

    // (void*)0): This parameter is the offset within the buffer where this attribute begins.
    // Here, it is set to (void*)0, meaning that the position data starts at the beginning of the buffer (offset 0).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // unbind the currently bound buffer and vertex array.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(Window))
    {
        // input
        // -----
        processInput(Window);

        // render
        // ------
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(ShaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    // de-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(ShaderProgram);

    // terminate, clearing all previously allocated GLFW resources.
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    else if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
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
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}