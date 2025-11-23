#include "element.h"
#include "layer.h"
#include "scheme.h"
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

void motorControlMenu(Motor *motor) {
    while (true)
    {
        std::cout << "=== Motor Control ===" << std::endl;
        std::cout << "1. Stop" << std::endl;
        std::cout << "2. Start rotating" << std::endl;
        std::cout << "3. Change direction" << std::endl;
        std::cout << "4. Change speed" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "Choose action: ";

        int choice = getInput();

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

void makeElem(vector<Element *> &elements)
{
    cout << "MAKING ELEMENT" << endl;
    int width, height;
    while (true) {
        std::cout << "Enter width:" << std::endl;
        width = getInput();
        std::cout << "Enter height:" << std::endl;
        height = getInput();
        if (width <= 0 || height <= 0) std::cout << "Incorrect input" << std::endl;
        else {
            break;
        }  
    }
    std::vector<std::vector<char>> matrix = inputMatrix(width, height);
    Element *newElement = new Element(width, height, matrix);
    elements.push_back(newElement);
    std::cout << "Element is made!" << std::endl;
}

void makeMotor(vector<Element *> &elements)
{
    cout << "MAKING MOTOR" << endl;
    int width, height;
    while (true) {
        std::cout << "Enter width:" << std::endl;
        width = getInput();
        std::cout << "Enter height:" << std::endl;
        height = getInput();
        if (width <= 0 || height <= 0) std::cout << "Incorrect input" << std::endl;
        else {
            break;
        }  
    }
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
    std::vector<std::vector<char>> matrix = inputMatrix(width, height);
    Motor *newMotor = new Motor(width, height, matrix, speed, direction);
    elements.push_back(newMotor);
    std::cout << "Motor is made!" << std::endl;
}

void controlMotor(vector<Element*> &elements)
{
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
                motorControlMenu(motor);
            }
            else
            {
                std::cout << "No actions with this element" << std::endl;
            }

            break;
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
}

void createNewScheme(Scheme& scheme) {
    scheme = Scheme(); // Создаем новую схему
    // Автоматически создаем базовый слой
    scheme.createLayer();
    std::cout << "New scheme created with base layer!" << std::endl;
}

void addElementToScheme(Scheme& scheme, vector<Element*>& elements) {
    if (scheme.getLayerCount() == 0) {
        std::cout << "Error: No layers in scheme! Create a scheme first." << std::endl;
        return;
    }
    
    if (elements.empty()) {
        std::cout << "No elements available! Create elements first." << std::endl;
        return;
    }
    
    // Выбор элемента
    std::cout << "=== Available Elements ===" << std::endl;
    for (int i = 0; i < elements.size(); ++i) {
        std::cout << i + 1 << ". " << (elements[i]->getType() == ElementType::MOTOR ? "Motor" : "Element")
             << " (" << elements[i]->getWidth() << "x" << elements[i]->getHeight() << ")" << std::endl;
    }
    
    std::cout << "Select element (1-" << elements.size() << "): ";
    int elemChoice = getInput();
    if (elemChoice < 1 || elemChoice > elements.size()) {
        std::cout << "Invalid element choice!" << std::endl;
        return;
    }
    Element* selectedElem = elements[elemChoice - 1];
    
    // Выбор слоя
    std::cout << "=== Available Layers ===" << std::endl;
    for (int i = 0; i < scheme.getLayerCount(); ++i) {
        Layer* layer = scheme.getLayer(i);
        std::cout << i << ": " << layer->getElements().size() << " elements, "
             << "size: " << layer->getWidth() << "x" << layer->getHeight() << std::endl;
    }
    
    std::cout << "Select layer (0-" << scheme.getLayerCount() - 1 << "): ";
    int layerChoice = getInput();
    if (layerChoice < 0 || layerChoice >= scheme.getLayerCount()) {
        std::cout << "Invalid layer choice!" << std::endl;
        return;
    }
    
    // Ввод координат
    std::cout << "Enter X coordinate: ";
    int x = getInput();
    std::cout << "Enter Y coordinate: ";
    int y = getInput();
    
    // Добавление элемента
    if (scheme.addElement(selectedElem, layerChoice, x, y)) {
        std::cout << "Element successfully added to layer " << layerChoice << " at (" << x << "," << y << ")" << std::endl;
    } else {
        std::cout << "Failed to add element! Check for overlaps or connection issues." << std::endl;
    }
}

// ТЕсты
void runTests()
{
    std::cout << "Running tests...\n" << std::endl;

    // ТЕСТИРОВАНИЕ ELEMENT
    std::cout << "TESTING ELEMENT..." << std::endl;

    // Конструктор по умолчанию
    Element elem1;
    assert(elem1.getWidth() == 0);
    assert(elem1.getHeight() == 0);
    assert(elem1.getCell(0, 0) == ' ');

    // Конструктор инициализации
    std::vector<std::vector<char>> mat = {
        {'0', '1', '0'},
        {'1', '0', '1'},
        {'0', '1', '0'}
    };
    Element elem2(3, 3, mat);
    assert(elem2.getWidth() == 3);
    assert(elem2.getHeight() == 3);
    assert(elem2.getCell(0, 1) == '1');
    assert(elem2.getCell(1, 1) == '0');
    assert(elem2.getCell(1, 0) == '1');

    // Конструктор копирования
    Element elem3(elem2);
    assert(elem3.getWidth() == 3);
    assert(elem3.getHeight() == 3);
    assert(elem3.getCell(1, 1) == '0');

    // Сеттеры
    elem1.setWidth(5);
    elem1.setHeight(4);
    assert(elem1.getWidth() == 5);
    assert(elem1.getHeight() == 4);

    elem2.setCell(0, 0, '1');
    assert(elem2.getCell(0, 0) == '1');

    // Некорректные координаты
    char original = elem2.getCell(10, 10);
    elem2.setCell(10, 10, '1');
    assert(elem2.getCell(10, 10) == original); // не изменилось

    // setMatrix
    std::vector<std::vector<char>> newMat = {
        {'1', '1'},
        {'1', '1'}
    };
    elem1.setMatrix(newMat);
    assert(elem1.getWidth() == 2);
    assert(elem1.getHeight() == 2);
    assert(elem1.getCell(0, 0) == '1');

    // getMatrix
    const auto& refMat = elem2.getMatrix();
    assert(refMat.size() == 3);
    assert(refMat[0].size() == 3);

    // getType
    assert(elem1.getType() == ElementType::ELEMENT);


    // ТЕСТИРОВАНИЕ MOTOR
    std::cout << "TESTING MOTOR..." << std::endl;
    Motor motor1;
    assert(motor1.getWidth() == 0);
    assert(motor1.getHeight() == 0);
    assert(motor1.getSpeed() == 0);
    assert(motor1.getDirection() == 0);
    assert(motor1.getStatus() == false);
    assert(motor1.getType() == ElementType::MOTOR);

    // Конструктор инициализации
    std::vector<std::vector<char>> motorMat = {
        {'1', '0'},
        {'0', '1'}
    };
    Motor motor2(2, 2, motorMat, 80, 1);
    assert(motor2.getWidth() == 2);
    assert(motor2.getHeight() == 2);
    assert(motor2.getSpeed() == 80);
    assert(motor2.getDirection() == 1);
    assert(motor2.getStatus() == true);
    assert(motor2.getType() == ElementType::MOTOR);

    // Конструктор копирования
    Motor motor3(motor2);
    assert(motor3.getSpeed() == 80);
    assert(motor3.getDirection() == 1);
    assert(motor3.getStatus() == true);

    // Сеттеры
    motor1.setSpeed(75);
    assert(motor1.getSpeed() == 75);
    motor1.setSpeed(150);  // >100
    assert(motor1.getSpeed() == 75);
    motor1.setSpeed(-5);
    assert(motor1.getSpeed() == 75);

    motor1.setDirection(2);
    assert(motor1.getDirection() == 2);
    motor1.setDirection(3);  // invalid
    assert(motor1.getDirection() == 2);

    // rotate()
    motor1.rotate(60, 2);
    assert(motor1.getSpeed() == 60);
    assert(motor1.getDirection() == 2);
    assert(motor1.getStatus() == true);

    // stop()
    motor1.stop();
    assert(motor1.getSpeed() == 0);
    assert(motor1.getDirection() == 0);
    assert(motor1.getStatus() == false);

    // Проверка полиморфизма
    Element* elemPtr = &motor2;
    assert(elemPtr->getType() == ElementType::MOTOR);
    assert(elemPtr->getWidth() == 2);


    // ТЕСТИРОВАНИЕ LAYER
    std::cout << "TESTING LAYER..." << std::endl;

    Layer layer1;
    assert(layer1.isEmpty() == true);
    assert(layer1.getWidth() == 0);
    assert(layer1.getHeight() == 0);

    // placeElement + hasOverlap
    Element baseElem(3, 3, mat);
    assert(layer1.placeElement(&baseElem, 0, 0) == true);
    assert(layer1.placeElement(&baseElem, 1, 1) == false); // overlap
    assert(layer1.getElements().size() == 1); // getElements()

    // getCell
    assert(layer1.getCell(0, 0) == '0');
    assert(layer1.getCell(1, 0) == '1');

    // canPlaceWithLowerLayer — проверка соединения
    Layer lowerLayer;
    Layer upperLayer;
    lowerLayer.placeElement(&baseElem, 0, 0);

    // Элемент с '1' на позиции, где в нижнем слое не '0' — не должен соединяться
    assert(upperLayer.canPlaceWithLowerLayer(&baseElem, 0, 0, &lowerLayer) == false);

    // Элемент с '1' там, где внизу '0' — должен соединяться
    std::vector<std::vector<char>> connectMat = {
        {'0', '0'},
        {'0', '0'}
    };
    Element connector(2, 2, connectMat);
    assert(upperLayer.canPlaceWithLowerLayer(&connector, 0, 0, &lowerLayer) == true);

    // Удаление элемента
    layer1.removeElement(0);
    assert(layer1.isEmpty() == true);

    // Границы
    layer1.placeElement(&baseElem, 10, 10);
    assert(layer1.getWidth() == 3);
    assert(layer1.getHeight() == 3);
    assert(layer1.getMinX() == 10);
    assert(layer1.getMaxX() == 12);

    // ТЕСТИРОВАНИЕ SCHEME
    std::cout << "TESTING SCHEME..." << std::endl;
    Scheme scheme;
    assert(scheme.getLayerCount() == 0);

    int layer0 = scheme.createLayer();
    assert(layer0 == 0);
    assert(scheme.getLayerCount() == 1);

    Layer* l0 = scheme.getLayer(0);
    assert(l0 != nullptr);
    assert(scheme.getLayer(1) == nullptr);

    // Добавление элемента
    assert(scheme.addElement(&baseElem, 0, 0, 0) == true);
    assert(scheme.addElement(&baseElem, 0, 0, 0) == false); // overlap

    // Создаём второй слой
    scheme.createLayer();

    // Проверка валидации соединений между слоями
    assert(scheme.validateStructure() == true); // пока нет верхнего элемента

    // Попытка корректного соединения
    Element topElem(1, 1, std::vector<std::vector<char>>(1, std::vector<char>(1, '1')));
    assert(scheme.addElement(&topElem, 1, 0, 0) == true); // '1' на '0'

    // Попытка некорректного соединения
    Element baseTop(1, 1, std::vector<std::vector<char>>(1, std::vector<char>(1, '0')));
    assert(scheme.addElement(&baseTop, 1, 0, 0) == false); // '0' на '0'

    // Валидация — должна пройти
    assert(scheme.validateStructure() == true);

    // Удаление элемента и слоя
    assert(scheme.removeElement(0, 0) == true);
    assert(scheme.removeLayer(1) == true);
    assert(scheme.getLayerCount() == 1);

    // Глубокое копирование Scheme
    Scheme schemeCopy(scheme);
    assert(schemeCopy.getLayerCount() == 1);
    assert(schemeCopy.getLayer(0)->isEmpty() == true);


    // ТЕСТИРОВАНИЕ КОНСОЛЬНОГО ИНТЕРФЕЙСА
    std::cout << "TESTING CONSOLE INTERFACE..." << std::endl;

    // Тест функции getInput с корректным вводом
    {
        stringstream test_input("42");
        streambuf *orig_cin = cin.rdbuf(test_input.rdbuf());
        int result = getInput();
        cin.rdbuf(orig_cin);
        assert(result == 42);
    }

    // inputMatrix
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

    // Возвращаем обратно
    std::cin.rdbuf(std::cin.rdbuf());


    // ТЕСТИРОВАНИЕ УПРАВЛЕНИЯ ЭЛЕМЕНТАМИ И СХЕМОЙ
    std::cout << "TESTING ELEMENTS AND SCHEME CONTROL..." << std::endl;
    std::vector<Element*> elements;
    elements.push_back(new Element(2, 2, mat));
    elements.push_back(new Motor(2, 2, motorMat, 50, 1));

    assert(elements[0]->getType() == ElementType::ELEMENT);
    assert(elements[1]->getType() == ElementType::MOTOR);

    Scheme testScheme;
    testScheme.createLayer();
    assert(testScheme.getLayerCount() == 1);

    // Добавление элемента в схему
    assert(testScheme.addElement(elements[0], 0, 0, 0) == true);
    assert(testScheme.addElement(elements[1], 0, 2, 2) == true);

    // Статистика
    testScheme.getStats(); // не падает

    // Валидация
    assert(testScheme.validateStructure() == true);

    // Удаление
    assert(testScheme.removeElement(0, 0) == true);
    assert(testScheme.removeLayer(0) == true);

    // Очистка
    for (auto e : elements) {
        delete e;
    }

    std::cout << "\nAll tests have been passed!" << std::endl;
}
void manageElements(vector<Element*> &elements) {
    while (true) {
        std::cout << "=== ELEMENT MANAGER ===" << std::endl;
        std::cout << "1. Make element" << std::endl;
        std::cout << "2. Make motor" << std::endl;
        std::cout << "3. Show all elements" << std::endl;
        std::cout << "4. Change element" << std::endl;
        std::cout << "5. Back to Main Menu" << std::endl;
        std::cout << "Choose: ";
        int subChoice = getInput();
                
        if (subChoice == 1) makeElem(elements);
        else if (subChoice == 2) makeMotor(elements);
        else if (subChoice == 3) showElements(elements);
        else if (subChoice == 4) {
            showElements(elements);
            controlMotor(elements);
        }
        else if (subChoice == 5) break;
        else std::cout << "Invalid choice!" << std::endl;
    }
}

void manageLayers(Scheme& scheme) {
    while (true) {
        std::cout << "=== Layer Management ===" << std::endl;
        std::cout << "1. Create new layer" << std::endl;
        std::cout << "2. Remove layer" << std::endl;
        std::cout << "3. Show current layers" << std::endl;
        std::cout << "4. Back to main menu" << std::endl;
        std::cout << "Choose action: ";
        
        int choice = getInput();
        
        switch (choice) {
            case 1: {
                int newLayerIndex = scheme.createLayer();
                std::cout << "New layer created with index: " << newLayerIndex << std::endl;
                break;
            }
            case 2: {
                std::cout << "Enter layer index to remove: ";
                int layerIndex = getInput();
                if (scheme.removeLayer(layerIndex)) {
                    std::cout << "Layer " << layerIndex << " removed successfully!" << std::endl;
                } else {
                    std::cout << "Failed to remove layer!" << std::endl;
                }
                break;
            }
            case 3: {
                scheme.display();
                break;
            }
            case 4:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void removeElementFromScheme(Scheme& scheme) {
    if (scheme.getLayerCount() == 0) {
        std::cout << "No layers in scheme!" << std::endl;
        return;
    }
    
    // Выбор слоя
    std::cout << "Select layer (0-" << scheme.getLayerCount() - 1 << "): ";
    int layerChoice = getInput();
    if (layerChoice < 0 || layerChoice >= scheme.getLayerCount()) {
        std::cout << "Invalid layer choice!" << std::endl;
        return;
    }
    
    Layer* layer = scheme.getLayer(layerChoice);
    if (layer->isEmpty()) {
        std::cout << "Layer " << layerChoice << " is empty!" << std::endl;
        return;
    }
    
    // Показ элементов на слое
    std::cout << "=== Elements on Layer " << layerChoice << " ===" << std::endl;
    const auto& elements = layer->getElements();
    for (int i = 0; i < elements.size(); ++i) {
        Element* elem = elements[i].first;
        int x = elements[i].second.first;
        int y = elements[i].second.second;
        std::cout << i << ". " << (elem->getType() == ElementType::MOTOR ? "Motor" : "Element")
             << " at (" << x << "," << y << "), size: " << elem->getWidth() << "x" << elem->getHeight() << std::endl;
    }
    
    std::cout << "Select element to remove (0-" << elements.size() - 1 << "): ";
    int elemChoice = getInput();
    if (elemChoice < 0 || elemChoice >= elements.size()) {
        std::cout << "Invalid element choice!" << std::endl;
        return;
    }
    
    if (scheme.removeElement(layerChoice, elemChoice)) {
        std::cout << "Element removed successfully!" << std::endl;
    } else {
        std::cout << "Failed to remove element!" << std::endl;
    }
}

void schemeMenu(vector<Element*> &elements, Scheme &scheme) {
    
    while (true) {
        std::cout << "=== SCHEME BUILDER ===" << std::endl;
        std::cout << "1. Create new scheme" << std::endl;
        std::cout << "2. Add element to scheme" << std::endl;
        std::cout << "3. Remove element from scheme" << std::endl;
        std::cout << "4. Manage layers" << std::endl;
        std::cout << "5. Display scheme" << std::endl;
        std::cout << "6. Show statistics" << std::endl;
        std::cout << "7. Validate structure" << std::endl;
        std::cout << "8. Back to main menu" << std::endl;
        std::cout << "Choose action: ";
        
        int choice = getInput();
        
        switch (choice) {
            case 1:
                createNewScheme(scheme);
                break;
            case 2:
                showElements(elements);
                addElementToScheme(scheme, elements);
                break;
            case 3:
                removeElementFromScheme(scheme);
                break;
            case 4:
                manageLayers(scheme);
                break;
            case 5:
                scheme.display();
                break;
            case 6:
                scheme.getStats();
                break;
            case 7:
                if (scheme.validateStructure()) {
                    std::cout << "Structure is valid!" << std::endl;
                } else {
                    std::cout << "Structure has issues!" << std::endl;
                }
                break;
            case 8:
                return;
            default:
                std::cout << "Invalid choice!" << std::endl;
        }
    }
}

void mainMenu() {
    vector<Element*> elements;
    Scheme scheme;
    int ans;

    while (true) {
        std::cout << "=== MAIN MENU ===" << std::endl;
        std::cout << "1. Element & Motor Management" << std::endl;
        std::cout << "2. Scheme Builder" << std::endl;
        std::cout << "3. Run Tests" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choose: ";
        ans = getInput();
        
        if (ans == 1) {
            // Старое меню управления элементами
            manageElements(elements);
        }
        else if (ans == 2) {
            schemeMenu(elements, scheme);
        }
        else if (ans == 3) {
            runTests();
        }
        else if (ans == 4) {
            break;
        }
        else {
            std::cout << "Invalid choice!" << std::endl;
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
