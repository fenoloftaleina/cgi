// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cstring>

float randf() {
  return rand() / (float)RAND_MAX;
}

int main() {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto mWindow = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (mWindow == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(mWindow);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    const int n = 30000;
    float vertices[n * 3];
    for(int i = 0; i < n; ++i) {
      vertices[3 * i] = randf();
      vertices[3 * i + 1] = randf();
      vertices[3 * i + 2] = randf();
      // vertices[6 * i + 3] = randf() * 0.5f;
      // vertices[6 * i + 4] = 0.1f + 0.1f * randf();
      // vertices[6 * i + 5] = 0.1f + 0.9f * randf();
    }

    const char* vertexSource = R"glsl(
      #version 330 core
      layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
      // layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

      uniform mat4 model;
      uniform mat4 view;
      uniform mat4 projection;

      out vec3 ourColor; // output a color to the fragment shader

      void main()
      {
          gl_Position = projection * view * model * vec4(aPos, 1.0);
          // ourColor = aColor; // set ourColor to the input color we got from the vertex data
          ourColor = gl_Position.xyz; //vec3(0.1, 0.3, 0.8);
      }
    )glsl";

    const char* fragmentSource = R"glsl(
      #version 330 core

      in vec3 ourColor;
      out vec4 outColor;

      uniform float t;
      uniform float shade;

      void main()
      {
          outColor = vec4(ourColor * shade + 0.2, 1.0);
      }
    )glsl";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    char buffer[512];
    int success;

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
      fprintf(stderr, "Vertex shader info log:\n%s\n", buffer);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, 512, NULL, buffer);
      fprintf(stderr, "Fragment shader info log:\n%s\n", buffer);
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);

    unsigned int vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    // glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    // glEnableVertexAttribArray(1);

    int tUniform = glGetUniformLocation(shaderProgram, "t");
    int shadeUniform = glGetUniformLocation(shaderProgram, "shade");
    int modelUniform = glGetUniformLocation(shaderProgram, "model");
    int viewUniform = glGetUniformLocation(shaderProgram, "view");
    int projectionUniform = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, -0.5f, -1.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), mWidth / (float) mHeight, 0.1f, 100.0f);

    float prev_t, dt;
    float t = 0.0f;

    int fps_frame = 0;
    float fps_prev_t = glfwGetTime();

    // Rendering Loop
    while (glfwWindowShouldClose(mWindow) == false) {
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(mWindow, true);

        prev_t = t;
        t = glfwGetTime();
        dt = t - prev_t;

        ++fps_frame;
        if (fps_frame == 200) {
          printf("%f fps\n", 200 / (t - fps_prev_t));
          fps_prev_t = t;
          fps_frame = 0;
        }

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(tUniform, t);
        glUniform1f(shadeUniform, 0.9f);
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

        view = glm::rotate(view, glm::radians(10.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));

        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, n);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

/*
 * Display FPS somewhere - BROKEN?
 *
 * Face culling or something?
 * Check if laying out the triangles differently changes anything
 * Read the perf stuff.
 * Try instancing?
 * "We got an OpenGL extension or two out of it at least. Use renderdoc and apitrace."
 *
 * Try some camera movement animation
 * Try doing a generic walking anywhere camera
 * Do some shapes finally
 * Try some textures
 * Build some API? Orginize the code a bit
 * Simple string to tiles + 2d platformer?
 */
