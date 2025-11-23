// scheme.cpp
#include "scheme.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
    #pragma comment(lib, "psapi.lib")  // ← Это автоматически подключает библиотеку
#endif


Scheme::Scheme() : nextElementId(1) {}

Scheme::Scheme(const Scheme& other) {
    nextElementId = other.nextElementId;
    for (const auto& layer : other.layers) {
        layers.push_back(new Layer(*layer));
    }
}

Scheme::~Scheme() {
    for (auto layer : layers) {
        delete layer;
    }
    layers.clear();
}

int Scheme::createLayer() {
    Layer* newLayer = new Layer();
    layers.push_back(newLayer);
    return layers.size() - 1;
}

bool Scheme::addElement(Element* elem, int layerIndex, int x, int y) {
    if (layerIndex < 0 || layerIndex >= layers.size()) {
        std::cout << "Error: Layer " << layerIndex << " doesn't exist!" << std::endl;
        return false;
    }

    if (!elem) {
        std::cout << "Error: Invalid element!" << std::endl;
        return false;
    }

    Layer* targetLayer = layers[layerIndex];

    if (targetLayer->hasOverlap(elem, x, y)) {
        std::cout << "Error: Element overlaps with existing elements on layer " << layerIndex << "!" << std::endl;
        return false;
    }

    if (layerIndex > 0) {
        Layer* lowerLayer = layers[layerIndex - 1];
        if (!targetLayer->canPlaceWithLowerLayer(elem, x, y, lowerLayer)) {
            std::cout << "Error: Element doesn't properly connect with layer below!" << std::endl;
            return false;
        }
    }

    return targetLayer->placeElement(elem, x, y);
}

bool Scheme::removeElement(int layerIndex, int elementIndex) {
    if (layerIndex < 0 || layerIndex >= layers.size()) {
        std::cout << "Error: Layer " << layerIndex << " doesn't exist!" << std::endl;
        return false;
    }

    Layer* layer = layers[layerIndex];
    if (elementIndex < 0 || elementIndex >= layer->getElements().size()) {
        std::cout << "Error: Element " << elementIndex << " doesn't exist on layer " << layerIndex << "!" << std::endl;
        return false;
    }

    layer->removeElement(elementIndex);
    return true;
}

bool Scheme::removeLayer(int layerIndex) {
    if (layerIndex < 0 || layerIndex >= layers.size()) {
        std::cout << "Error: Layer " << layerIndex << " doesn't exist!" << std::endl;
        return false;
    }

    Layer* layer = layers[layerIndex];
    if (!layer->isEmpty() && layerIndex < layers.size() - 1) {
        std::cout << "Error: Cannot delete non-empty layer with layers above!" << std::endl;
        return false;
    }

    delete layers[layerIndex];
    layers.erase(layers.begin() + layerIndex);
    return true;
}

Layer* Scheme::getLayer(int layerIndex) {
    if (layerIndex < 0 || layerIndex >= layers.size()) {
        return nullptr;
    }
    return layers[layerIndex];
}

int Scheme::getLayerCount() const {
    return layers.size();
}

bool Scheme::validateStructure() const {
    for (int i = 1; i < layers.size(); i++) {
        Layer* currentLayer = layers[i];
        Layer* lowerLayer = layers[i - 1];

        const auto& elements = currentLayer->getElements();
        for (const auto& elemPair : elements) {
            Element* elem = elemPair.first;
            int x = elemPair.second.first;
            int y = elemPair.second.second;

            if (!currentLayer->canPlaceWithLowerLayer(elem, x, y, lowerLayer)) {
                std::cout << "Validation failed: Element on layer " << i
                          << " at (" << x << "," << y << ") doesn't connect properly!" << std::endl;
                return false;
            }
        }
    }
    return true;
}

void Scheme::display() const {
    if (layers.empty()) {
        std::cout << "Scheme is empty!" << std::endl << std::endl;
        return;
    }

    std::cout << "=== SCHEME (" << layers.size() << " layers) ===" << std::endl;

    for (int i = 0; i < layers.size(); i++) {
        std::cout << "--- LAYER " << i << " ---" << std::endl;
        layers[i]->display();
    }

    if (validateStructure()) {
        std::cout << "Structure is valid" << std::endl;
    } else {
        std::cout << "Structure has connection issues!" << std::endl;
    }
    std::cout << std::endl;
}

void Scheme::showMemoryUsage() const {
    std::cout << "\n=== MEMORY USAGE ===" << std::endl;

#ifdef _WIN32
    HANDLE hProcess = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        std::cout << "Memory (RAM usage): " << pmc.WorkingSetSize / 1024 << " KB" << std::endl;
    } else {
        std::cout << "Could not get memory info." << std::endl;
    }
#else
    std::cout << "Memory monitoring: Windows only." << std::endl;
#endif
}


void Scheme::getStats() const {
    std::cout << "=== SCHEME STATISTICS ===" << std::endl;
    std::cout << "Total layers: " << layers.size() << std::endl;

    int totalElements = 0;
    int totalMotors = 0;

    for (int i = 0; i < layers.size(); i++) {
        const auto& elements = layers[i]->getElements();
        totalElements += elements.size();

        for (const auto& elemPair : elements) {
            if (elemPair.first->getType() == ElementType::MOTOR) {
                totalMotors++;
            }
        }

        std::cout << "Layer " << i << ": " << elements.size() << " elements, "
                  << "size: " << layers[i]->getWidth() << "x" << layers[i]->getHeight() << std::endl;
    }

    std::cout << "Total elements: " << totalElements << std::endl;
    std::cout << "Total motors: " << totalMotors << std::endl;
    showMemoryUsage();
    std::cout << std::endl;
}
