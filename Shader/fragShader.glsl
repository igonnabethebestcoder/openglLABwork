#version 430

in vec2 tc;
in vec3 fragNormal; // 接收法线
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D s;
uniform vec3 diffuseColor;

void main(void)
{
    vec4 texColor = texture(s, tc);
    // 这里可以添加光照计算，示例使用简单的漫反射
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // 假设光源方向
    float diff = 1;//光照参数
    color =vec4(diffuseColor * diff, 1.0);// texColor * vec4(diffuseColor * diff, 1.0);
}
