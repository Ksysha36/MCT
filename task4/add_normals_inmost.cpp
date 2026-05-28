#include "inmost.h"
#include <cmath>
#include <iostream>

using namespace INMOST;

int main(int argc, char* argv[]) {
    // Константы для Коломны (из вашего .geo)
    const double CX = 32.549019607843135;
    const double CY = 14.880952380952387;

    // Загружаем сетку (файл colomna.vtk лежит в папке task4)
    Mesh m;
    m.Load("../colomna.vtk");
    std::cout << "Сетка загружена. Узлов: " << m.NumberOfNodes() << std::endl;

    // Создаём векторный тег на узлах
    Tag normal_tag = m.CreateTag("NORMAL", DATA_REAL, NODE, NONE, 3);

    // Обходим все узлы
    for (auto node = m.BeginNode(); node != m.EndNode(); ++node) {
        double x = node->Coords()[0];
        double y = node->Coords()[1];
        double rx = x - CX;
        double ry = y - CY;
        
        // Ненормированная нормаль
        double nx = -ry;
        double ny =  rx;

        // Присваиваем компоненты тега
        node->RealArray(normal_tag)[0] = nx;
        node->RealArray(normal_tag)[1] = ny;
        node->RealArray(normal_tag)[2] = 0.0;
    }

    // Сохраняем результат
    m.Save("../colomna_normals.vtk");
    std::cout << "Сохранено: colomna_normals.vtk" << std::endl;

    return 0;
}