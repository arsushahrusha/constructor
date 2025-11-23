// element.cpp
#include "element.h"
#include <iostream>

// Element

Element::Element() : width(0), height(0), matrix() {}

Element::Element(int w, int h, const std::vector<std::vector<char>> &mat)
{
    if (w > 0 && h > 0) {
        width = w;
        height = h;
        matrix = mat;
    }
}

Element::Element(const Element &other)
{
    width = other.width;
    height = other.height;
    matrix = other.matrix;
}

int Element::getWidth() const
{
    return width;
}

int Element::getHeight() const
{
    return height;
}

char Element::getCell(int x, int y) const
{
    if (x >= 0 && x < width && y >= 0 && y < height)
    {
        return matrix[y][x];
    }
    return ' ';
}

const std::vector<std::vector<char>> &Element::getMatrix() const
{
    return matrix;
}

void Element::setWidth(int newWidth)
{
    if (newWidth > 0)
        width = newWidth;
}

void Element::setHeight(int newHeight)
{
    if (newHeight > 0)
        height = newHeight;
}

void Element::setCell(int x, int y, char value)
{
    if (x >= 0 && x < width && y >= 0 && y < height && (value == '0' || value == '1'))
    {
        matrix[y][x] = value;
    }
}

void Element::setMatrix(const std::vector<std::vector<char>> &newMatrix)
{
    for (const std::vector<char> &row : newMatrix)
    {
        for (const char &cell : row)
        {
            if (cell != '0' && cell != '1')
            {
                std::cout << "Error: Matrix can only contain '0' or '1'" << std::endl;
                return;
            }
        }
    }
    if (!newMatrix.empty() && !newMatrix[0].empty())
    {
        matrix = newMatrix;
        height = newMatrix.size();
        width = newMatrix[0].size();
    }
    else
    {
        std::cout << "Empty matrix" << std::endl;
    }
}

ElementType Element::getType() const
{
    return ElementType::ELEMENT;
}

// Motor

Motor::Motor() : Element(), speed(0), isRotating(false), direction(0) {}

Motor::Motor(int w, int h, const std::vector<std::vector<char>> &mat,
             int spd, int dir) : Element(w, h, mat), speed(spd), direction(dir)
{
    isRotating = (speed > 0);
}

Motor::Motor(const Motor &other) : Element(other),
                                   speed(other.speed),
                                   isRotating(other.isRotating),
                                   direction(other.direction)
{
}

ElementType Motor::getType() const
{
    return ElementType::MOTOR;
}

void Motor::setSpeed(int newSpeed)
{
    if ((newSpeed >= 0) && (newSpeed <= 100))
    {
        speed = newSpeed;
    }
}

int Motor::getSpeed() const
{
    return speed;
}

void Motor::setDirection(int newDirection)
{
    if (newDirection == 1 || newDirection == 2 || newDirection == 0)
    {
        direction = newDirection;
    }
}

int Motor::getDirection() const
{
    return direction;
}

void Motor::setStatus(int newStatus)
{
    if (newStatus == true || newStatus == false)
    {
        isRotating = newStatus;
    }
}

bool Motor::getStatus() const
{
    return isRotating;
}

void Motor::rotate(int newSpeed, int newDirection)
{
    std::cout << "Enter speed: " << std::endl;
    if ((newSpeed > 0) && (newSpeed <= 100))
    {
        speed = newSpeed;
        isRotating = true;
    }
    else
    {
        std::cout << "Error: Incorrect speed value!" << std::endl;
    }
    std::cout << "Enter direction" << std::endl;
    if (newDirection == 1)
    {
        direction = newDirection;
        std::cout << "Motor is moving with " << newSpeed << " speed in clockwise direction" << std::endl;
    }
    else if (newDirection == 2)
    {
        direction = newDirection;
        std::cout << "Motor is moving with " << newSpeed << " speed in counterclockwise direction" << std::endl;
    }
    else
    {
        std::cout << "Error: Incorrect direction!" << std::endl;
    }
}

void Motor::stop()
{
    speed = 0;
    isRotating = false;
    direction = 0;
    std::cout << "Motor has been stopped!" << std::endl;
}
