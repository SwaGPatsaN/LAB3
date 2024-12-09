#include "graph.h"
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <iostream>
#include "pipe.h"

using namespace std;

// Добавление соединения
void Graph::AddConnection(int inputKS, int outputKS, int pipeID) {
    adjacencyList[inputKS].push_back({ pipeID, inputKS, outputKS });
}

// Вспомогательная функция для топологической сортировки
void TopologicalSortUtil(int ksID, const std::unordered_map<int, std::vector<Connection>>& adjList,
    std::unordered_map<int, bool>& visited,
    std::unordered_map<int, bool>& inStack,
    std::stack<int>& stack) {
    visited[ksID] = true;
    inStack[ksID] = true;

    if (adjList.find(ksID) != adjList.end()) { // Проверяем, есть ли смежные вершины
        for (const auto& connection : adjList.at(ksID)) {
            if (!visited[connection.outputKS]) {
                TopologicalSortUtil(connection.outputKS, adjList, visited, inStack, stack);
            }
            else if (inStack[connection.outputKS]) {
                throw std::runtime_error("Цикл обнаружен в графе."); // Выбрасываем исключение при обнаружении цикла
            }
        }
    }

    inStack[ksID] = false; // Снимаем метку текущей вершины из стека
    stack.push(ksID);      // Добавляем вершину в стек
}



std::vector<int> Graph::TopologicalSort() const {
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, bool> inStack;
    std::stack<int> stack;

    // Инициализируем все вершины как непосещённые
    for (const auto& [ksID, _] : adjacencyList) {
        visited[ksID] = false;
        inStack[ksID] = false;
    }

    // Убедитесь, что все вершины из списка смежности обрабатываются
    for (const auto& [ksID, _] : adjacencyList) {
        if (!visited[ksID]) {
            TopologicalSortUtil(ksID, adjacencyList, visited, inStack, stack);
        }
    }

    // Извлечение результата из стека
    std::vector<int> result;
    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}

// Печать всех соединений в графе
void Graph::PrintConnections() const {
    for (const auto& [inputKS, connections] : adjacencyList) {
        cout << "КС " << inputKS << " соединена с:\n";
        for (const auto& conn : connections) {
            cout << "  КС " << conn.outputKS << " через трубу ID " << conn.pipeID << "\n";
        }
    }
}

bool Graph::IsPipeUsed(int pipeID) const {
    for (const auto& [ksID, connections] : adjacencyList) {
        for (const auto& conn : connections) {
            if (conn.pipeID == pipeID) {
                return true; // Труба уже используется
            }
        }
    }
    return false; // Труба свободна
}

const std::vector<Connection>& Graph::GetConnections(int ksID) const {
    static const std::vector<Connection> empty; // Пустой список для случаев, когда КС не имеет соединений
    auto it = adjacencyList.find(ksID);
    return (it != adjacencyList.end()) ? it->second : empty;
}

void Graph::removePipeConnections(const int& pipeToRemove) {
    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ) {
        // Проверяем каждое соединение в списке
        bool found = false;
        for (const auto& connection : it->second) {
            if (connection.pipeID == pipeToRemove) {
                found = true;
                break;
            }
        }
        // Если труба найдена, удаляем узел
        if (found) {
            it = adjacencyList.erase(it);
            cout << "Соединение с данной трубой удалено\n";
        }
        else {
            ++it;
        }
    }
}