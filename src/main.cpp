#define GL_SILENCE_DEPRECATION

#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <argparse/argparse.hpp>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "opengl.hpp"

argparse::ArgumentParser parse_args(int argc, char **argv) {
    argparse::ArgumentParser program("ogl-sandbox");

    program.add_argument("square")
        .help("number to calculate square")
        .scan<'i', int>();

    try {
        program.parse_args(argc, argv);
    } catch (const std::runtime_error &err) {
        std::cerr << err.what() << std::endl;
        std::cerr << program;
        std::exit(1);
    }

    return program;
}

static std::string loadShader(const std::string &filepath) {
    std::ifstream shaderFile;
    shaderFile.open(filepath);
    std::string shaderSource((std::istreambuf_iterator<char>(shaderFile)),
                             std::istreambuf_iterator<char>());

    return shaderSource;
}

static unsigned int compileShader(unsigned int type, const std::string &source) {
    GLuint id = CALL_GL(glCreateShader(type));
    const char *src = source.c_str();
    CALL_GL(glShaderSource(id, 1, &src, nullptr));
    CALL_GL(glCompileShader(id));

    int result;
    CALL_GL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
                  << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
                  << "shader: " << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static GLuint createShader(const std::string &vertexShader, const std::string &fragmentShader) {
    GLuint program = CALL_GL(glCreateProgram());
    GLuint vs = CALL_GL(compileShader(GL_VERTEX_SHADER, vertexShader));
    GLuint fs = CALL_GL(compileShader(GL_FRAGMENT_SHADER, fragmentShader));

    CALL_GL(glAttachShader(program, vs));
    CALL_GL(glAttachShader(program, fs));
    CALL_GL(glLinkProgram(program));
    CALL_GL(glValidateProgram(program));

    CALL_GL(glDeleteShader(vs));
    CALL_GL(glDeleteShader(fs));

    return program;
}

int main(int argc, char **argv) {
    auto args = parse_args(argc, argv);
    auto input = args.get<int>("square");
    std::cout << (input * input) << std::endl;

    GLFWwindow *window;

    // TODO: check errors
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "ogl-sandbox", NULL, NULL);
    if (!window) {
        std::cout << "Error: Unable to create GLFW window." << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Vsync
    glfwSwapInterval(1);

    // TODO: check errors
    if (glewInit() != GLEW_OK) {
        std::cout << "Error: Unable to initialize GLEW." << std::endl;
        glfwTerminate();
        return -1;
    };

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    // clang-format off
    float dots[] = {
        -0.5, -0.5,
         0.5, -0.5,
         0.5,  0.5,
        -0.5,  0.5,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    // clang-format on

    unsigned int buffer;
    CALL_GL(glGenBuffers(1, &buffer));

    unsigned int vao;
    CALL_GL(glGenVertexArrays(1, &vao));
    CALL_GL(glBindVertexArray(vao));

    CALL_GL(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    CALL_GL(glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), dots, GL_STATIC_DRAW));
    CALL_GL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0));
    CALL_GL(glEnableVertexAttribArray(0));

    unsigned int ibo;
    CALL_GL(glGenBuffers(1, &ibo));
    CALL_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    CALL_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    std::string vertexShader = loadShader("./res/shaders/vertex.glsl");
    std::string fragmentShader = loadShader("./res/shaders/fragment.glsl");

    GLuint shader = createShader(vertexShader, fragmentShader);
    CALL_GL(glUseProgram(shader));

    // Pass color into shader.
    int u_Color = CALL_GL(glGetUniformLocation(shader, "u_Color"));
    float r = 0.0;
    float dr = 0.05;

    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        CALL_GL(glClear(GL_COLOR_BUFFER_BIT));

        CALL_GL(glUniform4f(u_Color, r, 0.3, 0.8, 1.0));
        CALL_GL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (r > 1.0) {
            dr = -dr;
        } else if (r < 0.0) {
            dr = -dr;
        }

        r += dr;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    CALL_GL(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}
