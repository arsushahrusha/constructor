// scheme.h
#ifndef SCHEME_H
#define SCHEME_H

#include "element.h"
#include "layer.h"
#include <vector>

class Scheme {
private:
    std::vector<Layer*> layers;
    int nextElementId;

public:
    Scheme();
    Scheme(const Scheme& other);
    ~Scheme();

    int createLayer();
    bool addElement(Element* elem, int layerIndex, int x, int y);
    bool removeElement(int layerIndex, int elementIndex);
    bool removeLayer(int layerIndex);
    Layer* getLayer(int layerIndex);
    int getLayerCount() const;
    bool validateStructure() const;
    void display() const;
    void getStats() const;
    void showMemoryUsage() const;
};

#endif // SCHEME_H
