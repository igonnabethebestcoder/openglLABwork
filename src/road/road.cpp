#include <GL/glew.h>
#include <gl/GL.h>

void drawRoad() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f); // 马路颜色
    glVertex3f(-5.0f, 0.0f, -10.0f); // 左下角
    glVertex3f(5.0f, 0.0f, -10.0f); // 右下角
    glVertex3f(5.0f, 0.0f, 10.0f); // 右上角
    glVertex3f(-5.0f, 0.0f, 10.0f); // 左上角
    glEnd();
}
