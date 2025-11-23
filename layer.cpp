// layer.cpp
#include "layer.h"
#include <iostream>
#include <algorithm>
#include <limits>

void Layer::updateBounds() {
    if (elements.empty()) {
        minX = minY = maxX = maxY = 0;
        return;
    }

    minX = std::numeric_limits<int>::max();
    minY = std::numeric_limits<int>::max();
    maxX = std::numeric_limits<int>::min();
    maxY = std::numeric_limits<int>::min();

    for (const auto& elemPair : elements) {
        Element* elem = elemPair.first;
        int x = elemPair.second.first;
        int y = elemPair.second.second;
        int elemWidth = elem->getWidth();
        int elemHeight = elem->getHeight();

        minX = std::min(minX, x);
        minY = std::min(minY, y);
        maxX = std::max(maxX, x + elemWidth - 1);
        maxY = std::max(maxY, y + elemHeight - 1);
    }
}

void Layer::updateGrid() {
    for (auto& row : grid) {
        std::fill(row.begin(), row.end(), ' ');
    }

    for (const auto& elemPair : elements) {
        Element* elem = elemPair.first;
        int startX = elemPair.second.first;
        int startY = elemPair.second.second;

        for (int i = 0; i < elem->getHeight(); i++) {
            for (int j = 0; j < elem->getWidth(); j++) {
                char cell = elem->getCell(j, i);
                if (cell != ' ') {
                    if (startY + i >= grid.size()) {
                        grid.resize(startY + i + 1, std::vector<char>(grid.empty() ? 0 : grid[0].size(), ' '));
                    }
                    if (startX + j >= grid[0].size()) {
                        for (auto& row : grid) {
                            row.resize(startX + j + 1, ' ');
                        }
                    }
                    grid[startY + i][startX + j] = cell;
                }
            }
        }
    }
}

Layer::Layer() : minX(0), minY(0), maxX(0), maxY(0) {}

Layer::Layer(const Layer& other) {
    minX = other.minX;
    minY = other.minY;
    maxX = other.maxX;
    maxY = other.maxY;
    elements = other.elements;
}

Layer::~Layer() {
    elements.clear();
}

bool Layer::hasOverlap(Element* elem, int x, int y) const {
    int elemWidth = elem->getWidth();
    int elemHeight = elem->getHeight();

    for (const auto& existingPair : elements) {
        Element* existingElem = existingPair.first;
        int existingX = existingPair.second.first;
        int existingY = existingPair.second.second;
        int existingWidth = existingElem->getWidth();
        int existingHeight = existingElem->getHeight();

        if (x < existingX + existingWidth &&
            x + elemWidth > existingX &&
            y < existingY + existingHeight &&
            y + elemHeight > existingY) {
            return true;
        }
    }
    return false;
}

bool Layer::placeElement(Element* elem, int x, int y) {
    if (!elem) return false;

    if (hasOverlap(elem, x, y)) {
        return false;
    }

    elements.push_back(std::make_pair(elem, std::make_pair(x, y)));

    updateBounds();

    return true;
}

void Layer::removeElement(int index) {
    if (index >= 0 && index < elements.size()) {
        elements.erase(elements.begin() + index);
        updateGrid();
    }
}

void Layer::clearLayer() {
    elements.clear();
    updateGrid();
}

int Layer::getWidth() const {
    return elements.empty() ? 0 : (maxX - minX + 1);
}

int Layer::getHeight() const {
    return elements.empty() ? 0 : (maxY - minY + 1);
}

int Layer::getMinX() const { return minX; }
int Layer::getMinY() const { return minY; }
int Layer::getMaxX() const { return maxX; }
int Layer::getMaxY() const { return maxY; }

const std::vector<std::pair<Element*, std::pair<int, int>>>& Layer::getElements() const {
    return elements;
}

char Layer::getCell(int x, int y) const {
    for (const auto& elemPair : elements) {
        Element* elem = elemPair.first;
        int elemX = elemPair.second.first;
        int elemY = elemPair.second.second;
        int elemWidth = elem->getWidth();
        int elemHeight = elem->getHeight();

        if (x >= elemX && x < elemX + elemWidth &&
            y >= elemY && y < elemY + elemHeight) {
            return elem->getCell(x - elemX, y - elemY);
        }
    }
    return ' ';
}

bool Layer::isEmpty() const {
    return elements.empty();
}

bool Layer::canPlaceWithLowerLayer(Element* elem, int x, int y, const Layer* lowerLayer) const {
    if (!elem || !lowerLayer) return false;

    int elemWidth = elem->getWidth();
    int elemHeight = elem->getHeight();

    for (int i = 0; i < elemHeight; i++) {
        for (int j = 0; j < elemWidth; j++) {
            char elemCell = elem->getCell(j, i);

            if (elemCell == '1') {
                char lowerCell = lowerLayer->getCell(x + j, y + i);
                if (lowerCell != '0') {
                    std::cout << "Connection issue at (" << x + j << "," << y + i << "," << lowerLayer->getCell(1, 0) << ")" << std::endl;
                    return false;
                }
            }
        }
    }
    return true;
}

void Layer::display() const {
    if (elements.empty()) {
        std::cout << "Layer is empty" << std::endl << std::endl;
        return;
    }
    int width = getWidth();
    int height = getHeight();
    std::cout << "Layer (" << width << "x" << height << "):" << std::endl;
    std::cout << "Bounds: X[" << minX << ".." << maxX << "] Y[" << minY << ".." << maxY << "]" << std::endl;

    std::cout << "I";
    for (int x = 0; x < width; x++) std::cout << "__";
    std::cout << "I" << std::endl;

    for (int y = 0; y < height; y++) {
        std::cout << "|";
        for (int x = minX; x <= maxX; x++) {
            char cell = getCell(x, y);
            switch (cell) {
                case ' ': std::cout << "  "; break;
                case '0': std::cout << "0 "; break;
                case '1': std::cout << "1 "; break;
                default: std::cout << "? "; break;
            }
        }
        std::cout << "|" << std::endl;
    }

    std::cout << "I";
    for (int x = 0; x < width; x++) std::cout << "__";
    std::cout << "I" << std::endl;

    std::cout << "Elements: " << elements.size() << std::endl;
    for (size_t i = 0; i < elements.size(); i++) {
        Element* elem = elements[i].first;
        int x = elements[i].second.first;
        int y = elements[i].second.second;

        std::cout << "  " << i + 1 << ". ";
        if (elem->getType() == ElementType::MOTOR) {
            std::cout << "Motor ";
        } else {
            std::cout << "Element ";
        }
        std::cout << "at (" << x << "," << y << ") size: "
                  << elem->getWidth() << "x" << elem->getHeight() << std::endl;
    }
    std::cout << std::endl;
}
