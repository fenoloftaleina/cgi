#version 330 core

in vec3 ourColor;
in vec2 texCoord;
in float instance_id;
out vec4 outColor;

uniform float t;
uniform vec2 res;

uniform sampler2D texture1;

void main()
{
  //outColor = vec4(ourColor * gl_FragCoord.xyz / vec3(res, 1.0), 1.0);
  // outColor = texture(texture1, vec2(texCoord.x + instance_id * 0.1 + t * 0.1, texCoord.y)) + vec4(0.1, 0.1, 0.1, 1.0);
  outColor = texture(texture1, texCoord);
}
