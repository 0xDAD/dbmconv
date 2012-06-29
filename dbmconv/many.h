#pragma  once
#include <boost/any.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <atlstr.h>
#include <boost/shared_ptr.hpp>
#include <typeinfo>

struct conv{
	virtual std::wstring to_string (const boost::any* lpany) const = 0;
	virtual boost::any from_string(std::wstring& str) = 0;
	virtual ~conv(){}
};

template <class T>
struct pod_conv : public conv
{
	std::wstring to_string(const boost::any* lpany) const{
		return boost::lexical_cast<std::wstring>(boost::any_cast<T>(*lpany));
	}
	boost::any from_string(std::wstring& str){
		return boost::any(boost::lexical_cast<T>(str));	
	}
};

//struct atlstr_conv : public conv
//{
//	std::wstring operator() (const boost::any* lpany) const{
//		return std::wstring(boost::any_cast<CString>(*lpany).GetBuffer());
//	}
//};
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
			return pFnPtr->to_string(this);
		}
		bool from_string(const wchar_t* pStr){
			std::wstring str(pStr);
			if(pFnPtr == NULL)
				return false;
			try{
				boost::any::swap(pFnPtr->from_string(str));
			}
			catch(boost::bad_lexical_cast&){
				return false;
			}
			return true;
		}
		int to_int() const{
			int nRet = 0;
			if(typeid(nRet) == _base::type())
				return boost::any_cast<int>(*this);
			else
				return boost::lexical_cast<int>(to_string());
		}

		template <typename ValueType>
		ValueType value_cast(){
			try{
				return boost::any_cast<ValueType>(*this);
			}
			catch (boost::bad_any_cast&){
				return boost::lexical_cast<ValueType>(to_string());
			}
		}

		template <typename ValueType>
		bool cast(ValueType& rval){
			try{
				if(typeid(ValueType) == _base::type())
					rval = boost::any_cast<ValueType>(*this);
				else
					rval = boost::lexical_cast<ValueType>(to_string());
			}
			catch (boost::bad_any_cast&){
				return false;
			}
			catch (boost::bad_lexical_cast&){
				return false;
			}
			return true;
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

