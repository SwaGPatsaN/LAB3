#include "pipe.h"
#include <string>
#include <sstream>
using namespace std;

ostream& operator<<(ostream& os, const pipe& p) {
	os << "Имя трубы: " << p.GetName() << endl;
	os << "Длина трубы: " << p.GetLength() << endl;
	os << "Диаметр трубы: " << p.GetDiameter() << endl;
	os << "Статус ремонта трубы: " << (p.GetInRepair() ? "Да" : "Нет") << endl;
	return os;
}

pipe::pipe() : name(""), length(0), diametr(0), InRepair(false) {}

pipe::pipe(string n, int l, int d, bool rep)
{
	name = n;
	length = l;
	diametr = d;
	InRepair = rep;
}

string pipe::GetName() const
{
	return name;
}

int pipe::GetLength() const
{
	return length;
}

int pipe::GetDiameter() const
{
	return diametr;
}

bool pipe::GetInRepair() const
{
	return InRepair;
}

void pipe::SetInRepair(bool s)
{
	InRepair = s;
}

string pipe::ToString() const {
	ostringstream oss;
	oss << "Название: " << name << "\n"
		<< "Длина: " << length << "\n"
		<< "Диаметр: " << diametr << "\n"
		<< "В починке: " << InRepair << "\n";
	return oss.str();
}

