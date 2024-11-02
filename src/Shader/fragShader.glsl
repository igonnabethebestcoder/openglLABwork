#version 430

in vec2 tc;
in vec3 fragNormal; // ���շ���
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D s;
uniform vec3 diffuseColor;

void main(void)
{
    vec4 texColor = texture(s, tc);
    
    // ���������ɫ�Ƿ�Ϊ��ɫ�����Ը�����Ҫ������ɫ����ֵ��
    if (texColor.rgb != vec3(0.0, 0.0, 0.0)) {
        // ������Ǻ�ɫ��Ӧ��������ɫ
        color = texColor ;//* vec4(diffuseColor, 1.0);
    } else {
        // ����Ǻ�ɫ��ʹ����������ɫ
        color = vec4(diffuseColor, 1.0);
    }

    // ���������ӹ��ռ��㣬ʾ��ʹ�ü򵥵�������
    //vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // �����Դ����
    //float diff = max(dot(fragNormal, lightDir), 0.0); // �������������
    //color *= diff; // ��������������
}
