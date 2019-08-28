#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 ourColor; // output a color to the fragment shader
out vec2 texCoord;
out float instance_id;

void main()
{
  vec3 pos = aPos;
  pos.x = pos.x + gl_InstanceID * 1.5 - 5.5;
  pos = pos * 5.0;
  gl_Position = projection * view * model * vec4(pos, 1.0);
  ourColor = aColor; // set ourColor to the input color we got from the vertex data
  texCoord = vec2(aTexCoord.x, aTexCoord.y);
  instance_id = gl_InstanceID;
}
