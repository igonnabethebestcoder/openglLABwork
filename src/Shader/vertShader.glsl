#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normal; // ��ӷ�������
out vec2 tc;
out vec3 fragNormal; // ���ڴ��ݸ�Ƭ����ɫ��

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

void main(void)
{
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);
    tc = tex_coord;
    fragNormal = normal; // ���ݷ���
}