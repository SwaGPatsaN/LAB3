#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <iostream>
#include <string>
#include "pipe.h"

// ��������� ��� ������������� ����������
struct Connection {
    int pipeID;      // ID �����
    int inputKS;     // ID ������� ��
    int outputKS;    // ID �������� ��
};

class Graph {
private:
    std::unordered_map<int, std::vector<Connection>> adjacencyList; // ������ ���������
    std::map<int, pipe> connections;

public:
    // �������� ���������� ����� ��
    void AddConnection(int inputKS, int outputKS, int pipeID);

    // �������������� ����������
    std::vector<int> TopologicalSort() const;

    // ������ ���� ����������
    void PrintConnections() const;

    bool IsPipeUsed(int pipeID) const;

    const std::vector<Connection>& GetConnections(int ksID) const;

    void removePipeConnections(const int& pipeToRemove);
};
