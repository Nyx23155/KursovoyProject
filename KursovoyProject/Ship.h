#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Ship {
protected:
    int size;         // Розмір корабля
    int health;       // Залишилася кількість частин корабля
    vector<pair<int, int>> coordinates; // Координати корабля

public:
    Ship(int size);   // Конструктор корабля

    int getSize() const;   // Отримати розмір
    int getHealth() const; // Отримати поточне здоров'я
    void hit();            // Отримати ушкодження (зменшити здоров'я)
    bool isSunk() const;   // Перевірка: корабель потоплений?

    void setCoordinates(const vector<pair<int, int>>& coords); // Встановити координати
    const vector<pair<int, int>>& getCoordinates() const;      // Отримати координати
};
