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

glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

float prev_t, dt;
float t = 0.0f;

int fps_frame = 0;
float fps_prev_t = glfwGetTime();

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
  {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
  lastX = xpos;
  lastY = ypos;

  float sensitivity = 0.1f; // change this value to your liking
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);
}

void process_input(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 2.5 * dt;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

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
    glfwSetCursorPosCallback(mWindow, mouse_callback);
    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    gladLoadGL();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.1f, 0.2f, 0.3f,
      -0.5f,  0.5f, -0.5f, 0.1f, 0.5f, 0.7f,
      0.5f,  0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
      0.5f, -0.5f, -0.5f,  0.4f, 0.0f, 0.5f,

      -0.5f, -0.5f,  0.5f, 0.1f, 0.2f, 0.3f,
      0.5f, -0.5f,  0.5f,  0.4f, 0.0f, 0.5f,
      0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
      -0.5f,  0.5f,  0.5f, 0.1f, 0.5f, 0.7f,

      -0.5f,  0.5f,  0.5f, 0.1f, 0.2f, 0.3f,
      -0.5f,  0.5f, -0.5f, 0.4f, 0.0f, 0.5f,
      -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f,
      -0.5f, -0.5f,  0.5f, 0.1f, 0.5f, 0.7f,

      0.5f,  0.5f,  0.5f,  0.1f, 0.2f, 0.3f,
      0.5f, -0.5f,  0.5f,  0.1f, 0.5f, 0.7f,
      0.5f, -0.5f, -0.5f,  0.5f, 0.5f, 0.5f,
      0.5f,  0.5f, -0.5f,  0.4f, 0.0f, 0.5f,

      -0.5f, -0.5f, -0.5f, 0.1f, 0.2f, 0.3f,
      0.5f, -0.5f, -0.5f,  0.4f, 0.0f, 0.5f,
      0.5f, -0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
      -0.5f, -0.5f,  0.5f, 0.1f, 0.5f, 0.7f,

      -0.5f,  0.5f, -0.5f, 0.1f, 0.2f, 0.3f,
      -0.5f,  0.5f,  0.5f, 0.1f, 0.5f, 0.7f,
      0.5f,  0.5f,  0.5f,  0.5f, 0.5f, 0.5f,
      0.5f,  0.5f, -0.5f,  0.4f, 0.0f, 0.5f
    };


    const int n = 36;
    unsigned int indices[n];

    for(int i = 0; i < n / 6; ++i) {
      indices[i * 6] = i * 4;
      indices[i * 6 + 1] = i * 4 + 1;
      indices[i * 6 + 2] = i * 4 + 2;
      indices[i * 6 + 3] = i * 4 + 2;
      indices[i * 6 + 4] = i * 4 + 3;
      indices[i * 6 + 5] = i * 4;
    }

    const char* vertexSource = R"glsl(
      #version 330 core
      layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
      layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

      uniform mat4 model;
      uniform mat4 view;
      uniform mat4 projection;

      out vec3 ourColor; // output a color to the fragment shader

      void main()
      {
          vec3 pos = aPos;
          pos.x = pos.x + mod(gl_InstanceID * 1.1, 100.0);
          pos.y = mod(pos.x * pos.x + 0.1, 10.0);
          pos.y = pos.y + (gl_InstanceID - mod(gl_InstanceID * 1.1, 100.0)) * 0.07 * mod(sin(pos.x) * 123.0, 27.0) * 0.006;
          gl_Position = projection * view * model * vec4(pos, 1.0);
          ourColor = aColor; // set ourColor to the input color we got from the vertex data
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
          outColor = vec4(ourColor * shade, 1.0);
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
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    int tUniform = glGetUniformLocation(shaderProgram, "t");
    int shadeUniform = glGetUniformLocation(shaderProgram, "shade");
    int modelUniform = glGetUniformLocation(shaderProgram, "model");
    int viewUniform = glGetUniformLocation(shaderProgram, "view");
    int projectionUniform = glGetUniformLocation(shaderProgram, "projection");

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, -1.0f, 0.0f));
    float scale = 0.05;
    model = glm::scale(model, glm::vec3(scale, scale, scale));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), mWidth / (float) mHeight, 0.1f, 100.0f);

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

        process_input(mWindow);

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform1f(tUniform, t);
        glUniform1f(shadeUniform, 0.9f);
        glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

        // view = glm::rotate(view, glm::radians(dt * 100.0f * rand() / RAND_MAX), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(vao);
        // glDrawElements(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0);
        glDrawElementsInstanced(GL_TRIANGLES, n, GL_UNSIGNED_INT, 0, 1000);
        // glDrawArrays(GL_TRIANGLES, 0, n);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }   glfwTerminate();
    return EXIT_SUCCESS;
}

/*
 * Try some camera movement animation
 * Try doing a generic walking anywhere camera
 * Do some shapes finally
 * Try some textures
 * Build some API? Orginize the code a bit
 * Simple string to tiles + 2d platformer?
 */
