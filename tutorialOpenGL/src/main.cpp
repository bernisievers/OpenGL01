#include <iostream>
#include <glad/glad.h> //glad goes before GLFW
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <string>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
std::string loadShaderSrc(const char* filename);


int main() {
	std::cout << "Hello, World!" << std::endl; // Output to console

    int success;
    char infoLog[512];
    //glm test
    //glm::vec4 vec(5.0f, 1.0f, 1.0f, 1.0f);
    //glm::mat4 trans = glm::mat4(1.0f); //identity matrix
    //trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f)); // translation
    //trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));//rotation
    //trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));//scale
    
    //vec = trans * vec;
    //std::cout << vec.x << ' ' << vec.y << ' ' << vec.z << std::endl; // Output to console


    glfwInit();
    //open openGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Tutorial", NULL, NULL);
    if (window == NULL) { //window not created
        std::cout << "Could not create window." << std::endl;
        glfwTerminate();
        return -1;
    
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /*
        Shaders
    */

    //compile vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    std::string vertShaderSrc = loadShaderSrc("assets/vertex_core.glsl");
    const GLchar* vertShader = vertShaderSrc.c_str();
    glShaderSource(vertexShader, 1, &vertShader, NULL);
    glCompileShader(vertexShader);

    //catch error
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "error with vertex shader comp.:" << std::endl << infoLog << std::endl;
    }

    //compile fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragShaderSrc = loadShaderSrc("assets/fragment_core.glsl");
    const GLchar* fragShader = fragShaderSrc.c_str();
    glShaderSource(fragmentShader, 1, &fragShader, NULL);
    glCompileShader(fragmentShader);

    //catch error
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "error with frag shader comp.:" << std::endl << infoLog << std::endl;
    }

    //Create Program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //catch errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Linking error:" << std::endl << infoLog << std::endl;
    }

    glDeleteShader(vertexShader); // delete the shaders
    glDeleteShader(fragmentShader);

    //vertex array
    float vertices[] = {
        //first triangle
        0.5f, 0.5f, 0.0f,
        -0.5f, 0.5f, 0.0f,
        -0.5f,-0.5f,0.0f,
        0.5f, -0.5f, 0.0f,
    };
    unsigned int indices[] = {
        0, 1, 2, //first triangle
        2, 3, 0  //2nd triang
    };

    // VAO, VBO vertex array object y vertex buffer object
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind VAO
    glBindVertexArray(VAO);

    //bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // set attribute pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //set up EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    while (!glfwWindowShouldClose(window)) {
        //process input
        processInput(window);
        
        //render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // this create a green blueish color
        glClear(GL_COLOR_BUFFER_BIT);

        //draw shapes
        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        //send new frame to window
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}

//define the method, reset viewport, if resize or message...
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        std::cout << "Used escape to exit window" << std::endl;
        glfwSetWindowShouldClose(window, true);
    }
}

std::string loadShaderSrc(const char* filename) {
    std::ifstream file;
    std::stringstream buf;

    std::string ret = "";

    file.open(filename);

    if (file.is_open()) {
        buf << file.rdbuf();
        ret = buf.str();
    }
    else {
        std::cout << "could not open" << filename << std::endl;
    }

    file.close();
    return ret;
}