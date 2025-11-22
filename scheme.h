#ifndef SCHEME_H
#define SCHEME_H

#include "layer.h"
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace std;

class Scheme {
private:
    vector<Layer*> layers;
    int nextElementId; // Для уникальных идентификаторов элементов

public:
    // Конструкторы
    Scheme() : nextElementId(1) {}
    
    // Конструктор копирования
    Scheme(const Scheme& other) {
        nextElementId = other.nextElementId;
        for (const auto& layer : other.layers) {
            layers.push_back(new Layer(*layer));
        }
    }
    
    // Деструктор
    ~Scheme() {
        for (auto layer : layers) {
            delete layer;
        }
        layers.clear();
    }

    // Создание нового слоя
    int createLayer() {
        Layer* newLayer = new Layer();
        layers.push_back(newLayer);
        return layers.size() - 1; // Возвращаем индекс нового слоя
    }

    // Добавление элемента на конкретный слой
    bool addElement(Element* elem, int layerIndex, int x, int y) {
        if (layerIndex < 0 || layerIndex >= layers.size()) {
            cout << "Error: Layer " << layerIndex << " doesn't exist!" << endl;
            return false;
        }
        
        if (!elem) {
            cout << "Error: Invalid element!" << endl;
            return false;
        }
        
        Layer* targetLayer = layers[layerIndex];
        
        // Проверяем возможность размещения на текущем слое
        if (targetLayer->hasOverlap(elem, x, y)) {
            cout << "Error: Element overlaps with existing elements on layer " << layerIndex << "!" << endl;
            return false;
        }
        
        // Если это не базовый слой, проверяем соединение с нижним слоем
        if (layerIndex > 0) {
            Layer* lowerLayer = layers[layerIndex - 1];
            if (!targetLayer->canPlaceWithLowerLayer(elem, x, y, lowerLayer)) {
                cout << "Error: Element doesn't properly connect with layer below!" << endl;
                return false;
            }
        }
        
        // Размещаем элемент
        return targetLayer->placeElement(elem, x, y);
    }

    // Удаление элемента со слоя
    bool removeElement(int layerIndex, int elementIndex) {
        if (layerIndex < 0 || layerIndex >= layers.size()) {
            cout << "Error: Layer " << layerIndex << " doesn't exist!" << endl;
            return false;
        }
        
        Layer* layer = layers[layerIndex];
        if (elementIndex < 0 || elementIndex >= layer->getElements().size()) {
            cout << "Error: Element " << elementIndex << " doesn't exist on layer " << layerIndex << "!" << endl;
            return false;
        }
        
        layer->removeElement(elementIndex);
        return true;
    }

    // Удаление слоя
    bool removeLayer(int layerIndex) {
        if (layerIndex < 0 || layerIndex >= layers.size()) {
            cout << "Error: Layer " << layerIndex << " doesn't exist!" << endl;
            return false;
        }
        
        // Нельзя удалить слой, если на нем есть элементы и есть слои выше
        Layer* layer = layers[layerIndex];
        if (!layer->isEmpty() && layerIndex < layers.size() - 1) {
            cout << "Error: Cannot delete non-empty layer with layers above!" << endl;
            return false;
        }
        
        delete layers[layerIndex];
        layers.erase(layers.begin() + layerIndex);
        return true;
    }

    // Получение слоя по индексу
    Layer* getLayer(int layerIndex) {
        if (layerIndex < 0 || layerIndex >= layers.size()) {
            return nullptr;
        }
        return layers[layerIndex];
    }

    // Получение количества слоев
    int getLayerCount() const {
        return layers.size();
    }

    // Проверка целостности структуры
    bool validateStructure() const {
        for (int i = 1; i < layers.size(); i++) {
            Layer* currentLayer = layers[i];
            Layer* lowerLayer = layers[i - 1];
            
            // Проверяем все элементы текущего слоя
            const auto& elements = currentLayer->getElements();
            for (const auto& elemPair : elements) {
                Element* elem = elemPair.first;
                int x = elemPair.second.first;
                int y = elemPair.second.second;
                
                if (!currentLayer->canPlaceWithLowerLayer(elem, x, y, lowerLayer)) {
                    cout << "Validation failed: Element on layer " << i 
                         << " at (" << x << "," << y << ") doesn't connect properly!" << endl;
                    return false;
                }
            }
        }
        return true;
    }

    // Отображение всей схемы
    void display() const {
        if (layers.empty()) {
            cout << "Scheme is empty!" << endl << endl;
            return;
        }
        
        cout << "=== SCHEME (" << layers.size() << " layers) ===" << endl;
        
        for (int i = 0; i < layers.size(); i++) {
            cout << "--- LAYER " << i << " ---" << endl;
            layers[i]->display();
        }
        
        // Проверка целостности
        if (validateStructure()) {
            cout << "Structure is valid" << endl;
        } else {
            cout << "Structure has connection issues!" << endl;
        }
        cout << endl;
    }

    // Получение статистики по схеме
    void getStats() const {
        cout << "=== SCHEME STATISTICS ===" << endl;
        cout << "Total layers: " << layers.size() << endl;
        
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
            
            cout << "Layer " << i << ": " << elements.size() << " elements, "
                 << "size: " << layers[i]->getWidth() << "x" << layers[i]->getHeight() << endl;
        }
        
        cout << "Total elements: " << totalElements << endl;
        cout << "Total motors: " << totalMotors << endl;
        cout << endl;
    }
};

#endif