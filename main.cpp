#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <string>
#include <cmath>
#include "Utils.h"
using namespace std;



#define numVAOs 13
#define numVBOs 26
#define M_PI 3.1415926
// 全局变量
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// 圆形的顶点数据
const int num_segments = 100;  // 圆形分段数（决定圆的平滑度）

// 生成椭圆的顶点
// 生成椭圆的顶点，传入中心点
// 生成椭圆的顶点，传入中心点，缩放系数scaleX, scaleY
void generateEllipseVertices(float* vertices, float centerX, float centerY, float radiusX, float radiusY, float scaleX, float scaleY, int num_segments) {
    // 设置椭圆的中心
    vertices[0] = centerX;
    vertices[1] = centerY;

    // 生成椭圆顶点
    for (int i = 0; i <= num_segments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);  // 计算当前角度
        float x = centerX + radiusX * scaleX * cosf(theta);  // 在 x 方向的半径为 radiusX * scaleX
        float y = centerY + radiusY * scaleY * sinf(theta);  // 在 y 方向的半径为 radiusY * scaleY
        vertices[2 * (i + 1)] = x;  // 更新顶点坐标 x
        vertices[2 * (i + 1) + 1] = y;  // 更新顶点坐标 y
    }
}




// 导入着色器，初始化窗口
void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);

    /// 圆形
    /// 
    /// 
    ///
    // 生成圆形顶点数据
    float vertices[2 * (num_segments + 2)];  // 圆心 + 顶点
    generateEllipseVertices(vertices, -0.5f, 0.25f, 0.5f, 0.5f, 1, 1, num_segments);
    // 生成圆的颜色数据
    float vertexColors[4 * (num_segments + 2)];
    for (int i = 0; i < (num_segments + 2); ++i) {
        vertexColors[4 * i] = 1.0f;   // 红色
        vertexColors[4 * i + 1] = 1.0f; // 绿色
        vertexColors[4 * i + 2] = 1.0f; // 蓝色
        vertexColors[4 * i + 3] = 1.0f; // alpha
    }

    // 绑定VAO
    glBindVertexArray(vao[0]);

    // 加载顶点数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 关联顶点数据给顶点着色器中的vPosition
    GLuint vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // 加载颜色数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

    // 关联颜色数据给顶点着色器中的vColor
    GLuint vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);




    ///
    /// 椭圆形
    /// 
    // 生成椭圆顶点数据
    float tuoyuanvertices[2 * (num_segments + 2)];  // 中心点 + 椭圆顶点
    generateEllipseVertices(tuoyuanvertices, -0.5f, 0.25f, 0.1f, 0.35f, 1, 1, num_segments);  // 长半轴0.7，短半轴0.4  // 长半轴为0.7，短半轴为0.4

    // 生成颜色数据
    float tuoyuanvertexColors[4 * (num_segments + 2)];
    for (int i = 0; i < (num_segments + 2); ++i) {
        tuoyuanvertexColors[4 * i] = 0.0f;   // 红色
        tuoyuanvertexColors[4 * i + 1] = 0.0f; // 绿色
        tuoyuanvertexColors[4 * i + 2] = 0.0f; // 蓝色
        tuoyuanvertexColors[4 * i + 3] = 1.0f; // alpha
    }

    // 绑定VAO
    glBindVertexArray(vao[1]);

    // 加载顶点数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertices), tuoyuanvertices, GL_STATIC_DRAW);

    // 关联顶点数据给顶点着色器中的vPosition
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // 加载颜色数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertexColors), tuoyuanvertexColors, GL_STATIC_DRAW);

    // 关联颜色数据给顶点着色器中的vColor
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);




    //圆
    generateEllipseVertices(vertices, 0.5f, 0.25f, 0.5f, 0.5f, 1, 1, num_segments);

    // 绑定VAO
    glBindVertexArray(vao[2]);

    // 加载顶点数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 关联顶点数据给顶点着色器中的vPosition
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // 加载颜色数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

    // 关联颜色数据给顶点着色器中的vColor
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);


    //椭圆
    generateEllipseVertices(tuoyuanvertices, 0.5f, 0.25f, 0.1f, 0.35f, 1, 1, num_segments);  // 长半轴0.7，短半轴0.4  // 长半轴为0.7，短半轴为0.4

    // 绑定VAO
    glBindVertexArray(vao[3]);

    // 加载顶点数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertices), tuoyuanvertices, GL_STATIC_DRAW);

    // 关联顶点数据给顶点着色器中的vPosition
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // 加载颜色数据到GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertexColors), tuoyuanvertexColors, GL_STATIC_DRAW);

    // 关联颜色数据给顶点着色器中的vColor
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);


    //三角形
    float trivertices[6] = {
        -0.25f, -0.25f,
        0.0f, -0.0f,
        0.25f, -0.25f
    };

    float trivertexColors[12] = {
        0.0f, 1.0f, 0.0f, 1.0f,  // red
        0.0f, 1.0f, 0.0f, 1.0f,  // green
        0.0f, 1.0f, 0.0f, 1.0f   // blue
    };
    // 绑定 VAO
    glBindVertexArray(vao[4]);

    // Load the data into the GPU  
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trivertices), trivertices, GL_STATIC_DRAW);
    //将VBO关联给顶点着色器中相应的顶点属性
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // Load the data into the GPU  
    glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trivertexColors), trivertexColors, GL_STATIC_DRAW);
    //将VBO关联给顶点着色器中相应的顶点属性
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);
}

// 绘制函数
void display(GLFWwindow* window, double currentTime)
{
    glUseProgram(renderingProgram);

    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 计算缩放系数，使用 sin 函数生成周期性变化

    float scaleX = (sin(currentTime) > 0) ? 1.0f + 2.5f * sin(currentTime) : 1.0f + 2.5f * -sin(currentTime);  // X 方向的缩放系数在 0 到 1 之间
    float scaleY = 1.0f;//0.5f + 0.5f * cos(currentTime);  // Y 方向的缩放系数在 0 到 1 之间

    // 动态生成椭圆顶点数据
    float vertices[2 * (num_segments + 2)];
    generateEllipseVertices(vertices, -0.5f, 0.25f, 0.1f, 0.35f, scaleX, scaleY, num_segments);
    // 更新 第一个椭圆 VBO 中的顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    generateEllipseVertices(vertices, 0.5f, 0.25f, 0.1f, 0.35f, scaleX, scaleY, num_segments);
    // 更新 第一个椭圆 VBO 中的顶点数据
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // 绘制两个圆形
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);
    glBindVertexArray(vao[2]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);


    // 绘制两个椭圆
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);
    glBindVertexArray(vao[3]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);

    //绘制三角形
    glBindVertexArray(vao[4]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

#define OLDRUN
#ifdef OLDRUN
int main(void)
{
    // glfw 初始化
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // 窗口版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 800, "Circle Example", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)  // glew 初始化
    {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    // 窗口大小改变的回调函数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 导入着色器，创建和绑定VAO和VBO
    init(window);

    // 事件循环
    while (!glfwWindowShouldClose(window))
    {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
#endif // OLDRUN
