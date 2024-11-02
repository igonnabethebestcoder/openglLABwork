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
#define numVBOs 4  // ����Ϊ4����Ϊ����������һ��VBO������·
#define numEBOs 4

float cameraX, cameraY, cameraZ;
float objLocX, objLocY, objLocZ;
GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLuint ebo[numEBOs];
GLuint shuttleTexture;
GLuint roadTexture; // ���ڴ洢��·������


//��·���޲���
const int numRoadSegments = 5; // ·��Ƭ������
const float roadSegmentLength = 20.0f; // ÿ��·��Ƭ�ĳ���
float roadOffsets[numRoadSegments]; // �洢·��Ƭ��λ��ƫ��
float roadSpeed = 0.1f; // ��ʼ��·������ٶ�

//��ͷת��
bool leftKeyPressed = false;  // ��¼���ͷ�Ƿ񱻰���
bool rightKeyPressed = false; // ��¼�Ҽ�ͷ�Ƿ񱻰���
int lefttimes = 0;//��ΰ�����ʱֻת��һ��
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

//���������ƶ������¼�
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    // ��鰴���Ƿ񱻰���
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_LEFT) {
            //������������
            if (roadSpeed > 0)
                objLocX -= 0.1f; // �����ƶ�
            leftKeyPressed = true;
            //++lefttimes;
        }
        if (key == GLFW_KEY_RIGHT) {
            if (roadSpeed > 0)
                objLocX += 0.1f; // �����ƶ�
            rightKeyPressed = true;
            //++righttimes;
        }
        if (key == GLFW_KEY_UP) {
            roadSpeed += 0.05f; // �ӿ�·������ٶ�

            // ��������������λ�ã������ٶȸı䣩
            objLocZ += roadSpeed * 0.5f; // ���磬�����ƶ��ٶ�Ϊ·���ٶȵ�һ��
        }
        if (key == GLFW_KEY_DOWN) {
            roadSpeed = glm::max(0.0f, roadSpeed - 0.05f); // ����·������ٶȣ���С��0

            // ��������������λ�ã������ٶȸı䣩
            objLocZ -= glm::max(-20.0f, roadSpeed * 0.5f); // ���磬�����ƶ��ٶ�Ϊ·���ٶȵ�һ��
        }
    }

    // ��鰴���Ƿ��ͷ�
    if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_LEFT) {
            leftKeyPressed = false; // ��¼����ͷ�״̬
            lefttimes = 0;//��ͷ�Զ�����
        }
        if (key == GLFW_KEY_RIGHT) {
            rightKeyPressed = false; // ��¼�Ҽ��ͷ�״̬
            righttimes = 0;//��ͷ�Զ�����
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

    // ����vao��vbo
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

    // ���㻺��
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), &pvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // �������껺��
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), &tvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // ���߻���
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), &nvalues[0], GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    // ��·��������
    GLfloat roadVertices[] = {
        -5.0f, 0.0f, -10.0f, 0.0f, 0.0f, // ����
         5.0f, 0.0f, -10.0f, 1.0f, 0.0f, // ����
         5.0f, 0.0f,  10.0f, 1.0f, 1.0f, // ����
        -5.0f, 0.0f,  10.0f, 0.0f, 1.0f  // ����
    }; // x,y,z,u,v


    GLuint roadIndices[] = {
        0, 1, 2, // ��һ��������
        0, 2, 3  // �ڶ���������
    };

    // ������·��VBO��EBO
    glBindVertexArray(vao[1]);

    // ��·���㻺��
    glBindBuffer(GL_ARRAY_BUFFER, vbo[3]); // ʹ��vbo[3]������·
    glBufferData(GL_ARRAY_BUFFER, sizeof(roadVertices), roadVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // ��·�������껺��
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // ��·��������
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(roadIndices), roadIndices, GL_STATIC_DRAW);

    glBindVertexArray(0); // ���VAO
}

void init(GLFWwindow* window)
{
    renderingProgram = Utils::createShaderProgram("./Shader/vertShader.glsl", "./Shader/fragShader.glsl");

    // ������Ȳ���
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK); // �޳�����

    cameraX = 0.0f; cameraY = 8.5f; cameraZ = 8.5f;
    objLocX = 0.0f; objLocY = 0.0f; objLocZ = 0.0f;

    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);

    setupVertices();
    shuttleTexture = Utils::loadTexture("./models/shallowblue.jpg");
    roadTexture = Utils::loadTexture("./models/road.jpg"); //��·������

    //��ʼ��·��ƫ��
    for (int i = 0; i < numRoadSegments; ++i) {
        roadOffsets[i] = i * roadSegmentLength; // ��ʼ��ÿ��·��Ƭ��λ��
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

    //���������, 
    glUniform3f(glGetUniformLocation(renderingProgram, "diffuseColor"), 1.0f, 0.5f, 0.5f); // ��������Ϊ��ɫ

    //vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));
    // ������ͼ����
    vMat = glm::lookAt(
        glm::vec3(cameraX, cameraY, cameraZ), // ����ͷλ��
        glm::vec3(0.0f, 0.0f, 0.0f),           // Ŀ��λ��
        glm::vec3(0.0f, 1.0f, 0.0f)            // �Ϸ���
    );
    mMat = glm::translate(glm::mat4(1.0f), glm::vec3(objLocX, objLocY, objLocZ));

    // ����ģ��
    mMat = glm::scale(mMat, glm::vec3(0.0095f, 0.0095f, 0.0095f));
    //mMat = glm::scale(mMat, glm::vec3(1.0f, 1.0f, 1.0f));

    //y���ת��ͷ��ԭ��ģʽ��ͷ����
    mMat = glm::rotate(mMat, toRadians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    if (leftKeyPressed)
        mMat = glm::rotate(mMat, toRadians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    if (rightKeyPressed)
        mMat = glm::rotate(mMat, toRadians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    mvMat = vMat * mMat;

    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    // ����ģ��
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, shuttleTexture);
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());


    //������·
    // ���ºͻ���·��Ƭ
    for (int i = 0; i < numRoadSegments; ++i) {
        glm::mat4 roadMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, roadOffsets[i]));
        glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(vMat * roadMat));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, roadTexture);
        glBindVertexArray(vao[1]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    // ����·��Ƭ��λ��
    for (int i = 0; i < numRoadSegments; ++i) {
        roadOffsets[i] += roadSpeed; // ������Ҫ�����ٶ�
        if (roadOffsets[i] > roadSegmentLength) { // ���·��Ƭ�Ƴ���Ұ
            roadOffsets[i] -= roadSegmentLength * numRoadSegments; // �����Ƶ������
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

    //���ڻص�
    glfwSetWindowSizeCallback(window, window_size_callback);

    // ���ü��̻ص�
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
