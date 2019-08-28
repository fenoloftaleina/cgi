#include "textures.hpp"

unsigned int gl_texture()
{
  unsigned int texture_id;
  glGenTextures(1, &texture_id);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  return texture_id;
}

void Textures::prepare()
{
  int width, height, nrChannels;
  unsigned char* image;
  unsigned char* texture = new unsigned char[texture_size * texture_size * 4]{};
  unsigned int texture_id = gl_texture();

  stbi_set_flip_vertically_on_load(true);
  mappings.reserve(resources_pathnames.size());

  int i = 0, j = 0, max_line_height = 0;

  for(auto& pathname : resources_pathnames) {
    printf("loop\n");

    image = stbi_load(pathname.c_str(), &width, &height, &nrChannels, 0);

    printf("%d %d\n", width, height);

    for(int m = 0; m < height; ++m) {

    printf("%d %d %d %d\n", m, height, i, j);

      memcpy(texture + i * 4 + j * texture_size * 4 + m * texture_size * 4, image + m * width * 4, width * 4);
    }

    Textures::Mapping mapping{};
    mapping.x1 = i / (float)texture_size;
    mapping.y1 = j / (float)texture_size;
    mapping.x2 = (i + width) / (float)texture_size;
    mapping.y2 = (j + height) / (float)texture_size;

    mapping.texture_id = texture_id;
    mapping.texture_data = texture;

    mappings.push_back(mapping);

    if (height > max_line_height) {
      max_line_height = height;
    }

    i += width + 1;
    if (i >= 1024) {
      i = 0;
      j = max_line_height + 1;
    }

    stbi_image_free(image);
  }

  // for(int j = 0; j < height; j += 2) {
  //   for(int i = 0; i < width * 4; ++i) {
  //     a[j * width * 4 + i] = data[j * width * 4 + i];
  //     a[(j + 1) * width * 4 + i] = data1[j * width * 4 + i];
  //   }
  // }

  // unsigned char *a = new unsigned char[width * height * 4];
  // for(int i = 0; i < width * height * 4; ++i) {
  //   a[i] = data[i];
  // }

  printf("post loop\n");

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture);
  glGenerateMipmap(GL_TEXTURE_2D);
}
