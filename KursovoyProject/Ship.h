#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Ship {
protected:
    int size;         // Размер корабля
    int health;       // Оставшееся количество частей корабля
    vector<pair<int, int>> coordinates; // Координаты корабля

public:
    Ship(int size);   // Конструктор корабля

    int getSize() const;   // Получить размер
    int getHealth() const; // Получить текущее здоровье
    void hit();            // Получить урон (уменьшить здоровье)
    bool isSunk() const;   // Проверка: корабль потоплен?

    void setCoordinates(const vector<pair<int, int>>& coords); // Установить координаты
    const vector<pair<int, int>>& getCoordinates() const;      // Получить координаты
};