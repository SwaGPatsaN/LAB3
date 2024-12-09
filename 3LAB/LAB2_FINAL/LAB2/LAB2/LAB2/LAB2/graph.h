#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include "pipe.h"

// Структура для представления соединения
struct Connection {
    int pipeID;      // ID трубы
    int inputKS;     // ID входной КС
    int outputKS;    // ID выходной КС
};

class Graph {
private:
    std::unordered_map<int, std::vector<Connection>> adjacencyList; // Список смежности
    std::map<int, pipe> connections;

public:
    // Добавить соединение между КС
    void AddConnection(int inputKS, int outputKS, int pipeID);

    // Топологическая сортировка
    std::vector<int> TopologicalSort() const;

    // Печать всех соединений
    void PrintConnections() const;

    bool IsPipeUsed(int pipeID) const;

    const std::vector<Connection>& GetConnections(int ksID) const;

    void removePipeConnections(const int& pipeToRemove);
};
