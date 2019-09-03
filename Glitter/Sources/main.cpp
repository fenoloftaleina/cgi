#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <string>

#include "buffer.hpp"
#include "gl.hpp"
#include "glitter.hpp"
#include "input.hpp"
#include "shaders.hpp"
#include "textures.hpp"
#include "utils.hpp"

Buffer b;

Gl gl;
Input input;
Shaders shaders;
Textures textures;
Utils utils;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  input.mouse_callback(window, xpos, ypos);
}

int main() {
  gl.basic_init(mouse_callback);

  textures.prepare();
  shaders.utils = utils;
  shaders.prepare();

  float prev_t, dt;
  float t = 0.0f;
  int fps_frame = 0;
  float fps_prev_t = glfwGetTime();

  unsigned int shaderProgram = shaders.use(0);
  int tUniform = glGetUniformLocation(shaderProgram, "t");
  int resUniform = glGetUniformLocation(shaderProgram, "res");
  int modelUniform = glGetUniformLocation(shaderProgram, "model");
  int viewUniform = glGetUniformLocation(shaderProgram, "view");
  int projectionUniform = glGetUniformLocation(shaderProgram, "projection");
  int texture1Uniform = glGetUniformLocation(shaderProgram, "texture1");

  glUniform2f(resUniform, gl.width, gl.height);

  glm::mat4 model = glm::mat4(1.0f);
  // model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, -1.0f, 0.0f));
  float scale = 0.05;
  model = glm::scale(model, glm::vec3(scale, scale, scale));

  glm::mat4 view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), gl.width / (float) gl.height, 0.1f, 100.0f);

  b.gen_plane();
  Textures::Mapping &m = textures.mappings[1];
  b.set_plane_tex_coords(0, m);
  b.setup();

  // Rendering Loop
  while (glfwWindowShouldClose(gl.window) == false) {
    if (glfwGetKey(gl.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(gl.window, true);

    prev_t = t;
    t = glfwGetTime();
    dt = t - prev_t;
    ++fps_frame;

    if (fps_frame == 200) {
      printf("%f - %f = %f, %f fps\n", t, fps_prev_t, (t - fps_prev_t), 200 / (t - fps_prev_t));
      fps_prev_t = t;
      fps_frame = 0;
    }

    input.process_input(gl.window, dt);

    // Background Fill Color
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // glBindTexture(GL_TEXTURE_2D, 0);

    glUniform1f(tUniform, t);
    glUniformMatrix4fv(modelUniform, 1, GL_FALSE, glm::value_ptr(model));

    // view = glm::rotate(view, glm::radians(dt * 100.0f * rand() / RAND_MAX), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 view = glm::lookAt(input.cameraPos, input.cameraPos + input.cameraFront, input.cameraUp);
    glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projection));

    // glUniform1i(texture1Uniform, 0);

    b.bind();

    Textures::Mapping &m = textures.mappings[(int)(t * 5) % 2];
    b.set_plane_tex_coords(0, m);
    b.update_vertices();

    b.draw();
    b.unbind();

    // Flip Buffers and Draw
    glfwSwapBuffers(gl.window);
    glfwPollEvents();
  }   glfwTerminate();
  return EXIT_SUCCESS;
}
