#pragma once
#include <atlstr.h>
class Options{
protected:
	Options();
public:
	static CString GetInFileName() {
		return m_strInFile;
	}
	static  CString GetOutFileName() {
		return m_strOutFile;
	}
	static void SetInFileName(CString& strInFile){
		m_strInFile = strInFile;
	}
	static void SetOutFileName(CString& strOutFile){
		m_strOutFile = strOutFile;
	}
private:
	static CString m_strInFile;
	static CString m_strOutFile;
};


