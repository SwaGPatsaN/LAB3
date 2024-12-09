#pragma once
#include <string>
using namespace std;
class ks
{
private:
	string name;
	int NumFac;
	int NumWorkFac;
	int eff;
public:
	friend ostream& operator<<(ostream& os, const ks& k);
	ks(string n, int nf, int nwf, int e);
	ks();
	string GetName() const;
	int GetNumFac() const;
	int GetNumWorkFac() const;
	int GetEff() const;
	void SetNumWorkFac(int A);
	string ToString() const;
};