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
// ȫ�ֱ���
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// Բ�εĶ�������
const int num_segments = 100;  // Բ�ηֶ���������Բ��ƽ���ȣ�

// ������Բ�Ķ���
// ������Բ�Ķ��㣬�������ĵ�
// ������Բ�Ķ��㣬�������ĵ㣬����ϵ��scaleX, scaleY
void generateEllipseVertices(float* vertices, float centerX, float centerY, float radiusX, float radiusY, float scaleX, float scaleY, int num_segments) {
    // ������Բ������
    vertices[0] = centerX;
    vertices[1] = centerY;

    // ������Բ����
    for (int i = 0; i <= num_segments; ++i) {
        float theta = 2.0f * M_PI * float(i) / float(num_segments);  // ���㵱ǰ�Ƕ�
        float x = centerX + radiusX * scaleX * cosf(theta);  // �� x ����İ뾶Ϊ radiusX * scaleX
        float y = centerY + radiusY * scaleY * sinf(theta);  // �� y ����İ뾶Ϊ radiusY * scaleY
        vertices[2 * (i + 1)] = x;  // ���¶������� x
        vertices[2 * (i + 1) + 1] = y;  // ���¶������� y
    }
}




// ������ɫ������ʼ������
void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("vertShader.glsl", "fragShader.glsl");

    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);

    /// Բ��
    /// 
    /// 
    ///
    // ����Բ�ζ�������
    float vertices[2 * (num_segments + 2)];  // Բ�� + ����
    generateEllipseVertices(vertices, -0.5f, 0.25f, 0.5f, 0.5f, 1, 1, num_segments);
    // ����Բ����ɫ����
    float vertexColors[4 * (num_segments + 2)];
    for (int i = 0; i < (num_segments + 2); ++i) {
        vertexColors[4 * i] = 1.0f;   // ��ɫ
        vertexColors[4 * i + 1] = 1.0f; // ��ɫ
        vertexColors[4 * i + 2] = 1.0f; // ��ɫ
        vertexColors[4 * i + 3] = 1.0f; // alpha
    }

    // ��VAO
    glBindVertexArray(vao[0]);

    // ���ض������ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // �����������ݸ�������ɫ���е�vPosition
    GLuint vPositionLoc = glGetAttribLocation(renderingProgram, "vPosition");
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // ������ɫ���ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

    // ������ɫ���ݸ�������ɫ���е�vColor
    GLuint vColorLoc = glGetAttribLocation(renderingProgram, "vColor");
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);




    ///
    /// ��Բ��
    /// 
    // ������Բ��������
    float tuoyuanvertices[2 * (num_segments + 2)];  // ���ĵ� + ��Բ����
    generateEllipseVertices(tuoyuanvertices, -0.5f, 0.25f, 0.1f, 0.35f, 1, 1, num_segments);  // ������0.7���̰���0.4  // ������Ϊ0.7���̰���Ϊ0.4

    // ������ɫ����
    float tuoyuanvertexColors[4 * (num_segments + 2)];
    for (int i = 0; i < (num_segments + 2); ++i) {
        tuoyuanvertexColors[4 * i] = 0.0f;   // ��ɫ
        tuoyuanvertexColors[4 * i + 1] = 0.0f; // ��ɫ
        tuoyuanvertexColors[4 * i + 2] = 0.0f; // ��ɫ
        tuoyuanvertexColors[4 * i + 3] = 1.0f; // alpha
    }

    // ��VAO
    glBindVertexArray(vao[1]);

    // ���ض������ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertices), tuoyuanvertices, GL_STATIC_DRAW);

    // �����������ݸ�������ɫ���е�vPosition
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // ������ɫ���ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertexColors), tuoyuanvertexColors, GL_STATIC_DRAW);

    // ������ɫ���ݸ�������ɫ���е�vColor
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);




    //Բ
    generateEllipseVertices(vertices, 0.5f, 0.25f, 0.5f, 0.5f, 1, 1, num_segments);

    // ��VAO
    glBindVertexArray(vao[2]);

    // ���ض������ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // �����������ݸ�������ɫ���е�vPosition
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // ������ɫ���ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);

    // ������ɫ���ݸ�������ɫ���е�vColor
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);


    //��Բ
    generateEllipseVertices(tuoyuanvertices, 0.5f, 0.25f, 0.1f, 0.35f, 1, 1, num_segments);  // ������0.7���̰���0.4  // ������Ϊ0.7���̰���Ϊ0.4

    // ��VAO
    glBindVertexArray(vao[3]);

    // ���ض������ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertices), tuoyuanvertices, GL_STATIC_DRAW);

    // �����������ݸ�������ɫ���е�vPosition
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // ������ɫ���ݵ�GPU
    glBindBuffer(GL_ARRAY_BUFFER, vbo[7]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tuoyuanvertexColors), tuoyuanvertexColors, GL_STATIC_DRAW);

    // ������ɫ���ݸ�������ɫ���е�vColor
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);


    //������
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
    // �� VAO
    glBindVertexArray(vao[4]);

    // Load the data into the GPU  
    glBindBuffer(GL_ARRAY_BUFFER, vbo[8]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trivertices), trivertices, GL_STATIC_DRAW);
    //��VBO������������ɫ������Ӧ�Ķ�������
    glVertexAttribPointer(vPositionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vPositionLoc);

    // Load the data into the GPU  
    glBindBuffer(GL_ARRAY_BUFFER, vbo[9]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(trivertexColors), trivertexColors, GL_STATIC_DRAW);
    //��VBO������������ɫ������Ӧ�Ķ�������
    glVertexAttribPointer(vColorLoc, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vColorLoc);
}

// ���ƺ���
void display(GLFWwindow* window, double currentTime)
{
    glUseProgram(renderingProgram);

    glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // ��������ϵ����ʹ�� sin �������������Ա仯

    float scaleX = (sin(currentTime) > 0) ? 1.0f + 2.5f * sin(currentTime) : 1.0f + 2.5f * -sin(currentTime);  // X ���������ϵ���� 0 �� 1 ֮��
    float scaleY = 1.0f;//0.5f + 0.5f * cos(currentTime);  // Y ���������ϵ���� 0 �� 1 ֮��

    // ��̬������Բ��������
    float vertices[2 * (num_segments + 2)];
    generateEllipseVertices(vertices, -0.5f, 0.25f, 0.1f, 0.35f, scaleX, scaleY, num_segments);
    // ���� ��һ����Բ VBO �еĶ�������
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    generateEllipseVertices(vertices, 0.5f, 0.25f, 0.1f, 0.35f, scaleX, scaleY, num_segments);
    // ���� ��һ����Բ VBO �еĶ�������
    glBindBuffer(GL_ARRAY_BUFFER, vbo[6]);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

    // ��������Բ��
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);
    glBindVertexArray(vao[2]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);


    // ����������Բ
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);
    glBindVertexArray(vao[3]);
    glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments + 2);

    //����������
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
    // glfw ��ʼ��
    if (!glfwInit())
    {
        exit(EXIT_FAILURE);
    }

    // ���ڰ汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // ��������
    GLFWwindow* window = glfwCreateWindow(800, 800, "Circle Example", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)  // glew ��ʼ��
    {
        exit(EXIT_FAILURE);
    }
    glfwSwapInterval(1);

    // ���ڴ�С�ı�Ļص�����
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // ������ɫ���������Ͱ�VAO��VBO
    init(window);

    // �¼�ѭ��
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
