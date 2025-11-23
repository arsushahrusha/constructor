// element.h
#ifndef ELEMENT_H
#define ELEMENT_H

#include <vector>

enum class ElementType
{
    ELEMENT,
    MOTOR,
    SCHEME,
    LAYER,
};

class Element
{
private:
    int width;
    int height;
    std::vector<std::vector<char>> matrix;

public:
    Element(); // Конструктор по умолчанию
    Element(int w, int h, const std::vector<std::vector<char>> &mat);
    Element(const Element &other); // Конструктор копирования

    // Селекторы (геттеры)
    int getWidth() const;
    int getHeight() const;
    char getCell(int x, int y) const;
    const std::vector<std::vector<char>> &getMatrix() const;

    // Модификаторы (сеттеры)
    void setWidth(int newWidth);
    void setHeight(int newHeight);
    void setCell(int x, int y, char value);
    void setMatrix(const std::vector<std::vector<char>> &newMatrix);

    // Виртуальный метод идентификации
    virtual ElementType getType() const;
    virtual ~Element() = default; // Деструктор по умолчанию
};

class Motor : public Element
{
private:
    int speed;
    bool isRotating;
    int direction;

public:
    Motor(); // Конструктор по умолчанию
    Motor(int w, int h, const std::vector<std::vector<char>> &mat,
          int spd = 0, int dir = 0);
    Motor(const Motor &other); // Конструктор копирования

    // Перегрузка виртуального метода идентификации
    virtual ElementType getType() const;
    
    // Индивидуальные сеттеры и геттеры
    void setSpeed(int newSpeed);
    int getSpeed() const;
    void setDirection(int newDirection);
    int getDirection() const;
    void setStatus(bool newStatus);
    bool getStatus() const;

    // Методы уникальные
    void rotate(int newSpeed, int newDirection);
    void stop();
};

#endif // ELEMENT_H
