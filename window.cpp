#include "include/glad/glad.h"  // Include GLAD for OpenGL function loading
#include <GLFW/glfw3.h>         // Include GLFW for window and input management
#include <iostream>              // Include iostream for standard input/output

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height); 
void processInput(GLFWwindow* window); 

// Vertex Shader Source Code
const char* vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char* fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.2f, 1.0f);\n"
    "}\n\0";


int main() {
    // glfw: initialize and configure
    // ------------------------------

    // Initialize GLFW (OpenGL Windowing Library)
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // Set OpenGL version and profile hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Major version (3.3 in this case)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);  // Minor version 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Use the core profile

    // Set window resizability
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Disables window resizing

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 600, "KREWER TEST", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    
    // Set the current OpenGL context to the window
    glfwMakeContextCurrent(window);
    // Set the callback function for window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // Initialize GLAD (OpenGL Loading Library)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    // Set the viewport (the area where rendering happens)
    glViewport(0, 0, 800, 600);  // Set the viewport to the full window size

    // build and compile our shader program
    // ------------------------------------

    // Create a vertex shader object
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Attach the vertex shader source code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check compilation vertex
    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success){
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // Create a fragment shader object
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Attach the fragment shader source code to the shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check compilation fragment
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if(!success){
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // Create program to link our shader
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    // Attach shader to the program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check compilation of program to attach shader
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::ATTACHING::COMPILATION_FAILED\n" << infoLog << '\n';
    }

    // Delete those once we've linked them into the program, we no longer need them anymore.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for the triangle (coordinates)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

        // Create a Vertex Buffer Object (VBO) to store vertex data
    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);  // Generate a VBO with a unique ID
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO); // Bind the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Send data to VBO

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        // Process input events (like keyboard presses)
        processInput(window);

        // Set the background color
        glClearColor(101.0f / 255.0f, 33.0f / 255.0f, 189.0f / 255.0f, 1.0f);  // Set a light blue background
        glClear(GL_COLOR_BUFFER_BIT);  // Clear the color buffer

        // Calls the shader and fragmentation
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Process events (like window resizing, mouse clicks, etc.)
        glfwPollEvents();

        // Swap the front and back buffers (for smooth rendering)
        glfwSwapBuffers(window);
    }

    // Terminate GLFW and clean up
    glfwTerminate();
    return 0;
}

// Callback function for window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height); // Update the viewport to match the new window size
}

// Function to process input events
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // Check for Escape key press
        glfwSetWindowShouldClose(window, true); // Set the window close flag to true
    }
}