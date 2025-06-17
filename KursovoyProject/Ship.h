#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Ship {
protected:
    int size;         // ������ �������
    int health;       // ���������� ���������� ������ �������
    vector<pair<int, int>> coordinates; // ���������� �������

public:
    Ship(int size);   // ����������� �������

    int getSize() const;   // �������� ������
    int getHealth() const; // �������� ������� ��������
    void hit();            // �������� ���� (��������� ��������)
    bool isSunk() const;   // ��������: ������� ��������?

    void setCoordinates(const vector<pair<int, int>>& coords); // ���������� ����������
    const vector<pair<int, int>>& getCoordinates() const;      // �������� ����������
};