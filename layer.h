// layer.h
#ifndef LAYER_H
#define LAYER_H

#include "element.h"
#include <vector>
#include <utility>

class Layer {
private:
    int minX, minY, maxX, maxY;
    std::vector<std::vector<char>> grid;
    std::vector<std::pair<Element*, std::pair<int, int>>> elements;

    void updateBounds();
    void updateGrid();

public:
    Layer();
    Layer(const Layer& other);
    ~Layer();

    bool hasOverlap(Element* elem, int x, int y) const;
    bool placeElement(Element* elem, int x, int y);
    void removeElement(int index);
    void clearLayer();

    int getWidth() const;
    int getHeight() const;
    int getMinX() const;
    int getMinY() const;
    int getMaxX() const;
    int getMaxY() const;
    const std::vector<std::pair<Element*, std::pair<int, int>>>& getElements() const;
    char getCell(int x, int y) const;
    bool isEmpty() const;
    bool canPlaceWithLowerLayer(Element* elem, int x, int y, const Layer* lowerLayer) const;
    void display() const;
};

#endif // LAYER_H
