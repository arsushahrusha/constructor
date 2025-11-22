#ifndef ELEMENT_H
#define ELEMENT_H
#include <vector>
#include <iostream>

enum class ElementType
{
    ELEMENT,
    MOTOR
};

class Element
{
private:
    int width;
    int height;
    std::vector<std::vector<char>> matrix;

public:
    Element() : width(0), height(0), matrix() {};                    // Конструктор по умолчанию
    Element(int w, int h, const std::vector<std::vector<char>> &mat) // Конструктор инициализации
    {
        width = w;
        height = h;
        matrix = mat;
    };
    Element(const Element &other) // Конструктор копирования
    {
        width = other.width;
        height = other.height;
        matrix = other.matrix;
    }

    // Селекторы (геттеры)
    int getWidth() const
    {
        return width;
    }
    int getHeight() const
    {
        return height;
    }

    char getCell(int x, int y) const
    {
        if (x >= 0 && x < width && y >= 0 && y < height)
        {
            return matrix[y][x];
        }
        return ' ';
    }

    const std::vector<std::vector<char>> &getMatrix() const
    {
        return matrix;
    }

    // Модификаторы (сеттеры)
    void setWidth(int newWidth)
    {
        if (newWidth > 0)
            width = newWidth;
    }

    void setHeight(int newHeight)
    {
        if (newHeight > 0)
            height = newHeight;
    }

    void setCell(int x, int y, char value)
    {
        if (x >= 0 && x < width && y >= 0 && y < height && (value == '0' or value == '1'))
        {
            matrix[y][x] = value;
        }
    }

    void setMatrix(const std::vector<std::vector<char>> &newMatrix)
    {
        // Проверяем, что все элементы матрицы содержат только '0' или '1'
        for (const std::vector<char> &row : newMatrix)
        { // & это ссылка на отдельный символ и не делает копий
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

    // Виртуальный метод идентификации
    virtual ElementType getType() const
    {
        return ElementType::ELEMENT;
    }
};

class Motor : public Element
{
private:
    int speed;
    bool isRotating;
    int direction; // 1 - по часовой 2 - против часовой 0 - не двигается
public:
    // Конструктор по умолчанию
    Motor() : Element(), speed(0), isRotating(false), direction(0) {}

    // Конструктор инициализации
    Motor(int w, int h, const std::vector<std::vector<char>> &mat,
          int spd = 0, int dir = 0) : Element(w, h, mat), speed(spd), direction(dir)
    {
        isRotating = (speed > 0); // Если скорость > 0, мотор вращается
    }
    // Конструктор копирования
    Motor(const Motor &other) : Element(other),
                                speed(other.speed),
                                isRotating(other.isRotating),
                                direction(other.direction)
    {
    }

    // Перегрузка виртуального метода идентификации
    virtual ElementType getType() const
    {
        return ElementType::MOTOR;
    }

    // Индивидуальные сеттеры и геттеры
    void setSpeed(int newSpeed)
    {
        if ((newSpeed >= 0) && (newSpeed <= 100))
        {
            speed = newSpeed;
        }
    }

    int getSpeed() const
    {
        return speed;
    }

    void setDirection(int newDirection)
    {
        if (newDirection == 1 || newDirection == 2 || newDirection == 0)
        {
            direction = newDirection;
        }
    }

    int getDirection() const
    {
        return direction;
    }

    void setStatus(int newStatus)
    {
        if (newStatus == true || newStatus == false)
        {
            isRotating = newStatus;
        }
    }

    bool getStatus() const
    {
        return isRotating;
    }

    // Методы уникальные
    void rotate(int newSpeed, int newDirection)
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

    void stop()
    {
        speed = 0;
        isRotating = false;
        direction = 0;
        std::cout << "Motor has been stopped!" << std::endl;
    }
};

#endif