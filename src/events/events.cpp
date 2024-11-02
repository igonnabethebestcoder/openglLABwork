#include <glm/common.hpp>
#include <GLFW/glfw3.h>
#include<algorithm>

///
/// �����¼���װ
///

extern float objLocX, objLocY, objLocZ;

//��·���޲���
extern const int numRoadSegments; // ·��Ƭ������
extern const float roadSegmentLength ; // ÿ��·��Ƭ�ĳ���
extern float roadOffsets[]; // �洢·��Ƭ��λ��ƫ��
extern float roadSpeed; // ��ʼ��·������ٶ�

//��ͷת��
extern bool leftKeyPressed ;  // ��¼���ͷ�Ƿ񱻰���
extern bool rightKeyPressed; // ��¼�Ҽ�ͷ�Ƿ񱻰���
extern int lefttimes;//��ΰ�����ʱֻת��һ��
extern int righttimes;


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