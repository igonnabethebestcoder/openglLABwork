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
    
    // 检查纹理颜色是否为黑色（可以根据需要调整黑色的阈值）
    if (texColor.rgb != vec3(0.0, 0.0, 0.0)) {
        // 如果不是黑色，应用纹理颜色
        color = texColor ;//* vec4(diffuseColor, 1.0);
    } else {
        // 如果是黑色，使用漫反射颜色
        color = vec4(diffuseColor, 1.0);
    }

    // 这里可以添加光照计算，示例使用简单的漫反射
    //vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // 假设光源方向
    //float diff = max(dot(fragNormal, lightDir), 0.0); // 计算漫反射光照
    //color *= diff; // 乘以漫反射因子
}
