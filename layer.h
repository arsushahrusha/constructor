#ifndef LAYER_H
#define LAYER_H

#include "element.h"
#include <vector>
#include <utility>
#include <iostream>
#include <stdexcept>
#include <algorithm>

using namespace std;

class Layer {
private:
    int width;
    int height;
    int minX, minY, maxX, maxY; 
    vector<vector<char>> grid; // ' '=пусто, '0'=паз, '1'=выпуклость
    vector<pair<Element*, pair<int, int>>> elements; // элемент + координаты (x,y)

    void updateBounds() {
        if (elements.empty()) {
            minX = minY = maxX = maxY = 0;
            return;
        }
        
        minX = numeric_limits<int>::max();
        minY = numeric_limits<int>::max();
        maxX = numeric_limits<int>::min();
        maxY = numeric_limits<int>::min();
        
        for (const auto& elemPair : elements) {
            Element* elem = elemPair.first;
            int x = elemPair.second.first;
            int y = elemPair.second.second;
            int elemWidth = elem->getWidth();
            int elemHeight = elem->getHeight();
            
            minX = min(minX, x);
            minY = min(minY, y);
            maxX = max(maxX, x + elemWidth - 1);
            maxY = max(maxY, y + elemHeight - 1);
        }
    }

    // Проверка перекрытия элементов
    bool hasOverlap(Element* elem, int x, int y) const {
        int elemWidth = elem->getWidth();
        int elemHeight = elem->getHeight();
        
        for (const auto& existingPair : elements) {
            Element* existingElem = existingPair.first;
            int existingX = existingPair.second.first;
            int existingY = existingPair.second.second;
            int existingWidth = existingElem->getWidth();
            int existingHeight = existingElem->getHeight();
            
            // Проверяем пересечение прямоугольников
            if (x < existingX + existingWidth &&
                x + elemWidth > existingX &&
                y < existingY + existingHeight &&
                y + elemHeight > existingY) {
                return true;
            }
        }
        return false;
    }

    // Вспомогательный метод для обновления grid
    void updateGrid() {
        // Очищаем grid
        for (auto& row : grid) {
            fill(row.begin(), row.end(), ' ');
        }
        
        // Заполняем grid элементами
        for (const auto& elemPair : elements) {
            Element* elem = elemPair.first;
            int startX = elemPair.second.first;
            int startY = elemPair.second.second;
            
            for (int i = 0; i < elem->getHeight(); i++) {
                for (int j = 0; j < elem->getWidth(); j++) {
                    char cell = elem->getCell(j, i);
                    if (cell != ' ') {
                        grid[startY + i][startX + j] = cell;
                    }
                }
            }
        }
    }

public:

    // Конструктор по умолчанию
    Layer() : minX(0), minY(0), maxX(0), maxY(0) {}
    
    // Конструктор копирования
    Layer(const Layer& other) {
        minX = other.minX;
        minY = other.minY;
        maxX = other.maxX;
        maxY = other.maxY;
        elements = other.elements;
    }

    // Размещение элемента на конкретной позиции
    bool placeElement(Element* elem, int x, int y) {
        if (!elem) return false;
        
        // Проверяем перекрытие
        if (hasOverlap(elem, x, y)) {
            return false;
        }
        
        // Добавляем элемент
        elements.push_back(make_pair(elem, make_pair(x, y)));
        
        // Обновляем границы
        updateBounds();
        
        return true;
    }

    // Деструктор
    ~Layer() {
        // Не удаляем элементы, так как они управляются извне
        elements.clear();
    }

    // // Проверка возможности размещения элемента
    // bool canPlaceElement(Element* elem, int x, int y, Layer* lowerLayer = nullptr) const {
    //     if (!elem) return false;
        
    //     int elemWidth = elem->getWidth();
    //     int elemHeight = elem->getHeight();
        
    //     // 1. Проверка границ
    //     if (x < 0 || y < 0 || x + elemWidth > width || y + elemHeight > height) {
    //         return false;
    //     }
        
    //     // 2. Проверка перекрытия на текущем слое
    //     for (int i = 0; i < elemHeight; i++) {
    //         for (int j = 0; j < elemWidth; j++) {
    //             char elemCell = elem->getCell(j, i);
    //             char gridCell = grid[y + i][x + j];
                
    //             // Если в элементе есть материал (0 или 1), а в grid уже что-то есть
    //             if (elemCell != ' ' && gridCell != ' ') {
    //                 return false;
    //             }
    //         }
    //     }
        
    //     // 3. Проверка соединения с нижним слоем (если он есть)
    //     if (lowerLayer != nullptr) {
    //         for (int i = 0; i < elemHeight; i++) {
    //             for (int j = 0; j < elemWidth; j++) {
    //                 char elemCell = elem->getCell(j, i);
                    
    //                 if (elemCell == '1') { // Выпуклость
    //                     char lowerCell = lowerLayer->getGridCell(x + j, y + i);
    //                     if (lowerCell != '0') { // Должен быть паз
    //                         return false;
    //                     }
    //                 }
    //                 // Для паза (0) проверка не требуется - может быть над выпуклостью или пустотой
    //             }
    //         }
    //     }
        
    //     return true;
    // }

    // // Размещение элемента на слое
    // bool placeElement(Element* elem, int x, int y) {
    //     if (!canPlaceElement(elem, x, y)) {
    //         return false;
    //     }
        
    //     // Добавляем элемент в список
    //     elements.push_back(make_pair(elem, make_pair(x, y)));
        
    //     // Обновляем grid
    //     updateGrid();
        
    //     return true;
    // }

    // Удаление элемента по индексу
    void removeElement(int index) {
        if (index >= 0 && index < elements.size()) {
            elements.erase(elements.begin() + index);
            updateGrid();
        }
    }

    // Очистка слоя
    void clearLayer() {
        elements.clear();
        updateGrid();
    }

    // Селекторы
    // Селекторы
    int getWidth() const { return elements.empty() ? 0 : (maxX - minX + 1); }
    int getHeight() const { return elements.empty() ? 0 : (maxY - minY + 1); }
    int getMinX() const { return minX; }
    int getMinY() const { return minY; }
    int getMaxX() const { return maxX; }
    int getMaxY() const { return maxY; }
    const vector<pair<Element*, pair<int, int>>>& getElements() const { return elements; }
    
    // Получение символа в определенной позиции слоя
    char getCell(int x, int y) const {
        for (const auto& elemPair : elements) {
            Element* elem = elemPair.first;
            int elemX = elemPair.second.first;
            int elemY = elemPair.second.second;
            int elemWidth = elem->getWidth();
            int elemHeight = elem->getHeight();
            
            // Проверяем, попадает ли точка в границы элемента
            if (x >= elemX && x < elemX + elemWidth &&
                y >= elemY && y < elemY + elemHeight) {
                
                // Возвращаем символ из матрицы элемента
                return elem->getCell(x - elemX, y - elemY);
            }
        }
        return ' '; // Пустота
    }

    // Проверка пустоты слоя
    bool isEmpty() const {
        return elements.empty();
    }

    // Проверка возможности размещения с нижним слоем
    bool canPlaceWithLowerLayer(Element* elem, int x, int y, const Layer* lowerLayer) const {
        if (!elem || !lowerLayer) return false;
        
        int elemWidth = elem->getWidth();
        int elemHeight = elem->getHeight();
        
        // Проверяем соединение с нижним слоем
        for (int i = 0; i < elemHeight; i++) {
            for (int j = 0; j < elemWidth; j++) {
                char elemCell = elem->getCell(j, i);
                
                if (elemCell == '1') { // Выпуклость
                    char lowerCell = lowerLayer->getCell(x + j, y + i);
                    if (lowerCell != '0') { // Должен быть паз
                        std::cout<< "Connection issue at (" << x + j << "," << y + i << "," << lowerLayer->getCell(1, 0) <<  ")" << std::endl;
                        return false; 
                    }
                }
            }
        }
        
        return true;
    }

    // Отображение слоя
    void display() const {
        if (elements.empty()) {
            cout << "Layer is empty" << endl << endl;
            return;
        }
        int width = getWidth();
        int height = getHeight();
        cout << "Layer (" << width << "x" << height << "):" << endl;
        cout << "Bounds: X[" << minX << ".." << maxX << "] Y[" << minY << ".." << maxY << "]" << endl;
        
        // Верхняя граница
        cout << "I";
        for (int x = 0; x < width; x++) cout << "__";
        cout << "I" << endl;
        
        // Содержимое
        for (int y = 0; y < height; y++) {
            cout << "|";
            for (int x = minX; x <= maxX; x++) {
                char cell = getCell(x, y);
                switch (cell) {
                    case ' ': cout << "  "; break;  // Пустота
                    case '0': cout << "0 "; break; // Паз
                    case '1': cout << "1 "; break; // Выпуклость
                    default: cout << "? "; break;
                }
            }
            cout << "|" << endl;
        }
        
        // Нижняя граница
        cout << "I";
        for (int x = 0; x < width; x++) cout << "__";
        cout << "I" << endl;
        
        // Информация об элементах
        cout << "Elements: " << elements.size() << endl;
        for (size_t i = 0; i < elements.size(); i++) {
            Element* elem = elements[i].first;
            int x = elements[i].second.first;
            int y = elements[i].second.second;
            
            cout << "  " << i + 1 << ". ";
            if (elem->getType() == ElementType::MOTOR) {
                cout << "Motor ";
            } else {
                cout << "Element ";
            }
            cout << "at (" << x << "," << y << ") size: " 
                 << elem->getWidth() << "x" << elem->getHeight() << endl;
        }
        cout << endl;
    }
};

#endif