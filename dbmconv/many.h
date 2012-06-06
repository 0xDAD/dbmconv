#pragma  once
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <atlstr.h>
#include <boost/shared_ptr.hpp>


struct conv{
	virtual std::wstring operator() (const boost::any* lpany) const = 0;
	virtual ~conv(){}
};

template <class T>
struct pod_conv : public conv
{
	std::wstring operator() (const boost::any* lpany) const{
		return boost::lexical_cast<std::wstring>(boost::any_cast<T>(*lpany));
	}
};

struct atlstr_conv : public conv
{
	std::wstring operator() (const boost::any* lpany) const{
		return std::wstring(boost::any_cast<CString>(*lpany).GetBuffer());
	}
};
class many: public boost::any
{
	typedef boost::any _base;
	public:
		many(){}
		many( const many& rhs):boost::any((const boost::any&)rhs), pFnPtr(rhs.pFnPtr){}
		template<typename T> 
		many(const T& t, conv* ptr = new pod_conv<T>()) : boost::any(t){
			pFnPtr = boost::shared_ptr<conv>(ptr);
		}
		virtual ~many() { }
public:
		std::wstring to_string() const { 	
			if(pFnPtr == NULL)
				return L"";
			return (*pFnPtr)(this);
		}
		many & operator=(many rhs)
		{
			rhs.swap(*((_base*)this));
			pFnPtr = rhs.pFnPtr;
			return *this;
		}
private:
		boost::shared_ptr<conv> pFnPtr;
};

