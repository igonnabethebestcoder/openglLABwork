#version 330 core

layout(lines) in; // 输入线段
layout(triangle_strip, max_vertices = 4) out; // 输出四个顶点

out vec4 fragColor; // 输出颜色变量

uniform float lineWidth; // 线宽

void main()
{
    vec2 offset = vec2(lineWidth / 2.0, 0.0);

    // 计算线段的两个端点
    vec2 p1 = gl_in[0].gl_Position.xy; // 第一个端点
    vec2 p2 = gl_in[1].gl_Position.xy; // 第二个端点

    // 计算法向量
    vec2 direction = normalize(p2 - p1);
    vec2 normal = vec2(-direction.y, direction.x);

    // 发射线宽的顶点
    gl_Position = vec4(p1 + normal * lineWidth, 0.0, 1.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0); // 红色
    EmitVertex();

    gl_Position = vec4(p2 + normal * lineWidth, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p1 - normal * lineWidth, 0.0, 1.0);
    EmitVertex();

    gl_Position = vec4(p2 - normal * lineWidth, 0.0, 1.0);
    EmitVertex();

    EndPrimitive();
}
