#include <string>
#include "ks.h"
#include <sstream>
using namespace std;

ostream& operator<<(ostream& os, const ks& k) {
	os << "��� ��: " << k.GetName() << endl;
	os << "���������� ������: " << k.GetNumFac() << endl;
	os << "���������� ������� ������: " << k.GetNumWorkFac() << endl;
	os << "�������������: " << k.GetEff() << endl;
	return os;
}

ks::ks() : name(""), NumFac(0), NumWorkFac(0), eff(0) {}

ks::ks(string n, int nf, int nwf, int e)
{
	name = n;
	NumFac = nf;
	NumWorkFac = nwf;
	eff = e;
}

string ks::GetName() const
{
	return name;
}

int ks::GetNumFac() const
{
	return NumFac;
}

int ks::GetNumWorkFac() const
{
	return NumWorkFac;
}

int ks::GetEff() const
{
	return eff;
}

void ks::SetNumWorkFac(int A)
{
	NumWorkFac = A;
}

string ks::ToString() const {
	ostringstream oss;
	oss << "��������: " << name << "\n"
		<< "���������� �����: " << NumFac << "\n"
		<< "���������� �����: " << NumWorkFac << "\n"
		<< "�������������: " << eff << "%\n";
	return oss.str();
}