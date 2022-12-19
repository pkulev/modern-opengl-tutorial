#define GL_SILENCE_DEPRECATION

#include <iostream>

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5, 0.0);
        glVertex2f(0.0, 0.5);
        glVertex2f(0.5, 0.0);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
