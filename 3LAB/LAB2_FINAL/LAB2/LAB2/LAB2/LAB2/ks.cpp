#include <string>
#include "ks.h"
#include <sstream>
using namespace std;

ostream& operator<<(ostream& os, const ks& k) {
	os << "Имя КС: " << k.GetName() << endl;
	os << "Количество фабрик: " << k.GetNumFac() << endl;
	os << "Количество рабочих фабрик: " << k.GetNumWorkFac() << endl;
	os << "Эффективность: " << k.GetEff() << endl;
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
	oss << "Название: " << name << "\n"
		<< "Количество цехов: " << NumFac << "\n"
		<< "Работающих цехов: " << NumWorkFac << "\n"
		<< "Эффективность: " << eff << "%\n";
	return oss.str();
}