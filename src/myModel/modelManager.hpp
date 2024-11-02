#pragma once
#include <vector>
#include <memory>
#include "Model.h"
class ModelManager {
public:
    std::vector<std::shared_ptr<Model>> models;

    void loadModel(const char* path) {
        models.push_back(std::make_shared<Model>(path));
    }

    void drawAll() {
        for (const auto& model : models) {
            model->draw();
        }
    }
};
