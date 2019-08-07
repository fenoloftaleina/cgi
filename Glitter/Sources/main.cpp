// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

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

    int s = 100;

    const int n = 3 * s * s * s;
    float *vertices = new float[n * 3];

    float dn = 1 / (float)s;

    float z_mult = 50.0f;

    float x, y, z;
    x = -0.5f;
    for(int i = 0; i < s; ++i) {
      y = -0.5f;
      for(int j = 0; j < s; ++j) {
        z = -0.5f;
        for(int k = 0; k < s; ++k) {
          int p = 9 * (i * s * s + j * s + k);
          vertices[p] = x;
          vertices[p + 1] = y;
          vertices[p + 2] = z * z_mult;

          vertices[p + 3] = x + dn * 0.5f;
          vertices[p + 4] = y;
          vertices[p + 5] = z * z_mult;

          vertices[p + 6] = x;
          vertices[p + 7] = y + dn * 0.5f;
          vertices[p + 8] = z * z_mult;

          z += dn;
        }
        y += dn;
      }
      x += dn;
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
          ourColor = gl_Position.xyz;
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
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, n * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

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
    // model = glm::rotate(model, glm::radians(-5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::scale(model, glm::vec3(2.0f, 2.0f, 1.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), mWidth / (float) mHeight, 0.1f, 100.0f);

    float prev_t, dt = 0.0f;
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
          printf("%f - %f = %f, %f fps\n", t, fps_prev_t, (t - fps_prev_t), 200 / (t - fps_prev_t));
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

        view = glm::rotate(view, glm::radians(sin(t) * 5.0f * dt), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, dt));
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
 * Check if laying out the triangles differently changes anything
 * Try instancing?
 *
 * Try some camera movement animation
 * Try doing a generic walking anywhere camera
 * Do some shapes finally
 * Try some textures
 * Build some API? Orginize the code a bit
 * Simple string to tiles + 2d platformer?
 */
