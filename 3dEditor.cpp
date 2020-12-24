//#define GLEW_STATIC
#include <GL/glew.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include "obj_loader.h"
#include "mesh.h"
#include "shader.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "controls.h"

const GLchar* vertexSource = R"glsl(
    #version 150 core

    uniform float time;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    in vec3 position;
    in vec2 texture;

    out vec2 vTexture;

    void main()
    {
        vTexture = texture;
        gl_Position = projection * view * model * vec4(position, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 150 core

    uniform float time;
    uniform sampler2D tex;

    in vec2 vTexture;

    out vec4 outColor;

    void main()
    {
        outColor = texture(tex, vTexture);
    }
)glsl";

static const int DISPLAY_WIDTH = 800;
static const int DISPLAY_HEIGHT = 600;

void modelViewProjection(GLuint shaderProgram, float time) {

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
    model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(time * 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    GLint uniformTranslation = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(uniformTranslation, 1, GL_FALSE, glm::value_ptr(model));

    // View matrix
    glm::mat4 view = glm::lookAt(
        glm::vec3(5.0f, 5.0f, 5.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    // Projection matrix
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 1.0f, 10.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}


int main()
{
    // Configure OpenGL Context through SFML
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;
    settings.majorVersion = 4;
    settings.minorVersion = 0;
    settings.attributeFlags = sf::ContextSettings::Core;

    // Create the main window
    sf::Window window(sf::VideoMode(DISPLAY_WIDTH, DISPLAY_HEIGHT), "OpenGL", sf::Style::Resize | sf::Style::Close, settings);
    window.setVerticalSyncEnabled(true);

    settings = window.getSettings();

    std::cout << "depth bits:" << settings.depthBits << std::endl;
    std::cout << "stencil bits:" << settings.stencilBits << std::endl;
    std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
    std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;

    // start glew
    glewExperimental = GL_TRUE;
    glewInit();

    //// load 3D model
    //objl::Loader loader;
    //loader.LoadFile("cube.obj");

    //// load texture
    //sf::Image img_data;
    //if (!img_data.loadFromFile("D:\\Dokumenty\\MagisterKononowirus\\3D\\Lab2\\lab2\\GL\\Lab2 3D\\cube.png"))
    //{
    //    std::cout << "Texture not loaded!" << std::endl;
    //}

    //// texture
    //GLuint tex;
    //glGenTextures(1, &tex);
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, tex);

    //sf::Vector2u texSize = img_data.getSize();
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSize.x, texSize.y,
    //    0, GL_RGBA, GL_UNSIGNED_BYTE, img_data.getPixelsPtr());


    //float borderColor[] = { 1.0f, 0.0f, 1.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //glGenerateMipmap(GL_TEXTURE_2D);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    //// enable depth
    //glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    //GLuint vertexCount = loader.LoadedVertices.size();
    //GLuint indexCount = loader.LoadedIndices.size();

    //std::vector<float> vertexAttributes(vertexCount * 5);

    //// Create a vertex array object
    //GLuint vao;
    //glGenVertexArrays(1, &vao);
    //glBindVertexArray(vao);

    //// Create vertex buffer
    //GLuint vbo;
    //glGenBuffers(1, &vbo); // Generate 1 buffer
    //glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //for (int i = 0; i < vertexCount; i++) {
    //    vertexAttributes[i * 5] = loader.LoadedVertices[i].Position.X;
    //    vertexAttributes[i * 5 + 1] = loader.LoadedVertices[i].Position.Y;
    //    vertexAttributes[i * 5 + 2] = loader.LoadedVertices[i].Position.Z;
    //    vertexAttributes[i * 5 + 3] = loader.LoadedVertices[i].TextureCoordinate.X;
    //    vertexAttributes[i * 5 + 4] = loader.LoadedVertices[i].TextureCoordinate.Y;
    //}

    //glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertexCount * 5, vertexAttributes.data(), GL_STATIC_DRAW);

    //// load faces / indices
    //GLuint ebo;
    //glGenBuffers(1, &ebo);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indexCount, loader.LoadedIndices.data(), GL_STATIC_DRAW);


    //// Load texture


    //// Create and compile the vertex shader
    //GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //glShaderSource(vertexShader, 1, &vertexSource, NULL);
    //glCompileShader(vertexShader);

    //// Create and compile the fragment shader
    //GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    //glCompileShader(fragmentShader);


    //// combine shaders into a program
    //GLuint shaderProgram = glCreateProgram();
    //glAttachShader(shaderProgram, vertexShader);
    //glAttachShader(shaderProgram, fragmentShader);

    //glBindFragDataLocation(shaderProgram, 0, "outColor");
    //glLinkProgram(shaderProgram);
    //glUseProgram(shaderProgram);

    //// Specify the layout of the vertex data
    //GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    //glEnableVertexAttribArray(posAttrib);
    //glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

    //// Specify texture data
    //GLint texAttrib = glGetAttribLocation(shaderProgram, "texture");
    //glEnableVertexAttribArray(texAttrib);
    //glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


    //// Set uniforms
    //auto t_start = std::chrono::high_resolution_clock::now();
    //GLint timeUni = glGetUniformLocation(shaderProgram, "time");

    //// main loop
    //bool running = true;
    //while (running)
    //{
    //    // handle events
    //    sf::Event event;
    //    while (window.pollEvent(event))
    //    {
    //        if (event.type == sf::Event::Closed)
    //        {
    //            // end the program
    //            running = false;
    //        }
    //        else if (event.type == sf::Event::Resized)
    //        {
    //            // adjust the viewport when the window is resized
    //            glViewport(0, 0, event.size.width, event.size.height);
    //        }
    //        else if (event.type == sf::Event::KeyPressed) {
    //            if (event.key.code == sf::Keyboard::Escape)
    //                running = false;
    //            break;
    //        }
    //    }

    //    // make sure the graphic context is active
    //    window.setActive();

    //    // clear the buffers
    //    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //    // set uniforms
    //    auto t_now = std::chrono::high_resolution_clock::now();
    //    float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
    //    glUniform1f(timeUni, time);

    //    // Set Model View Projection matrices
    //    modelViewProjection(shaderProgram, time);

    //    // draw...
    //    //glDrawArrays(GL_TRIANGLES, 0, 10);
    //    glDrawElements(GL_TRIANGLES, indexCount * 3, GL_UNSIGNED_INT, 0);

    //    // end the current frame (internally swaps the front and back buffers)
    //    window.display();
    //    sf::sleep(sf::seconds(0.01f));
    //}

    //// release resources...

    //glDeleteProgram(shaderProgram);
    //glDeleteShader(fragmentShader);
    //glDeleteShader(vertexShader);

    //glDeleteBuffers(1, &vbo);

    //glDeleteVertexArrays(1, &vao);

Vertex vertices[] =
{
    Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 0), glm::vec3(0, 0, -1)),
    Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 0, -1)),
    Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 0, -1)),
    Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(0, 0, -1)),

    Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(0, 0, 1)),
    Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(0, 0, 1)),
    Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 1), glm::vec3(0, 0, 1)),
    Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 1), glm::vec3(0, 0, 1)),

    Vertex(glm::vec3(-1, -1, -1), glm::vec2(0, 1), glm::vec3(0, -1, 0)),
    Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 1), glm::vec3(0, -1, 0)),
    Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(0, -1, 0)),
    Vertex(glm::vec3(1, -1, -1), glm::vec2(0, 0), glm::vec3(0, -1, 0)),

    Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(0, 1, 0)),
    Vertex(glm::vec3(-1, 1, 1), glm::vec2(1, 1), glm::vec3(0, 1, 0)),
    Vertex(glm::vec3(1, 1, 1), glm::vec2(1, 0), glm::vec3(0, 1, 0)),
    Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 0), glm::vec3(0, 1, 0)),

    Vertex(glm::vec3(-1, -1, -1), glm::vec2(1, 1), glm::vec3(-1, 0, 0)),
    Vertex(glm::vec3(-1, -1, 1), glm::vec2(1, 0), glm::vec3(-1, 0, 0)),
    Vertex(glm::vec3(-1, 1, 1), glm::vec2(0, 0), glm::vec3(-1, 0, 0)),
    Vertex(glm::vec3(-1, 1, -1), glm::vec2(0, 1), glm::vec3(-1, 0, 0)),

    Vertex(glm::vec3(1, -1, -1), glm::vec2(1, 1), glm::vec3(1, 0, 0)),
    Vertex(glm::vec3(1, -1, 1), glm::vec2(1, 0), glm::vec3(1, 0, 0)),
    Vertex(glm::vec3(1, 1, 1), glm::vec2(0, 0), glm::vec3(1, 0, 0)),
    Vertex(glm::vec3(1, 1, -1), glm::vec2(0, 1), glm::vec3(1, 0, 0)),
};

unsigned int indices[] = { 0, 1, 2,
                          0, 2, 3,

                          6, 5, 4,
                          7, 6, 4,

                          10, 9, 8,
                          11, 10, 8,

                          12, 13, 14,
                          12, 14, 15,

                          16, 17, 18,
                          16, 18, 19,

                          22, 21, 20,
                          23, 22, 20
};

    Mesh mesh(vertices, sizeof(vertices) / sizeof(vertices[0]), indices, sizeof(indices) / sizeof(indices[0]));
    Mesh monkey("./res/monkey3.obj");
    Shader shader("./res/basicShader");
    Texture texture("./res/filtrer2.png");
    Transform transform;
    Camera camera(glm::vec3(0.0f, 0.0f, -2.0f), 70.0f, (float)DISPLAY_WIDTH / (float)DISPLAY_HEIGHT, 0.1f, 100.0f);
    Controls controls(&window, &camera);

    bool isRunning = true;
    float counter = 0.0f;
    sf::Event event;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);



    while (isRunning)
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // end the program
                isRunning = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // adjust the viewport when the window is resized
                glViewport(0, 0, event.size.width, event.size.height);
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape)
                    isRunning = false;
                break;
            }
        }

        //making sure context is active
        window.setActive();

        // clear the buffers
        glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //run controls update method
        controls.Update(transform, event);

        //get intersections with mouse ray
        std::vector<int> intersectionIndeces;
        std::vector<glm::vec3> intersections =  mesh.GetMouseIntersections(camera.pos, controls.GetMouseRay(), intersectionIndeces, transform);
       
        //chose the closest hit
        float closestDistance = 0;
        int closestIntersectionIndex = 0;
        int index = 0;
        for (int i = 0; i < intersections.size(); i++) {
            if (glm::distance(intersections[i], camera.pos) < closestDistance || i == 0) {
                closestIntersectionIndex = intersectionIndeces[i];
                index = i;
            }
        }

        glm::vec2 texture_position;
        if (intersections.size() > 0) {
            mesh.CalculateTexturePosition(intersections[index], closestIntersectionIndex, transform);
        }

        //DEBUG
        //std::cout << intersections.size() << std::endl;

        shader.Bind();
        texture.Bind();
        shader.Update(transform, camera);

        mesh.Draw();

        window.display();
        sf::sleep(sf::seconds(0.01f));
    }

    window.close();

    return 0;
}
