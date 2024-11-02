#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL2/soil2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "./ImportedModel/ImportedModel.h"
#include "./Utils/Utils.h"
#include "./events/events.cpp"
using namespace std;

#define numVAOs 4
#define numVBOs 4  // 更新为4，因为我们增加了一个VBO用于马路
#define numEBOs 4

float cameraX, cameraY, cameraZ;
float objLocX, objLocY, objLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numEBOs];
GLuint shuttleTexture;
GLuint roadTexture; // 用于存储马路的纹理


//马路无限产生
const int numRoadSegments = 5; // 路面片的数量
const float roadSegmentLength = 20.0f; // 每个路面片的长度
float roadOffsets[numRoadSegments]; // 存储路面片的位置偏移
float roadSpeed = 0.1f; // 初始化路面更新速度

//车头转向
bool leftKeyPressed = false;  // 记录左箭头是否被按下
bool rightKeyPressed = false; // 记录右箭头是否被按下
int lefttimes = 0;//多次按下左时只转向一次
int righttimes = 0;

// variable allocation for display
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat;

ImportedModel myModel("./models/cylinder.obj");

float toRadians(float degrees)
{
    return (degrees * 2.0f * 3.14159f) / 360.0f;
}

//汽车左右移动键盘事件
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // 检查按键是否被按下
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) {
            //保留鬼畜特性
            if (roadSpeed > 0)
                objLocX -= 0.1f; // 向左移动
            leftKeyPressed = true;
            //++lefttimes;
        }
        if (key == GLFW_KEY_RIGHT) {
            if (roadSpeed > 0)
                objLocX += 0.1f; // 向右移动
            rightKeyPressed = true;
            //++righttimes;
        }
        if (key == GLFW_KEY_UP) {
            roadSpeed += 0.05f; // 加快路面更新速度

            // 更新汽车的上下位置（根据速度改变）
            objLocZ += roadSpeed * 0.5f; // 例如，上下移动速度为路面速度的一半
        }
        if (key == GLFW_KEY_DOWN) {
            roadSpeed = glm::max(0.0f, roadSpeed - 0.05f); // 减慢路面更新速度，不小于0

            // 更新汽车的上下位置（根据速度改变）
            objLocZ -= glm::max(-20.0f, roadSpeed * 0.5f); // 例如，上下移动速度为路面速度的一半
        }
    }

    // 检查按键是否释放
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) {
            leftKeyPressed = false; // 记录左键释放状态
            lefttimes = 0;//车头自动回正
        }
        if (key == GLFW_KEY_RIGHT) {
            rightKeyPressed = false; // 记录右键释放状态
            righttimes = 0;//车头自动回正
        }
    }
}

void setupVertices(void)
{
    std::vector<glm::vec3> vert = myModel.getVertices();
    std::vector<glm::vec2> tex = myModel.getTextureCoords();
    std::vector<glm::vec3> norm = myModel.getNormals();

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    // 生成vao和vbo
    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);
    glGenBuffers(numEBOs, ebo);

    for (int i = 0; i < myModel.getNumVertices(); i++) {
        pvalues.push_back((vert[i]).x);
        pvalues.push_back((vert[i]).y);
        pvalues.push_back((vert[i]).z);
        tvalues.push_back((tex[i]).s);
        tvalues.push_back((tex[i]).t);
        nvalues.push_back((norm[i]).x);
        nvalues.push_back((norm[i]).y);
        nvalues.push_back((norm[i]).z);
    }

    glBindVertexArray(vao[0]);

    // 顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // 纹理坐标缓冲
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // 法线缓冲
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // 马路顶点数据
    GLfloat roadVertices[] = {
        -5.0f, 0.0f, -10.0f, 0.0f, 0.0f, // 左下
         5.0f, 0.0f, -10.0f, 1.0f, 0.0f, // 右下
         5.0f, 0.0f,  10.0f, 1.0f, 1.0f, // 右上
        -5.0f, 0.0f,  10.0f, 0.0f, 1.0f  // 左上
    }; // x,y,z,u,v


    GLuint roadIndices[] = {
        0, 1, 2, // 第一个三角形
        0, 2, 3  // 第二个三角形
    };

    // 设置马路的VBO和EBO
    glBindVertexArray(vao[1]);

    // 马路顶点缓冲
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]); // 使用vbo[3]用于马路
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 马路纹理坐标缓冲
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // 马路索引缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roadIndices), roadIndices, GL_STATIC_DRAW);

    glBindVertexArray(0); // 解绑VAO
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("./Shader/vertShader.glsl", "./Shader/fragShader.glsl");

    // 启用深度测试
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); // 剔除背面

    cameraX = 0.0f; cameraY = 8.5f; cameraZ = 8.5f;
    objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    setupVertices();
    shuttleTexture = Utils::loadTexture("./models/shallowblue.jpg");
    roadTexture = Utils::loadTexture("./models/road.jpg"); //马路的纹理

    //初始化路面偏移
    for (int i = 0; i < numRoadSegments; ++i) {
        roadOffsets[i] = i * roadSegmentLength; // 初始化每个路面片的位置
    }

}

void display(GLFWwindow* window, double currentTime)
{
    glClear(GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderingProgram);

    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    //添加漫反射, 
    glUniform3f(glGetUniformLocation(renderingProgram, "diffuseColor"), 1.0f, 0.5f, 0.5f); // 例如设置为红色

    //vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    // 设置视图矩阵
    vMat = glm::lookAt(
        glm::vec3(cameraX, cameraY, cameraZ), // 摄像头位置
        glm::vec3(0.0f, 0.0f, 0.0f),           // 目标位置
        glm::vec3(0.0f, 1.0f, 0.0f)            // 上方向
    );
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));

    // 缩放模型
    mMat = glm::scale(mMat, glm::vec3(0.0095f, 0.0095f, 0.0095f));
    //mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));

    //y轴调转车头，原本模式车头反了
    mMat = glm::rotate(mMat, toRadians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    if (leftKeyPressed)
        mMat = glm::rotate(mMat, toRadians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    if (rightKeyPressed)
        mMat = glm::rotate(mMat, toRadians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    mvMat = vMat * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // 绘制模型
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shuttleTexture);
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());


    //绘制马路
    // 更新和绘制路面片
    for (int i = 0; i < numRoadSegments; ++i) {
        glm::mat4 roadMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, roadOffsets[i]));
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat * roadMat));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, roadTexture);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // 更新路面片的位置
    for (int i = 0; i < numRoadSegments; ++i) {
        roadOffsets[i] += roadSpeed; // 根据需要调整速度
        if (roadOffsets[i] > roadSegmentLength) { // 如果路面片移出视野
            roadOffsets[i] -= roadSegmentLength * numRoadSegments; // 将它移到最后面
        }
    }
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight)
{
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void)
{
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(1000, 1000, "ObjLoader", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);

    //窗口回调
    glfwSetWindowSizeCallback(window, window_size_callback);

    // 设置键盘回调
    glfwSetKeyCallback(window, key_callback); 

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
