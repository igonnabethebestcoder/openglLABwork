#pragma once
#include <GL/glew.h>
#include <string>
#include "../ImportedModel/ImportedModel.h"
#include <glm/ext/vector_float3.hpp>
#include <glm/ext/vector_float2.hpp>

class Model {
public:
    GLuint vao, vbo[3], ebo;
    int numVertices;

    Model(const char* filePath) {
        loadModel(filePath);
    }

    void loadModel(const char* filePath) {
        ImportedModel model(filePath);
        std::vector<glm::vec3> vert = model.getVertices();
        std::vector<glm::vec2> tex = model.getTextureCoords();
        std::vector<glm::vec3> norm = model.getNormals();
        numVertices = model.getNumVertices();

        std::vector<float> pvalues, tvalues, nvalues;

        for (int i = 0; i < model.getNumVertices(); i++) {
            pvalues.push_back((vert[i]).x);
            pvalues.push_back((vert[i]).y);
            pvalues.push_back((vert[i]).z);
            tvalues.push_back((tex[i]).s);
            tvalues.push_back((tex[i]).t);
            nvalues.push_back((norm[i]).x);
            nvalues.push_back((norm[i]).y);
            nvalues.push_back((norm[i]).z);
        }

        glGenVertexArrays(1, &vao);
        glGenBuffers(3, vbo);
        glGenBuffers(1, &ebo);

        // °ó¶¨VAO
        glBindVertexArray(vao);

        // ¶¥µã»º³å
        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, pvalues.size() * sizeof(float), pvalues.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(0);

        // ÎÆÀí×ø±ê»º³å
        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, tvalues.size() * sizeof(float), tvalues.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(1);

        // ·¨Ïß»º³å
        glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, nvalues.size() * sizeof(float), nvalues.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);
        glEnableVertexAttribArray(2);

        // ½â°óVAO
        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, numVertices);
        glBindVertexArray(0);
    }
};
