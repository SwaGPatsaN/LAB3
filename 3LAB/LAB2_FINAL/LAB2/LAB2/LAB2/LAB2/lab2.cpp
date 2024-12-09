#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>
#include <thread>
#include "pipe.h"
#include "ks.h"
#include <map>
#include <unordered_map>
#include "graph.h"
using namespace std;

const int MAX_PIPES = 100;
const int MAX_KS = 50;
Graph graph;

void LogToFile(const string& message, ofstream& logFile) {
	if (logFile.is_open()) {
		logFile << message << endl;
	}
}

bool IsInteger(string s) {
	istringstream iss(s);
	int num;
	return (iss >> num) && iss.eof();
}

int GetPositiveNumber(const string& zapros, ofstream& logFile) {
	string input;
	int value;
	std::cout << zapros;
	while (true) {
		getline(cin, input);
		LogToFile(input, logFile);
		if (input.empty()) {
			cout << "���� �� ������ ���� ������. ���������� �����: ";
			LogToFile("Error. Empty insert >:(", logFile);
			continue;
		}
		if (IsInteger(input)) {
			value = stoi(input);
			if (value > 0) return value;
		}
		std::cout << "������� ������������� ����� \n";
		LogToFile("Error. Non-positive number >:(", logFile);
	}

}

int GetPositiveNumberID(const string& zapros, ofstream& logFile) {
	string input;
	int value;
	std::cout << zapros;
	while (true) {
		getline(cin, input);
		LogToFile(input, logFile);
		if (input.empty()) {
			cout << "���� �� ������ ���� ������. ���������� �����: ";
			LogToFile("Error. Empty insert >:(", logFile);
			continue;
		}
		if (IsInteger(input)) {
			value = stoi(input);
			if (value >= 0) return value;
		}
		std::cout << "������� ������������� ����� ��� 0 \n";
		LogToFile("Error. Non-positive number >:(", logFile);
	}

}

bool GetRepairStatus(const string& zapros, ofstream& logFile) {
	string repairStatus;
	cout << zapros;
	while (true) {
		getline(cin, repairStatus);
		LogToFile(repairStatus, logFile);
		if (repairStatus == "y" || repairStatus == "Y") return true;
		if (repairStatus == "n" || repairStatus == "N") return false;
		cout << "������� y(��) ��� n(���)\n";
		LogToFile("Error. Try y or n >:(", logFile);
	}
}

void PacEditPipes(map<int, pipe>& pipes, int pipeIDCounter, ofstream& logFile) {

	cout << "������� ������� ���� ��� �������������� (����� �������, ��� ��������): ";
	string input;
	getline(cin, input);
	LogToFile(input, logFile);

	stringstream ss(input);
	string indexStr;
	bool inRepair = GetRepairStatus("��������� �� ����� � �������?: ", logFile);
	while (getline(ss, indexStr, ',')) {
		int index = stoi(indexStr);
		if (index >= 0 && index <= pipeIDCounter) {
			pipes[index].SetInRepair(inRepair);
			cout << "������ ������� ����� " << pipes[index].GetName() << " ��������.\n";
		}
		else {
			cout << "�������� ������: " << index << endl;
			LogToFile("Error. Empty index: " + to_string(index), logFile);
		}
	}
}

void PipeEdit(map<int, pipe>& pipes, int pipeIDCounter, ofstream& logFile) {
	if (pipes.size() == 0) {
		cout << "��� ���� ��� ��������������\n";
		return;
	}
	string choice;
	cout << "1. ������������� ���� �����\n";
	cout << "2. �������� �������������� ����\n";
	getline(cin, choice);
	LogToFile((choice), logFile);


	if (choice == "1") {
		string indexs;
		cout << "������� ������ ����� ��� ��������������: ";
		getline(cin, indexs);
		LogToFile(indexs, logFile);


		if (IsInteger(indexs)) {
			auto it = pipes.find(stoi(indexs));
			if (stoi(indexs) < 0 || it == pipes.end()) {
				cout << "�������� ������.\n";
				return;
			}
			int index = stoi(indexs);
			cout << "1. �������� ������ �������\n";
			cout << "2. ������� �����\n";
			getline(cin, choice);
			LogToFile(choice, logFile);

			if (choice == "1") {
				pipes[index].SetInRepair(GetRepairStatus("��������� �� ����� � �������?: ", logFile));
				cout << "������ ������� ����� ��������.\n";
			}
			else if (choice == "2") {
				pipes.erase(index);
				graph.removePipeConnections(index);
				cout << "����� �������.\n";
				LogToFile("����� �������", logFile);
			}
			else {
				cout << "�������� �����.\n";
				LogToFile("������: �������� ����� ��� �������������� ����� �����.", logFile);
			}

		}
	}
	else if (choice == "2") {
		PacEditPipes(pipes, pipeIDCounter, logFile);
	}
	else {
		cout << "�������� �����.\n";
		LogToFile("Error. Wrong choice >:(", logFile);
	}
}
void KSEdit(map<int, ks>& kss, int ksIDCounter, ofstream& logFile) {
	if (kss.size() == 0) {
		cout << "��� �� ��� ��������������\n";
		return;
	}
	int index;
	cout << "������� ������ �� ��� ��������������: ";

	while (!(cin >> index) || index < 0 || index > ksIDCounter) {
		LogToFile(to_string(index), logFile);
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "�������� ������. ���������� �����: ";
		LogToFile("Error. Wrong index >:(", logFile);
	}
	LogToFile(to_string(index), logFile);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "1. �������� ���������� ���������� ������\n";
	cout << "2. ������� ��\n";
	string editChoice;
	getline(cin, editChoice);
	LogToFile(editChoice, logFile);

	while (!IsInteger(editChoice) || (stoi(editChoice) < 1 || stoi(editChoice) > 2)) {
		cout << "������� 1 ��� 2: ";
		LogToFile("Error. Try 1 or 2 >:(", logFile);
		getline(cin, editChoice);
		LogToFile(editChoice, logFile);
	}

	int choice = stoi(editChoice);
	if (choice == 1) {
		int newNumWorkFac;
		while (true) {
			newNumWorkFac = GetPositiveNumber("������� ����� ���������� ���������� ������: ", logFile);
			if (newNumWorkFac <= kss[index].GetNumFac()) {
				break;
			}
			cout << "���������� ���������� ������ ������ ���� ������ ��� ����� ������ ���������� ������.\n";
			LogToFile("Error. Ammount of working factories need to be below total >:(", logFile);
		}
		kss[index].SetNumWorkFac(newNumWorkFac);
		cout << "���������� ���������� ������ ���������.\n";
	}
	else if (choice == 2) {
		kss.erase(index);
		cout << "KC �������\n";
	}
}
void pipeInfo(int id, const pipe p) {
	cout << "Pipe ID: " << id << endl;
	cout << p << endl;
}

void showPipes(map<int, pipe>& pipes) {
	if (pipes.empty()) {
		cout << "��� ��������� ����." << endl;
	}
	for (const auto& item : pipes) {
		int id = item.first;
		const pipe p = item.second;
		pipeInfo(id, p);
	}
}

void ksInfo(int id, const ks k) {
	cout << "KS ID: " << id << endl;
	cout << k << endl;
}

void showKSS(map<int, ks>& kss) {
	if (kss.empty()) {
		cout << "��� ��������� KC." << endl;
	}
	for (const auto& item : kss) {
		int id = item.first;
		const ks k = item.second;
		ksInfo(id, k);
	}
}

void SaveData(map<int, pipe>pipes, int& pipeIDCounter, map<int, ks>kss, int& ksIDCounter) {
	cout << "������� ��� ����� ��� ���������� ������: \n";
	string filename;
	getline(cin, filename);

	ofstream outFile(filename);
	if (!outFile) {
		cout << "������ ��� �������� ����� ��� ���������� ������.\n";
		return;
	}


	outFile << "Pipes" << endl;
	for (int i = 0; i < pipeIDCounter; ++i) {
		if (pipes.contains(i)) {
			outFile << pipes[i].GetName() << endl;
			outFile << pipes[i].GetLength() << endl;
			outFile << pipes[i].GetDiameter() << endl;
			outFile << pipes[i].GetInRepair() << endl;
		}
	}


	outFile << "KS" << endl;
	for (int i = 0; i < ksIDCounter; ++i) {
		if (kss.contains(i)) {
			outFile << kss[i].GetName() << endl;
			outFile << kss[i].GetNumFac() << endl;
			outFile << kss[i].GetNumWorkFac() << endl;
			outFile << kss[i].GetEff() << endl;
		}
	}

	outFile.close();
	cout << "������ ������� ��������� � " << filename << ".\n";
}

void LoadData(map<int, pipe>&pipes, int& pipeIDCounter, map<int, ks>&kss, int& ksIDCounter) {
    cout << "������� ��� ����� ��� �������� ������: ";
    string filename;
    getline(cin, filename);

    ifstream inFile(filename);
    if (!inFile) {
        cout << "������ ��� �������� ����� ��� �������� ������.\n";
        return;
    }

    string line;
    getline(inFile, line);

	if (line == "Pipes") {
		while (getline(inFile, line) && line != "KS") {
			string name = line;
			int length, diameter;
			bool inRepair;

			inFile >> length >> diameter >> inRepair;
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');

			if (pipeIDCounter < MAX_PIPES) {
				pipes[pipeIDCounter] = pipe(name, length, diameter, inRepair);
				pipeIDCounter++;
			}
		}
	}

	if (line == "KS") {
		while (getline(inFile, line)) {
			string name = line;
			int numFac, numWorkFac, eff;

			inFile >> numFac >> numWorkFac >> eff;
			inFile.ignore(numeric_limits<streamsize>::max(), '\n');

			if (ksIDCounter < MAX_KS) {
				kss[ksIDCounter] = ks(name, numFac, numWorkFac, eff);
				ksIDCounter++;
			}
		}
	}

	inFile.close();
	cout << "������ ������� ��������� �� " << filename << ".\n";
}

bool SearchPipe(map<int, pipe>&pipes, int pipeCount, ofstream& logFile) {
	cout << "�������� ������ ��� ������ ����:\n";
	cout << "1. �� ��������\n";
	cout << "2. �� ������� �������\n";
	int p = 0;

	string choice;
	getline(cin, choice);
	LogToFile(choice, logFile);

	if (choice == "1") {
		string searchName;
		cout << "������� �������� �����: ";
		getline(cin, searchName);
		LogToFile(searchName, logFile);
		for (int i = 0; i < pipeCount; ++i) {
			if (pipes[i].GetName().find(searchName) != string::npos) {
				cout << "������� �����:\n";
				cout << "������: " << i <<"\n";
				cout << pipes[i].ToString();
				cout << "\n";
				p++;
			}
		}
		if (p == 0) {
			cout << "�� ������� ���� �� �������\n";
			return 0;
		}
		else if (!(p == 0)) {
			return 1;
		}

	}
	else if (choice == "2") {
		string status;
		cout << "������� ������ ������� (y/n): ";
		getline(cin, status);
		LogToFile(status, logFile);
		while (true) {
			if (status == "y" || status == "Y" || status == "n" || status == "N") {
				break;
			}
			cout << "������� ������ ������ (y/n): ";
			LogToFile("Error.Try y or n >:(", logFile);
			getline(cin, status);
			LogToFile(status, logFile);
		}
		bool inRepair = (status == "y" || status == "Y");
		for (int i = 0; i < pipeCount; ++i) {
			if (pipes[i].GetInRepair() == inRepair) {
				cout << "������� �����:\n";
				cout << "������: " << i << "\n";
				cout << pipes[i].ToString();
				cout << "\n";
				p++;
			}
		}
		if (p == 0) {
			cout << "�� ������� ���� �� �������\n";
			return 0;
		}
		else if (!(p == 0)) {
			return 1;
		}
	}
	else {
		cout << "�������� �����.\n";
		LogToFile("Error. Wrong choice >:(", logFile);
	}
}

bool SearchKS(map<int, ks>&kss, int ksCount, ofstream& logFile) {
	cout << "�������� ������ ��� ������ ��:\n";
	cout << "1. �� ��������\n";
	cout << "2. �� �������� ����������������� �����\n";
	int k = 0;

	string choice;
	getline(cin, choice);
	LogToFile(choice, logFile);

	if (choice == "1") {
		string searchName;
		cout << "������� �������� �� ��� ������: ";
		getline(cin, searchName);
		LogToFile(searchName, logFile);
		for (int i = 0; i < ksCount; ++i) {
			if (kss[i].GetName().find(searchName) != string::npos) {
				cout << "������� ��:\n";
				cout << "������: " << i << "\n";
				cout << kss[i].ToString(); 
				cout << "\n";
				k++;
			}
		}
		if (k == 0) {
			cout << "�� ������� KC �� �������\n";
			return 0;
		}
		else if (!(k == 0)) {
			return 1;
		}
	}
	else if (choice == "2") {
		int percent;
		cout << "������� ������� ����������������� �����: ";
		cin >> percent;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		LogToFile(to_string(percent), logFile);
		for (int i = 0; i < ksCount; ++i) {
			int unusedPercent = 100 * (kss[i].GetNumFac() - kss[i].GetNumWorkFac()) / kss[i].GetNumFac();
			if (unusedPercent == percent) {
				cout << "������� ��:\n";
				cout << "������: " << i << "\n";
				cout << kss[i].ToString();
				cout << "\n";
				k++;
			}
		}
		if (k == 0) {
			cout << "�� ������� KC �� �������\n";
			return 0;
		}
		else if (!(k == 0)) {
			return 1;
		}
	}
	else {
		cout << "�������� �����.\n";
		LogToFile("Error. Wrong choice >:(", logFile);
	}
}

void ConnectKS(Graph& graph, map<int, pipe>& pipes, map<int, ks>& kss, int& pipeIDCounter, ofstream& logFile) {
	int inputKS, outputKS, diameter;
	cout << "������� ID ������� ��: ";
	inputKS = GetPositiveNumberID("", logFile);
	cout << "������� ID �������� ��: ";
	outputKS = GetPositiveNumberID("", logFile);
	cout << "������� ��������� ������� ����� (500, 700, 1000, 1400): ";
	string input;
	while (true) {
		getline(cin, input);
		LogToFile(input, logFile);
		if (input.empty()) {
			cout << "���� �� ������ ���� ������. ���������� �����: ";
			LogToFile("Error. Empty insert >:(", logFile);
			continue;
		}
		if (IsInteger(input)) {
			diameter = stoi(input);
			if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
				break;
			}
		}
		std::cout << "������� ���� �� ������������ ��������\n";
	}

	// ��������, ������ �� ��� ��
	if (inputKS == outputKS) {
		cout << "������: ������� ������ ��.\n";
		return;
	}

	// �������� ������������� ��
	if (kss.find(inputKS) == kss.end() || kss.find(outputKS) == kss.end()) {
		cout << "������: ���� �� ��������� �� �� ����������.\n";
		return;
	}

	// ��������, �� ��������� �� ��� �� ���
	for (const auto& conn : graph.GetConnections(inputKS)) {
		if (conn.outputKS == outputKS) {
			cout << "������: ��� �� ��� ��������� ����� ����� ID " << conn.pipeID << ".\n";
			return;
		}
	}

	// ����� ���������� �����, ������� ��� �� ������������
	int selectedPipeID = -1;
	for (const auto& [id, p] : pipes) {
		if (p.GetDiameter() == diameter && !p.GetInRepair()) {
			// ���������, �� ������������ �� �����
			if (!graph.IsPipeUsed(id)) {
				selectedPipeID = id;
				break;
			}
		}
	}

	// ���� ���������� ����� ���, ������� �����
	if (selectedPipeID == -1) {
		cout << "���������� ����� �� �������, ������� �����.\n";
		pipes[pipeIDCounter] = pipe("GeneratedPipe", 100, diameter, false); // ������ ��������
		selectedPipeID = pipeIDCounter++;
	}

	// ��������� ���������� � ����
	graph.AddConnection(inputKS, outputKS, selectedPipeID);
	cout << "�� ��������� ����� ����� ID: " << selectedPipeID << "\n";
}


void PerformTopologicalSort(Graph& graph, map<int, ks>& kss) {
	try {
		if (kss.empty()) {
			cout << "������: ���� �� �������� ������ (��). ���������� ���������� ����������.\n";
			return;
		}

		vector<int> sortedKS = graph.TopologicalSort();
		if (sortedKS.empty()) {
			cout << "���� �� �������� ���������� ��� �������� ����������.\n";
			return;
		}

		cout << "�������������� ������� ��:\n";
		for (int ksID : sortedKS) {
			cout << ksID << " ";
		}
		cout << endl;
	}
	catch (const std::exception& e) {
		cout << "������: " << e.what() << endl;
	}
}

void PrintGraphConnections(Graph& graph) {
	cout << "���������� � �����:\n";
	graph.PrintConnections();
}




int main() {
	ofstream logFile("C:/Users/05bla/Downloads/LAB2/LAB2/LAB2/log.txt", ios::app); // ��������� ���� ��� ����������
	if (!logFile) {
		cerr << "������ ��� �������� ����� ��� �����������.\n";
		return 1; // ��������� ��������� � ������ ������
	}
	int pipeIDCounter = 0;
	int ksIDCounter = 0;
	map<int, pipe> pipes;
	map<int, ks> kss;
	string choose;
	int pipeCount = 0;
	int ksCount = 0;
	while (true) {
		setlocale(LC_ALL, (""));
		cout << "1. ������� �����\n";
		cout << "2. ������� ��\n";
		cout << "3. �������������� ����\n";
		cout << "4. �������������� ��\n";
		cout << "5. �������� ��� �������\n";
		cout << "6. ���������\n";
		cout << "7. ���������\n";
		cout << "8. �����\n";
		cout << "9. ��������� ��\n";
		cout << "10. �������������� ����������\n";
		cout << "11. �����\n";
		getline(cin, choose);
		LogToFile(choose, logFile);
		while (!IsInteger(choose)) {
			cout << "������� ����� �� 1 �� 9" << endl;
			LogToFile("Error. Try 1 to 9 >:(", logFile);
			getline(cin, choose);
			LogToFile(choose, logFile);
		}

		switch (stoi(choose)) {
		case 1: {
			if (pipeIDCounter >= MAX_PIPES) {
				cout << "��������� �������� ����" << endl;
				break;
			}
			string name;
			cout << "������� �������� �����: ";
			while (true) {
				getline(cin, name);
				LogToFile(name, logFile);
				if (name.empty()) {
					cout << "��� �� ����� ���� ������ �������. ���������� �����: ";
					LogToFile("Error. Name can't be empty >:(", logFile);
				}
				else {
					break;
				}
			}

			int length = GetPositiveNumber("������� ����� �����: ", logFile);
			int diameter = GetPositiveNumber("������� ������� �����: ", logFile);
			bool inRepair = GetRepairStatus("��������� �� ����� � �������? (y/n): ", logFile);

			pipes[pipeIDCounter] = pipe(name, length, diameter, inRepair);
			pipeCount++;
			pipeIDCounter++;
			cout << "����� �������\n";
		}
			  break;
		case 2: {
			if (ksIDCounter >= MAX_KS) {
				cout << "���������� ������������� ���������� ��" << endl;
				break;
			}
			string name;
			cout << "������� �������� K�: ";
			while (true) {
				getline(cin, name);
				LogToFile(name, logFile);
				if (name.empty()) {
					cout << "��� �� ����� ���� ������ �������. ���������� �����: ";
					LogToFile("Error.Name can't be empty >:(", logFile);
				}
				else {
					break;
				}
			}
			int NumFac = GetPositiveNumber("������� ���������� ������: ", logFile);
			int NumWorkFac;
			while (true) {
				NumWorkFac = GetPositiveNumber("������� ���������� ���������� ������: ", logFile);
				if (NumWorkFac <= NumFac) {
					break;
				}
				cout << "���������� ��������� ������ ������ ���� ������ ������\n";
				LogToFile("Error. Ammount of working factories need to be below total >:(", logFile);
			}

			int eff;
			while (true) {
				eff = GetPositiveNumber("������� ������������� �� (0-100): ", logFile);
				if (eff >= 0 && eff <= 100) {
					break;
				}
				cout << "������� ����� �� 0 �� 100\n";
				LogToFile("Error. Try 0 to 100 >:(", logFile);
			}

			kss[ksIDCounter] = ks(name, NumFac, NumWorkFac, eff);
			ksCount++;
			ksIDCounter++;
			cout << "�� �������\n";
		}
			  break;
		case 3: {
			PipeEdit(pipes, pipeIDCounter, logFile);
			}
			break;
		case 4: {
			KSEdit(kss, ksIDCounter, logFile);
		}
			  break;
		case 5: {
			if (pipes.size() == 0) {
				cout << "��� ��������� ����\n";
			}
			else {
				cout << "������ ����:\n";
				showPipes(pipes);
			}
			if (kss.size() == 0) {
				cout << "��� ��������� ��\n";
			}
			else {
				cout << "������ KC:\n";
				showKSS(kss);
			}
			break;
		case 6:
			SaveData(pipes, pipeIDCounter, kss, ksIDCounter);
			break;
		case 7:
			LoadData(pipes, pipeIDCounter, kss, ksIDCounter);
			break;
		case 8:
			cout << "�������� ������ ��� ������:\n";
			cout << "1. �����\n";
			cout << "2. ��\n";
			getline(cin, choose);
			LogToFile(choose, logFile);

			if (choose == "1") {
				if (SearchPipe(pipes, pipeIDCounter, logFile) == 1) {
					PipeEdit(pipes, pipeIDCounter, logFile);
				}
			}
			else if (choose == "2") {
				if (SearchKS(kss, ksIDCounter, logFile) == 1) {
					KSEdit(kss, ksIDCounter, logFile);
				}
			}
			else {
				cout << "�������� �����.\n";
				LogToFile("Error. Wrong choice >:(", logFile);
			}
			break;
		case 9:
			ConnectKS(graph, pipes, kss, pipeIDCounter, logFile);
			PrintGraphConnections(graph);
			break;
		case 10:
			PerformTopologicalSort(graph, kss);
			break;
		case 11:
			return 0;
			break;
		default:
			cout << "������� ����� �� 1 �� 11" << endl;
			break;
		}

		}
	}
}