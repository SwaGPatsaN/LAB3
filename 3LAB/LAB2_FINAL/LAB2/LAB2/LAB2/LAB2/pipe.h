#pragma once
#include <string>
using namespace std;
class pipe
{
private:
	string name;
	int length;
	int diametr;
	bool InRepair;
public:
	friend ostream& operator<<(ostream& os, const pipe& p);
	pipe(string n, int l, int d, bool rep);
	pipe();
	string GetName() const;
	int GetLength() const;
	int GetDiameter() const;
	bool GetInRepair() const;
	void SetInRepair(bool s);
	string ToString() const;
};
