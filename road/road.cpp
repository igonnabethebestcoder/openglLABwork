#include <GL/glew.h>
#include <gl/GL.h>

void drawRoad() {
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f); // ��·��ɫ
    glVertex3f(-5.0f, 0.0f, -10.0f); // ���½�
    glVertex3f(5.0f, 0.0f, -10.0f); // ���½�
    glVertex3f(5.0f, 0.0f, 10.0f); // ���Ͻ�
    glVertex3f(-5.0f, 0.0f, 10.0f); // ���Ͻ�
    glEnd();
}
