#include "pipe.h"
#include <string>
#include <sstream>
using namespace std;

ostream& operator<<(ostream& os, const pipe& p) {
	os << "��� �����: " << p.GetName() << endl;
	os << "����� �����: " << p.GetLength() << endl;
	os << "������� �����: " << p.GetDiameter() << endl;
	os << "������ ������� �����: " << (p.GetInRepair() ? "��" : "���") << endl;
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
	oss << "��������: " << name << "\n"
		<< "�����: " << length << "\n"
		<< "�������: " << diametr << "\n"
		<< "� �������: " << InRepair << "\n";
	return oss.str();
}

