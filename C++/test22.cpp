#include <iostream>
#include <memory>
#include <map>
#include <string>

#define DEFER_IMPL(expr, ID) \
auto deferFunc##ID = [&]() { expr; }; \
struct Defer##ID { \
	decltype(deferFunc##ID) _f; \
	Defer##ID(decltype(deferFunc##ID) f) : _f(f) {} \
	~Defer##ID() { _f(); } \
} defer##ID(deferFunc##ID);

#define __DEFER__(expr, ID) DEFER_IMPL(expr, ID)

#define DEFER(expr) __DEFER__(expr, __COUNTER__)

struct New 
{
	virtual void* operator()() = 0;	
};

std::map<std::string, New*> mapcls;

void* newobj(std::string cls) 
{
	return (void*)((*mapcls[cls])());
}

#define PUTCLS_IMPL(CLS, PRE, ID) \
struct PRE##ID : public New  \
{ \
	PRE##ID() \
	{ \
		extern std::map<std::string, New*> mapcls; \
		mapcls[#CLS] = this; \
	} \
	void* operator()() override  \
	{ \
		return new CLS(); \
	} \
} obj##PRE##ID

#define __PUTCLS__(CLS, PRE, ID)  PUTCLS_IMPL(CLS, PRE, ID)

#define PUTCLS(CLS) __PUTCLS__(CLS, ADSFASDF, __COUNTER__)

#define NEWOBJ(CLSNAME) newobj(CLSNAME) 

class Rate
{
public:
	virtual ~Rate(); 
	virtual bool operator()(int score) = 0;
};

#define NEWRATEOBJ(CLSNAME) (Rate*)NEWOBJ(CLSNAME)

class BadRate : public Rate 
{
public:
	~BadRate() {std::cout << "~BadRate" << std::endl;}
	bool operator()(int score) override
	{
		std::cout << "Bad Score " << score << std::endl;
		return true;	
		return true;
	}
};

PUTCLS(BadRate);

class OKRate : public Rate 
{
public:
	~OKRate() {std::cout << "~OKRate" << std::endl;}
	bool operator()(int score) override
	{
		if (score >= 60)
		{
			std::cout << "OK Score " << score << std::endl;
			return true;	
		}
		Rate* o = NEWRATEOBJ("BadRate");
		DEFER(delete o);
		return (*o)(score);
	}	
};

PUTCLS(OKRate);

class GoodRate : public Rate 
{
public:
	~GoodRate() { std::cout << "~GoodRate" << std::endl; }
	bool operator()(int score) override
	{
		if (score >= 80)
		{
			std::cout << "Good Score " << score << std::endl;
			return true;	
		}
		Rate* o = NEWRATEOBJ("OKRate");
		DEFER(delete o);
		return (*o)(score);
	}
};

PUTCLS(GoodRate);


bool rate(int score)
{
	Rate* o = NEWRATEOBJ("GoodRate");
	DEFER(delete o);

	return (*o)(score);
}

int main() {
	int score = 90;

	rate(90);
	rate(80);
	rate(70);
	rate(40);

	return 0;
}


//class Rational {
//public:
//	Rational() {
//		std::cout << "Rational Constructor" << std::endl;
//	}
//	Rational(Rational&& oth) : 
//		_modlecule(oth._modlecule), _demonimator(oth._demonimator)
//	{}
//	void SetMolecule(int mod) { _modlecule = mod; }
//	void SetDemonimator(int demo) { _demonimator = demo; }
//	int  GetMolecule() { return _modlecule; }
//	int  GetDemonimator() { return _demonimator; }
//	
//private:
//	int _modlecule;
//	int _demonimator;
//};
//
//SP<Rational> RatAdd(Rational& rat1, Rational& rat2) {
//	SP<Rational> rat3(new Rational());
//
//	return rat3;
//}
//
//Rational& RatMul(Rational& rat1, Rational& rat2);


//int main() {
//	Rational rat1 = Rational();
//	Rational rat2 = Rational();
//
//	sp<Rational> rat3 = RatAdd(rat1, rat2);
//	
//	std::cout << "Hello world" << std::endl;
//	return 0;
//}

