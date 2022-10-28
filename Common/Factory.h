#ifndef PRODUCT_FACTORY_H
#define PRODUCT_FACTORY_H
#include <map>

#define CLASS_COUNT 2000

/**********************
meaning of the template parameters:
Base:
class name of base class

RealClass:
class name of real class, RealClass is derived from Base.

classkey:
could be enum value, string, etc,
every RealClass has its corresponding classkey.

***********************/

//struct ptrCmp
//{
//	bool operator()(classkeyType key1,classkeyType key2) const
//	{
//		return strcmp(s1, s2) == 0;
//	}
//};

//产生长度为length的随机字符串  
//length 必须小于63， 因sOut的长度是64
static void GenerateRandomString(const int length, char* sOut)  
{  
	for (int i = 0; i < length - 1; i++)  
	{  
		int nFlag = rand() % 3;  
		switch (nFlag)  
		{  
		case 0:  
			sOut[i] = 'A' + rand() % 26;  
			break;  
		case 1:  
			sOut[i] = 'a' + rand() % 26;  
			break;  
		case 2:  
			sOut[i] = '0' + rand() % 10;  
			break;  
		default:  
			sOut[i] = 'x';  
			break;  
		}  
	}  
	sOut[length - 1] = '\0';  
}  

template<class Base>
class AbstractFactoryUnit
{
public:
	virtual Base* BuildProduct() {return NULL;}
};

template <class Base, class classkeyType>
class Factory
{
protected:
	Factory(void){
		memset(m_szID,0,64);
		GenerateRandomString(8,m_szID);
	};

public:
	static Factory& Instance();

public:
	Base* BuildProduct(classkeyType key){		
		std::map<classkeyType,AbstractFactoryUnit<Base>*> ::iterator it = m_Factors.find(key);
		if (m_Factors.end() == it || NULL == it->second){
			return NULL;	
		}

		AbstractFactoryUnit<Base>* pFactoryUnit = it->second;
		return pFactoryUnit->BuildProduct();	
	}

	void RegisterFactory(classkeyType key, AbstractFactoryUnit<Base>* pFactory){
		m_Factors[key]= pFactory;
	};

protected:
	std::map<classkeyType,AbstractFactoryUnit<Base>*>  m_Factors;
	char m_szID[64];
};

template<class Base,class classkeyType>
Factory<Base,classkeyType>& Factory<Base,classkeyType>::Instance()
{
	static Factory<Base,classkeyType> factory;
	return factory;
}

template <class Base, class RealClass,class classkeyType>
class FactoryUnit: public AbstractFactoryUnit<Base>
{
public:
	FactoryUnit(classkeyType key)
	{
		Factory<Base,classkeyType>::Instance().RegisterFactory(key,this);
	}

	Base* BuildProduct()
	{
		Base* pObj = new RealClass;
		return pObj;
	}
};

#define IMPLEMENT_FACTORY(Base,RealClass,classkeyType,type) \
	static FactoryUnit<Base,RealClass,classkeyType> RealClass##factory(type);\

#endif