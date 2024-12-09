#include "graph.h"
#include <unordered_map>
#include <vector>
#include <stack>
#include <set>
#include <iostream>
#include "pipe.h"

using namespace std;

// ���������� ����������
void Graph::AddConnection(int inputKS, int outputKS, int pipeID) {
    adjacencyList[inputKS].push_back({ pipeID, inputKS, outputKS });
}

// ��������������� ������� ��� �������������� ����������
void TopologicalSortUtil(int ksID, const std::unordered_map<int, std::vector<Connection>>& adjList,
    std::unordered_map<int, bool>& visited,
    std::unordered_map<int, bool>& inStack,
    std::stack<int>& stack) {
    visited[ksID] = true;
    inStack[ksID] = true;

    if (adjList.find(ksID) != adjList.end()) { // ���������, ���� �� ������� �������
        for (const auto& connection : adjList.at(ksID)) {
            if (!visited[connection.outputKS]) {
                TopologicalSortUtil(connection.outputKS, adjList, visited, inStack, stack);
            }
            else if (inStack[connection.outputKS]) {
                throw std::runtime_error("���� ��������� � �����."); // ����������� ���������� ��� ����������� �����
            }
        }
    }

    inStack[ksID] = false; // ������� ����� ������� ������� �� �����
    stack.push(ksID);      // ��������� ������� � ����
}



std::vector<int> Graph::TopologicalSort() const {
    std::unordered_map<int, bool> visited;
    std::unordered_map<int, bool> inStack;
    std::stack<int> stack;

    // �������������� ��� ������� ��� ������������
    for (const auto& [ksID, _] : adjacencyList) {
        visited[ksID] = false;
        inStack[ksID] = false;
    }

    // ���������, ��� ��� ������� �� ������ ��������� ��������������
    for (const auto& [ksID, _] : adjacencyList) {
        if (!visited[ksID]) {
            TopologicalSortUtil(ksID, adjacencyList, visited, inStack, stack);
        }
    }

    // ���������� ���������� �� �����
    std::vector<int> result;
    while (!stack.empty()) {
        result.push_back(stack.top());
        stack.pop();
    }

    return result;
}

// ������ ���� ���������� � �����
void Graph::PrintConnections() const {
    for (const auto& [inputKS, connections] : adjacencyList) {
        cout << "�� " << inputKS << " ��������� �:\n";
        for (const auto& conn : connections) {
            cout << "  �� " << conn.outputKS << " ����� ����� ID " << conn.pipeID << "\n";
        }
    }
}

bool Graph::IsPipeUsed(int pipeID) const {
    for (const auto& [ksID, connections] : adjacencyList) {
        for (const auto& conn : connections) {
            if (conn.pipeID == pipeID) {
                return true; // ����� ��� ������������
            }
        }
    }
    return false; // ����� ��������
}

const std::vector<Connection>& Graph::GetConnections(int ksID) const {
    static const std::vector<Connection> empty; // ������ ������ ��� �������, ����� �� �� ����� ����������
    auto it = adjacencyList.find(ksID);
    return (it != adjacencyList.end()) ? it->second : empty;
}

void Graph::removePipeConnections(const int& pipeToRemove) {
    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ) {
        // ��������� ������ ���������� � ������
        bool found = false;
        for (const auto& connection : it->second) {
            if (connection.pipeID == pipeToRemove) {
                found = true;
                break;
            }
        }
        // ���� ����� �������, ������� ����
        if (found) {
            it = adjacencyList.erase(it);
            cout << "���������� � ������ ������ �������\n";
        }
        else {
            ++it;
        }
    }
}