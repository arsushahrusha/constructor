#include "element.h"
#include "layer.h"
#include <iostream>
#include <vector>
#include <cassert>
#include <limits>
#include <sstream>
using namespace std;

// Считыватель ввода элемента пользователем
int getInput()
{
    int value;
    while (!(cin >> value))
    {
        cin.clear();
        cout << "Invalid input. Enter a number: ";
    }
    return value;
}

// Считыватель ввода матрицы
vector<vector<char>> inputMatrix(int width, int height)
{
    vector<vector<char>> matrix(height, vector<char>(width, ' '));

    while (true)
    {
        std::cout << "Enter matrix " << width << "x" << height << " (0 or 1):\n";
        bool err = false;

        for (int y = 0; y < height; ++y)
        {
            std::cout << "Line " << (y + 1) << ": ";
            for (int x = 0; x < width; ++x)
            {
                char cell;
                std::cin >> cell;

                if (cell != '0' && cell != '1')
                {
                    std::cout << "Incorrect input, repeat\n";
                    err = true;
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    break;
                }
                matrix[y][x] = cell;
            }

            if (err)
            {
                break;
            }
        }

        if (!err)
        {
            break;
        }

        std::cout << "Please enter the matrix again.\n";
    }

    return matrix;
}

void makeElem(vector<Element *> &elements)
{
    cout << "MAKING ELEMENT" << endl;
    std::cout << "Enter width:" << std::endl;
    int width = getInput();
    std::cout << "Enter height:" << std::endl;
    int height = getInput();
    std::vector<std::vector<char>> matrix = inputMatrix(width, height);
    Element *newElement = new Element(width, height, matrix);
    elements.push_back(newElement);
    std::cout << "Element is made!" << std::endl;
}

void makeMotor(vector<Element *> &elements)
{
    std::cout << "Enter width:" << std::endl;
    int width = getInput();
    std::cout << "Enter height:" << std::endl;
    int height = getInput();
    std::vector<std::vector<char>> matrix = inputMatrix(width, height);

    // Проверка скорости
    int speed;
    while (true)
    {
        std::cout << "Enter speed (1-100):" << std::endl;
        speed = getInput();
        if (speed >= 1 && speed <= 100)
        {
            break;
        }
        else
        {
            std::cout << "Incorrect input" << std::endl;
        }
    }

    // Проверка направления
    int direction;
    while (true)
    {
        std::cout << "Enter direction:" << std::endl;
        std::cout << "1. Clockwise" << std::endl;
        std::cout << "2. Counterclockwise" << std::endl;
        direction = getInput();
        if (direction == 1 || direction == 2)
        {
            break;
        }
        else
        {
            std::cout << "Incorrect input" << std::endl;
        }
    }

    Motor *newMotor = new Motor(width, height, matrix, speed, direction);
    elements.push_back(newMotor);
    std::cout << "Motor is made!" << std::endl;
}

void controlMotor(Motor *motor)
{
    int choice;

    while (true)
    {
        std::cout << "=== Motor Control ===" << std::endl;
        std::cout << "1. Stop" << std::endl;
        std::cout << "2. Start rotating" << std::endl;
        std::cout << "3. Change direction" << std::endl;
        std::cout << "4. Change speed" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "Choose action: ";

        choice = getInput();

        if (choice == 1)
        {
            motor->stop();
            break;
        }
        else if (choice == 2)
        {
            std::cout << "Enter speed (1-100): ";
            int speed = getInput();
            std::cout << "Enter direction (1-clockwise, 2-counterclockwise): ";
            int direction = getInput();
            if ((0 < speed && speed <= 100) && (direction == 1 || direction == 2))
            {
                motor->rotate(speed, direction);
                break;
            }
            else
            {
                std::cout << "Incorrect input" << std::endl;
            }
        }
        else if (choice == 3)
        {
            if (motor->getStatus() == true)
            {
                std::cout << "Enter new direction (1-clockwise, 2-counterclockwise): ";
                int newDirection = getInput();
                if (newDirection == 1 || newDirection == 2)
                {
                    motor->setDirection(newDirection);
                    std::cout << "Direction changed to: " << newDirection << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Incorrect input" << std::endl;
                }
            }
            else
            {
                std::cout << "Motor must be started at first" << std::endl;
            }
        }
        else if (choice == 4)
        {
            if (motor->getStatus() == true)
            {
                std::cout << "Enter new speed (0-100): ";
                int newSpeed = getInput();
                if (newSpeed > 0 && newSpeed <= 100)
                {
                    motor->setSpeed(newSpeed);
                    std::cout << "Speed changed to: " << newSpeed << std::endl;
                    break;
                }
                else
                {
                    std::cout << "Incorrect input" << std::endl;
                }
            }
            else
            {
                std::cout << "Motor must be started at first" << std::endl;
            }
        }
        else if (choice == 5)
        {
            return;
        }
        else
        {
            std::cout << "Incorrect choise, repeat: " << std::endl;
        }
    }
}

void showElements(vector<Element *> &elements)
{
    if (elements.empty())
    {
        cout << "No elements created yet!" << endl
             << endl;
        return;
    }

    cout << "=== All Elements ===" << endl;
    for (int i = 0; i < elements.size(); ++i)
    {
        cout << i + 1 << ". " << (elements[i]->getType() == ElementType::MOTOR ? "Motor" : "Element")
             << " (Width: " << elements[i]->getWidth()
             << ", Height: " << elements[i]->getHeight() << ")" << std::endl;
        std::vector<std::vector<char>> mat = elements[i]->getMatrix();
        int height = elements[i]->getHeight();
        int width = elements[i]->getWidth();
        for (int y = 0; y < height; ++y)
        {
            for (int x = 0; x < width; ++x)
            {
                std::cout << elements[i]->getCell(x, y) << " ";
            }
            std::cout << std::endl;
        }
        if (elements[i]->getType() == ElementType::MOTOR)
        {
            // Преобразование к наследнику
            Motor *motor = static_cast<Motor *>(elements[i]);
            cout << " [Speed: " << motor->getSpeed()
                 << ", Direction: " << motor->getDirection() << "]";
        }
        cout << endl;
    }

    std::cout << std::endl
              << "Select element number to control (0 to return to Main Menu): ";
    while (true)
    {
        int choice = getInput();
        if (choice == 0)
        {
            return;
        }
        else if (choice < 1 || choice > elements.size())
        {
            std::cout << "Incorrect choise, repeat: " << std::endl;
        }
        else
        {
            int index = choice - 1;
            Element *selected = elements[index];
            if (selected->getType() == ElementType::MOTOR)
            {
                Motor *motor = static_cast<Motor *>(selected);
                controlMotor(motor);
            }
            else
            {
                std::cout << "No actions with this element" << std::endl;
            }

            break;
        }
    }
}

// ТЕсты
void runTests()
{
    std::cout << "Running tests..." << std::endl;

    // Тест конструктора по умолчанию
    Element elem1;
    assert(elem1.getWidth() == 0);
    assert(elem1.getHeight() == 0);
    assert(elem1.getCell(0, 0) == ' ');

    // Тест конструктора инициализации
    std::vector<std::vector<char>> mat = {
        {'0', '0', '0'},
        {'1', '1', '1'},
        {'0', '0', '0'},
    };
    Element elem2(3, 3, mat);
    assert(elem2.getWidth() == 3);
    assert(elem2.getHeight() == 3);
    assert(elem2.getCell(0, 0) == '0');
    assert(elem2.getCell(1, 1) == '1');
    assert(elem2.getCell(2, 2) == '0');

    // Тест конструктора копирования
    Element elem3(elem2);
    assert(elem3.getWidth() == 3);
    assert(elem3.getHeight() == 3);
    assert(elem3.getCell(1, 1) == '1');

    // Тест сеттеров ширины и высоты
    elem1.setWidth(5);
    elem1.setHeight(4);
    assert(elem1.getWidth() == 5);
    assert(elem1.getHeight() == 4);

    // Тест сеттеров ячейкеек
    elem2.setCell(0, 0, '1');
    assert(elem2.getCell(0, 0) == '1');

    // Тест сеттеров ячеек (некорректные координаты)
    char original = elem2.getCell(5, 5);
    elem2.setCell(5, 5, '1');
    assert(elem2.getCell(5, 5) == original);

    // Тест сеттера матрицы
    std::vector<std::vector<char>> newMat = {
        {'0', '0', '0'},
        {'1', '1', '1'},
        {'0', '0', '0'},
    };
    elem1.setMatrix(newMat);
    assert(elem1.getWidth() == 3);
    assert(elem1.getHeight() == 3);
    assert(elem1.getCell(0, 0) == '0');
    assert(elem1.getCell(1, 1) == '1');

    // Тест геттера матрицы
    const auto &matrixRef = elem2.getMatrix();
    assert(matrixRef.size() == 3);
    assert(matrixRef[0].size() == 3);

    // Тест граничных значений
    assert(elem2.getCell(-1, -1) == ' ');
    assert(elem2.getCell(10, 10) == ' ');

    Motor motor1;
    assert(motor1.getWidth() == 0);
    assert(motor1.getHeight() == 0);
    assert(motor1.getSpeed() == 0);
    assert(motor1.getDirection() == 0);
    assert(motor1.getStatus() == false);
    assert(motor1.getType() == ElementType::MOTOR);

    // Тест констр иниц Motor
    std::vector<std::vector<char>> motorMat = {
        {'1', '0'},
        {'0', '1'}};
    Motor motor2(2, 2, motorMat, 50, 1);
    assert(motor2.getWidth() == 2);
    assert(motor2.getHeight() == 2);
    assert(motor2.getSpeed() == 50);
    assert(motor2.getDirection() == 1);
    assert(motor2.getStatus() == true);
    assert(motor2.getType() == ElementType::MOTOR);

    // Тест констр копир Motor
    Motor motor3(motor2);
    assert(motor3.getWidth() == 2);
    assert(motor3.getHeight() == 2);
    assert(motor3.getSpeed() == 50);
    assert(motor3.getDirection() == 1);
    assert(motor3.getStatus() == true);

    // Тест сеттеров Motor
    motor1.setSpeed(75);
    assert(motor1.getSpeed() == 75);

    motor1.setDirection(2);
    assert(motor1.getDirection() == 2);

    motor1.setStatus(true);
    assert(motor1.getStatus() == true);

    // Тест некорректных знач
    motor1.setSpeed(150);
    assert(motor1.getSpeed() == 75);

    motor1.setSpeed(-10);
    assert(motor1.getSpeed() == 75);

    motor1.setDirection(5);
    assert(motor1.getDirection() == 2);

    // Тест rotate()
    Motor motor4;
    motor4.rotate(80, 1);
    assert(motor4.getSpeed() == 80);
    assert(motor4.getDirection() == 1);
    assert(motor4.getStatus() == true);

    // Тест stop()
    motor4.stop();
    assert(motor4.getSpeed() == 0);
    assert(motor4.getDirection() == 0);
    assert(motor4.getStatus() == false);

    Element *elemPtr = &motor2;
    assert(elemPtr->getType() == ElementType::MOTOR);
    assert(elemPtr->getWidth() == 2);
    assert(elemPtr->getHeight() == 2);

    // ТЕСТИРОВАНИЕ КОНСОЛЬНОГО ИНТЕРФЕЙСА
    std::cout << "Testing console interface..." << std::endl;

    // Тест функции getInput с корректным вводом
    {
        stringstream test_input("42");
        streambuf *orig_cin = cin.rdbuf(test_input.rdbuf());
        int result = getInput();
        cin.rdbuf(orig_cin);
        assert(result == 42);
    }

    // Тест функции inputMatrix
    {
        // это типо фейк ввод
        stringstream test_input("0 1 1 0");
        streambuf *orig_cin = cin.rdbuf(test_input.rdbuf());
        vector<vector<char>> matrix = inputMatrix(2, 2);
        cin.rdbuf(orig_cin);
        assert(matrix.size() == 2);
        assert(matrix[0].size() == 2);
        assert(matrix[0][0] == '0');
        assert(matrix[0][1] == '1');
        assert(matrix[1][0] == '1');
        assert(matrix[1][1] == '0');
    }

    // Тест создания элементов через вектор указателей
    vector<Element *> elements;
    elements.push_back(new Element(2, 2, mat));
    elements.push_back(new Motor(3, 3, mat, 30, 2));

    assert(elements.size() == 2);
    assert(elements[0]->getType() == ElementType::ELEMENT);
    assert(elements[1]->getType() == ElementType::MOTOR);

    for (auto elem : elements)
    {
        delete elem;
    }

    std::cout << "All tests have been passed!" << std::endl;

    std::cout << "=== Testing Layer Class ===" << std::endl;

    // Создаем тестовые элементы
    vector<vector<char>> mat1 = {
        {'1', '0', '1'},
        {'0', '1', '0'}};
    Element elem11(3, 2, mat1);

    vector<vector<char>> mat2 = {
        {'0', '1'},
        {'1', '0'}};
    Element elem22(2, 2, mat2);

    vector<vector<char>> mat3 = {
        {'1', '1'},
        {'1', '1'}};
    Element elem33(2, 2, mat3);

    vector<vector<char>> mat4 = {
        {'0', '0'},
        {'0', '0'}};
    Element elem44(2, 2, mat4);
    // Создаем слои
    Layer baseLayer;
    Layer topLayer;

    // Тестируем размещение на базовом слое
    std::cout << "Placing element 1 on base layer..." << std::endl;
    if (baseLayer.placeElement(&elem11, 0, 0))
    {
        std::cout << "Success!" << std::endl;
    }
    else
    {
        std::cout << "Failed!" << std::endl;
    }

    baseLayer.display();

    // Тестируем размещение на верхнем слое с проверкой соединения
    std::cout << "Placing element 2 on top layer..." << std::endl;
    if (topLayer.canPlaceWithLowerLayer(&elem22, 0, 0, &baseLayer))
    {
        std::cout << "Can place!" << std::endl;
        topLayer.placeElement(&elem22, 0, 0);
        std::cout << "Placed successfully!" << std::endl;
    }
    else
    {
        std::cout << "Cannot place - connection issue!" << std::endl;
    }

    topLayer.display();

    // Тестируем ручное размещение
    cout << "Trying to manually place element at (1,1)..." << endl;
    if (baseLayer.placeElement(&elem33, 1, 1))
    {
        cout << "Unexpected success!" << endl;
    }
    else
    {
        cout << "Correctly failed - overlap detected!" << endl;
    }

    cout << "Trying to manually place element at (10,10)..." << endl;
    if (baseLayer.placeElement(&elem33, 10, 10))
    {
        cout << "Success! Layer expanded automatically." << endl;
    }
    baseLayer.display();

    cout << "Trying to manually place element at (10,10)..." << endl;
    if (topLayer.placeElement(&elem44, 2, 2))
    {
        cout << "Success! Layer expanded automatically." << endl;
    }
    topLayer.display();
}

void mainMenu()
{
    vector<Element *> elements;
    int ans;

    while (true)
    {
        std::cout << "Choose:" << std::endl;
        std::cout << "1. Make element" << std::endl;
        std::cout << "2. Make motor" << std::endl;
        std::cout << "3. Show all elements" << std::endl;
        std::cout << "4. Exit" << std::endl;
        ans = getInput();
        if (ans == 1)
        {
            makeElem(elements);
        }
        else if (ans == 2)
        {
            makeMotor(elements);
        }
        else if (ans == 3)
        {
            showElements(elements);
        }
        else if (ans == 4)
        {
            break;
        }
        else
        {
            std::cout << "Incorrect choise, repeat: " << endl;
        }
    }
}

int main()
{
#ifdef RUN_TESTS
    runTests();
#else
    mainMenu();
#endif

    return 0;
}
