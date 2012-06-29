#include <string>
#include <vector>
#include <iostream>
#include "../dbmconv/many.h"
#include <atlstr.h>
#include <boost/shared_ptr.hpp>

using namespace std;
struct opr{
	void operator() (many& m){
		wcout << m.to_string() << std::endl;
	}
};
int main()
{	
	std::vector<many> vctm;	
	vctm.push_back(many (5));
	vctm.push_back(many(-5));
	float f = 2.1;
	vctm.push_back(many(f));
	vctm.push_back(many(-5.4));
	//vctm.push_back(many(CString(L"kakulenok")));

	setlocale(LC_ALL, ".1251");
	std::for_each(vctm.begin(), vctm.end(), opr());

	wstring str;
	wcin >> str;
	vctm[0].from_string(str.c_str());
	int j = 4;
	int p = boost::lexical_cast<int>(boost::any_cast<int>(vctm[2]));
	return 0;
}