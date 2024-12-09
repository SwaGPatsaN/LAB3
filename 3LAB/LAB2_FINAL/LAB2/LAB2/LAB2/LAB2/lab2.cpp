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
			cout << "Ввод не должен быть пустым. Попробуйте снова: ";
			LogToFile("Error. Empty insert >:(", logFile);
			continue;
		}
		if (IsInteger(input)) {
			value = stoi(input);
			if (value > 0) return value;
		}
		std::cout << "Введите положительное число \n";
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
			cout << "Ввод не должен быть пустым. Попробуйте снова: ";
			LogToFile("Error. Empty insert >:(", logFile);
			continue;
		}
		if (IsInteger(input)) {
			value = stoi(input);
			if (value >= 0) return value;
		}
		std::cout << "Введите положительное число или 0 \n";
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
		cout << "Введите y(да) или n(нет)\n";
		LogToFile("Error. Try y or n >:(", logFile);
	}
}

void PacEditPipes(map<int, pipe>& pipes, int pipeIDCounter, ofstream& logFile) {

	cout << "Введите индексы труб для редактирования (через запятую, без пробелов): ";
	string input;
	getline(cin, input);
	LogToFile(input, logFile);

	stringstream ss(input);
	string indexStr;
	bool inRepair = GetRepairStatus("Находятся ли трубы в ремонте?: ", logFile);
	while (getline(ss, indexStr, ',')) {
		int index = stoi(indexStr);
		if (index >= 0 && index <= pipeIDCounter) {
			pipes[index].SetInRepair(inRepair);
			cout << "Статус ремонта трубы " << pipes[index].GetName() << " обновлен.\n";
		}
		else {
			cout << "Неверный индекс: " << index << endl;
			LogToFile("Error. Empty index: " + to_string(index), logFile);
		}
	}
}

void PipeEdit(map<int, pipe>& pipes, int pipeIDCounter, ofstream& logFile) {
	if (pipes.size() == 0) {
		cout << "Нет труб для редактирования\n";
		return;
	}
	string choice;
	cout << "1. Редактировать одну трубу\n";
	cout << "2. Пакетное редактирование труб\n";
	getline(cin, choice);
	LogToFile((choice), logFile);


	if (choice == "1") {
		string indexs;
		cout << "Введите индекс трубы для редактирования: ";
		getline(cin, indexs);
		LogToFile(indexs, logFile);


		if (IsInteger(indexs)) {
			auto it = pipes.find(stoi(indexs));
			if (stoi(indexs) < 0 || it == pipes.end()) {
				cout << "Неверный индекс.\n";
				return;
			}
			int index = stoi(indexs);
			cout << "1. Изменить статус починки\n";
			cout << "2. Удалить трубу\n";
			getline(cin, choice);
			LogToFile(choice, logFile);

			if (choice == "1") {
				pipes[index].SetInRepair(GetRepairStatus("Находится ли труба в ремонте?: ", logFile));
				cout << "Статус ремонта трубы обновлен.\n";
			}
			else if (choice == "2") {
				pipes.erase(index);
				graph.removePipeConnections(index);
				cout << "Труба удалена.\n";
				LogToFile("Труба удалена", logFile);
			}
			else {
				cout << "Неверный выбор.\n";
				LogToFile("Ошибка: Неверный выбор при редактировании одной трубы.", logFile);
			}

		}
	}
	else if (choice == "2") {
		PacEditPipes(pipes, pipeIDCounter, logFile);
	}
	else {
		cout << "Неверный выбор.\n";
		LogToFile("Error. Wrong choice >:(", logFile);
	}
}
void KSEdit(map<int, ks>& kss, int ksIDCounter, ofstream& logFile) {
	if (kss.size() == 0) {
		cout << "Нет КС для редактирования\n";
		return;
	}
	int index;
	cout << "Введите индекс КС для редактирования: ";

	while (!(cin >> index) || index < 0 || index > ksIDCounter) {
		LogToFile(to_string(index), logFile);
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Неверный индекс. Попробуйте снова: ";
		LogToFile("Error. Wrong index >:(", logFile);
	}
	LogToFile(to_string(index), logFile);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');

	cout << "1. Изменить количество работающих фабрик\n";
	cout << "2. Удалить КС\n";
	string editChoice;
	getline(cin, editChoice);
	LogToFile(editChoice, logFile);

	while (!IsInteger(editChoice) || (stoi(editChoice) < 1 || stoi(editChoice) > 2)) {
		cout << "Введите 1 или 2: ";
		LogToFile("Error. Try 1 or 2 >:(", logFile);
		getline(cin, editChoice);
		LogToFile(editChoice, logFile);
	}

	int choice = stoi(editChoice);
	if (choice == 1) {
		int newNumWorkFac;
		while (true) {
			newNumWorkFac = GetPositiveNumber("Введите новое количество работающих фабрик: ", logFile);
			if (newNumWorkFac <= kss[index].GetNumFac()) {
				break;
			}
			cout << "Количество работающих фабрик должно быть меньше или равно общему количеству фабрик.\n";
			LogToFile("Error. Ammount of working factories need to be below total >:(", logFile);
		}
		kss[index].SetNumWorkFac(newNumWorkFac);
		cout << "Количество работающих фабрик обновлено.\n";
	}
	else if (choice == 2) {
		kss.erase(index);
		cout << "KC удалена\n";
	}
}
void pipeInfo(int id, const pipe p) {
	cout << "Pipe ID: " << id << endl;
	cout << p << endl;
}

void showPipes(map<int, pipe>& pipes) {
	if (pipes.empty()) {
		cout << "Нет созданных труб." << endl;
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
		cout << "Нет созданных KC." << endl;
	}
	for (const auto& item : kss) {
		int id = item.first;
		const ks k = item.second;
		ksInfo(id, k);
	}
}

void SaveData(map<int, pipe>pipes, int& pipeIDCounter, map<int, ks>kss, int& ksIDCounter) {
	cout << "Введите имя файла для сохранения данных: \n";
	string filename;
	getline(cin, filename);

	ofstream outFile(filename);
	if (!outFile) {
		cout << "Ошибка при открытии файла для сохранения данных.\n";
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
	cout << "Данные успешно сохранены в " << filename << ".\n";
}

void LoadData(map<int, pipe>&pipes, int& pipeIDCounter, map<int, ks>&kss, int& ksIDCounter) {
    cout << "Введите имя файла для загрузки данных: ";
    string filename;
    getline(cin, filename);

    ifstream inFile(filename);
    if (!inFile) {
        cout << "Ошибка при открытии файла для загрузки данных.\n";
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
	cout << "Данные успешно загружены из " << filename << ".\n";
}

bool SearchPipe(map<int, pipe>&pipes, int pipeCount, ofstream& logFile) {
	cout << "Выберите фильтр для поиска труб:\n";
	cout << "1. По названию\n";
	cout << "2. По статусу ремонта\n";
	int p = 0;

	string choice;
	getline(cin, choice);
	LogToFile(choice, logFile);

	if (choice == "1") {
		string searchName;
		cout << "Введите название трубы: ";
		getline(cin, searchName);
		LogToFile(searchName, logFile);
		for (int i = 0; i < pipeCount; ++i) {
			if (pipes[i].GetName().find(searchName) != string::npos) {
				cout << "Найдена труба:\n";
				cout << "Индекс: " << i <<"\n";
				cout << pipes[i].ToString();
				cout << "\n";
				p++;
			}
		}
		if (p == 0) {
			cout << "Не найдено труб по запросу\n";
			return 0;
		}
		else if (!(p == 0)) {
			return 1;
		}

	}
	else if (choice == "2") {
		string status;
		cout << "Введите статус ремонта (y/n): ";
		getline(cin, status);
		LogToFile(status, logFile);
		while (true) {
			if (status == "y" || status == "Y" || status == "n" || status == "N") {
				break;
			}
			cout << "Введите верный статус (y/n): ";
			LogToFile("Error.Try y or n >:(", logFile);
			getline(cin, status);
			LogToFile(status, logFile);
		}
		bool inRepair = (status == "y" || status == "Y");
		for (int i = 0; i < pipeCount; ++i) {
			if (pipes[i].GetInRepair() == inRepair) {
				cout << "Найдена труба:\n";
				cout << "Индекс: " << i << "\n";
				cout << pipes[i].ToString();
				cout << "\n";
				p++;
			}
		}
		if (p == 0) {
			cout << "Не найдено труб по запросу\n";
			return 0;
		}
		else if (!(p == 0)) {
			return 1;
		}
	}
	else {
		cout << "Неверный выбор.\n";
		LogToFile("Error. Wrong choice >:(", logFile);
	}
}

bool SearchKS(map<int, ks>&kss, int ksCount, ofstream& logFile) {
	cout << "Выберите фильтр для поиска КС:\n";
	cout << "1. По названию\n";
	cout << "2. По проценту незадействованных цехов\n";
	int k = 0;

	string choice;
	getline(cin, choice);
	LogToFile(choice, logFile);

	if (choice == "1") {
		string searchName;
		cout << "Введите название КС для поиска: ";
		getline(cin, searchName);
		LogToFile(searchName, logFile);
		for (int i = 0; i < ksCount; ++i) {
			if (kss[i].GetName().find(searchName) != string::npos) {
				cout << "Найдена КС:\n";
				cout << "Индекс: " << i << "\n";
				cout << kss[i].ToString(); 
				cout << "\n";
				k++;
			}
		}
		if (k == 0) {
			cout << "Не найдено KC по запросу\n";
			return 0;
		}
		else if (!(k == 0)) {
			return 1;
		}
	}
	else if (choice == "2") {
		int percent;
		cout << "Введите процент незадействованных цехов: ";
		cin >> percent;
		cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
		LogToFile(to_string(percent), logFile);
		for (int i = 0; i < ksCount; ++i) {
			int unusedPercent = 100 * (kss[i].GetNumFac() - kss[i].GetNumWorkFac()) / kss[i].GetNumFac();
			if (unusedPercent == percent) {
				cout << "Найдена КС:\n";
				cout << "Индекс: " << i << "\n";
				cout << kss[i].ToString();
				cout << "\n";
				k++;
			}
		}
		if (k == 0) {
			cout << "Не найдено KC по запросу\n";
			return 0;
		}
		else if (!(k == 0)) {
			return 1;
		}
	}
	else {
		cout << "Неверный выбор.\n";
		LogToFile("Error. Wrong choice >:(", logFile);
	}
}

void ConnectKS(Graph& graph, map<int, pipe>& pipes, map<int, ks>& kss, int& pipeIDCounter, ofstream& logFile) {
	int inputKS, outputKS, diameter;
	cout << "Введите ID входной КС: ";
	inputKS = GetPositiveNumberID("", logFile);
	cout << "Введите ID выходной КС: ";
	outputKS = GetPositiveNumberID("", logFile);
	cout << "Введите требуемый диаметр трубы (500, 700, 1000, 1400): ";
	string input;
	while (true) {
		getline(cin, input);
		LogToFile(input, logFile);
		if (input.empty()) {
			cout << "Ввод не должен быть пустым. Попробуйте снова: ";
			LogToFile("Error. Empty insert >:(", logFile);
			continue;
		}
		if (IsInteger(input)) {
			diameter = stoi(input);
			if (diameter == 500 || diameter == 700 || diameter == 1000 || diameter == 1400) {
				break;
			}
		}
		std::cout << "Введите одно из предложенных значений\n";
	}

	// Проверка, разные ли это КС
	if (inputKS == outputKS) {
		cout << "Ошибка: введите разные КС.\n";
		return;
	}

	// Проверка существования КС
	if (kss.find(inputKS) == kss.end() || kss.find(outputKS) == kss.end()) {
		cout << "Ошибка: одна из указанных КС не существует.\n";
		return;
	}

	// Проверка, не соединены ли эти КС уже
	for (const auto& conn : graph.GetConnections(inputKS)) {
		if (conn.outputKS == outputKS) {
			cout << "Ошибка: Эти КС уже соединены через трубу ID " << conn.pipeID << ".\n";
			return;
		}
	}

	// Поиск подходящей трубы, которая еще не используется
	int selectedPipeID = -1;
	for (const auto& [id, p] : pipes) {
		if (p.GetDiameter() == diameter && !p.GetInRepair()) {
			// Проверяем, не используется ли труба
			if (!graph.IsPipeUsed(id)) {
				selectedPipeID = id;
				break;
			}
		}
	}

	// Если подходящей трубы нет, создаем новую
	if (selectedPipeID == -1) {
		cout << "Подходящая труба не найдена, создаем новую.\n";
		pipes[pipeIDCounter] = pipe("GeneratedPipe", 100, diameter, false); // Пример значений
		selectedPipeID = pipeIDCounter++;
	}

	// Добавляем соединение в граф
	graph.AddConnection(inputKS, outputKS, selectedPipeID);
	cout << "КС соединены через трубу ID: " << selectedPipeID << "\n";
}


void PerformTopologicalSort(Graph& graph, map<int, ks>& kss) {
	try {
		if (kss.empty()) {
			cout << "Ошибка: граф не содержит вершин (КС). Выполнение сортировки невозможно.\n";
			return;
		}

		vector<int> sortedKS = graph.TopologicalSort();
		if (sortedKS.empty()) {
			cout << "Граф не содержит соединений или является ацикличным.\n";
			return;
		}

		cout << "Топологический порядок КС:\n";
		for (int ksID : sortedKS) {
			cout << ksID << " ";
		}
		cout << endl;
	}
	catch (const std::exception& e) {
		cout << "Ошибка: " << e.what() << endl;
	}
}

void PrintGraphConnections(Graph& graph) {
	cout << "Соединения в графе:\n";
	graph.PrintConnections();
}




int main() {
	ofstream logFile("C:/Users/05bla/Downloads/LAB2/LAB2/LAB2/log.txt", ios::app); // Открываем файл для добавления
	if (!logFile) {
		cerr << "Ошибка при открытии файла для логирования.\n";
		return 1; // Завершаем программу в случае ошибки
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
		cout << "1. Создать трубу\n";
		cout << "2. Создать КС\n";
		cout << "3. Редактирование труб\n";
		cout << "4. Редактирование КС\n";
		cout << "5. Показать все объекты\n";
		cout << "6. Сохранить\n";
		cout << "7. Загрузить\n";
		cout << "8. Поиск\n";
		cout << "9. Соединить КС\n";
		cout << "10. Топологическая сортировка\n";
		cout << "11. Выход\n";
		getline(cin, choose);
		LogToFile(choose, logFile);
		while (!IsInteger(choose)) {
			cout << "Введите цифру от 1 до 9" << endl;
			LogToFile("Error. Try 1 to 9 >:(", logFile);
			getline(cin, choose);
			LogToFile(choose, logFile);
		}

		switch (stoi(choose)) {
		case 1: {
			if (pipeIDCounter >= MAX_PIPES) {
				cout << "Достигнут максимум труб" << endl;
				break;
			}
			string name;
			cout << "Введите название трубы: ";
			while (true) {
				getline(cin, name);
				LogToFile(name, logFile);
				if (name.empty()) {
					cout << "Имя не может быть пустой строкой. Попробуйте снова: ";
					LogToFile("Error. Name can't be empty >:(", logFile);
				}
				else {
					break;
				}
			}

			int length = GetPositiveNumber("Введите длину трубы: ", logFile);
			int diameter = GetPositiveNumber("Введите диаметр трубы: ", logFile);
			bool inRepair = GetRepairStatus("Находится ли труба в ремонте? (y/n): ", logFile);

			pipes[pipeIDCounter] = pipe(name, length, diameter, inRepair);
			pipeCount++;
			pipeIDCounter++;
			cout << "Труба создана\n";
		}
			  break;
		case 2: {
			if (ksIDCounter >= MAX_KS) {
				cout << "Достигнуто максималльное количество КС" << endl;
				break;
			}
			string name;
			cout << "Введите название KС: ";
			while (true) {
				getline(cin, name);
				LogToFile(name, logFile);
				if (name.empty()) {
					cout << "Имя не может быть пустой строкой. Попробуйте снова: ";
					LogToFile("Error.Name can't be empty >:(", logFile);
				}
				else {
					break;
				}
			}
			int NumFac = GetPositiveNumber("Введите количество фабрик: ", logFile);
			int NumWorkFac;
			while (true) {
				NumWorkFac = GetPositiveNumber("Введите количество работающих фабрик: ", logFile);
				if (NumWorkFac <= NumFac) {
					break;
				}
				cout << "Количество работаюих фабрик должно быть меньше общего\n";
				LogToFile("Error. Ammount of working factories need to be below total >:(", logFile);
			}

			int eff;
			while (true) {
				eff = GetPositiveNumber("Введите эффективность КС (0-100): ", logFile);
				if (eff >= 0 && eff <= 100) {
					break;
				}
				cout << "Введите число от 0 до 100\n";
				LogToFile("Error. Try 0 to 100 >:(", logFile);
			}

			kss[ksIDCounter] = ks(name, NumFac, NumWorkFac, eff);
			ksCount++;
			ksIDCounter++;
			cout << "КС создана\n";
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
				cout << "Нет созданных труб\n";
			}
			else {
				cout << "Список труб:\n";
				showPipes(pipes);
			}
			if (kss.size() == 0) {
				cout << "Нет созданных КС\n";
			}
			else {
				cout << "Список KC:\n";
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
			cout << "Выберите объект для поиска:\n";
			cout << "1. Труба\n";
			cout << "2. КС\n";
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
				cout << "Неверный выбор.\n";
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
			cout << "Введите цифру от 1 до 11" << endl;
			break;
		}

		}
	}
}