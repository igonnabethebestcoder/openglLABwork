#version 330 core

layout(lines) in; // �����߶�
layout(triangle_strip, max_vertices = 4) out; // ����ĸ�����

out vec4 fragColor; // �����ɫ����

uniform float lineWidth; // �߿�

void main()
{
    vec2 offset = vec2(lineWidth / 2.0, 0.0);

    // �����߶ε������˵�
    vec2 p1 = gl_in[0].gl_Position.xy; // ��һ���˵�
    vec2 p2 = gl_in[1].gl_Position.xy; // �ڶ����˵�

    // ���㷨����
    vec2 direction = normalize(p2 - p1);
    vec2 normal = vec2(-direction.y, direction.x);

    // �����߿�Ķ���
    gl_Position = vec4(p1 + normal * lineWidth, 0.0, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0); // ��ɫ
    EmitVertex();

    gl_Position = vec4(p2 + normal * lineWidth, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p1 - normal * lineWidth, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p2 - normal * lineWidth, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
