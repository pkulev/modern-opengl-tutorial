#define GL_SILENCE_DEPRECATION

#include <iostream>
#include <fstream>

#include <argparse/argparse.hpp>
#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>


argparse::ArgumentParser parse_args(int argc, char** argv) {
    argparse::ArgumentParser program("ogl-sandbox");

    program.add_argument("square")
        .help("number to calculate square")
        .scan<'i', int>();

    try {
        program.parse_args(argc, argv);
    }
    catch (const std::runtime_error& err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    return program;
}


static unsigned int compileShader(unsigned int type, const std::string& source) {
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length,  message);
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << "shader: " << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint createShader(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(int argc, char** argv) {
    auto args = parse_args(argc, argv);
    auto input = args.get<int>("square");
    std::cout << (input * input) << std::endl;

    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "ogl-sandbox", NULL, NULL);
    if (!window)
    {
        std::cout << "Error: Unable to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cout << "Error: Unable to initialize GLEW." << std::endl;
        glfwTerminate();
        return -1;
    };

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    float dots[6] = {
        -0.5, -0.5,
         0.0,  0.5,
         0.5, -0.5,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);

    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), dots, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    std::ifstream vertexShaderFile;
    vertexShaderFile.open("./res/shaders/vertex.glsl");
    std::string vertexShader((std::istreambuf_iterator<char>(vertexShaderFile)),
                         std::istreambuf_iterator<char>());

    std::ifstream fragmentShaderFile;
    fragmentShaderFile.open("./res/shaders/fragment.glsl");
    std::string fragmentShader((std::istreambuf_iterator<char>(fragmentShaderFile)),
                               std::istreambuf_iterator<char>());

    GLuint shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}
