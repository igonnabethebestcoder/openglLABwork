#include <glm/common.hpp>
#include <GLFW/glfw3.h>
#include<algorithm>

///
/// 键盘事件封装
///

extern float objLocX, objLocY, objLocZ;

//马路无限产生
extern const int numRoadSegments; // 路面片的数量
extern const float roadSegmentLength ; // 每个路面片的长度
extern float roadOffsets[]; // 存储路面片的位置偏移
extern float roadSpeed; // 初始化路面更新速度

//车头转向
extern bool leftKeyPressed ;  // 记录左箭头是否被按下
extern bool rightKeyPressed; // 记录右箭头是否被按下
extern int lefttimes;//多次按下左时只转向一次
extern int righttimes;


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