//Builder.h
#ifndef _BUILDER_H_
#define _BUILDER_H_

#include <string>
#include "product.h"
using namespace std;


class Builder_Product;


class Builder {
public:
	virtual ~Builder();
	virtual void BuildPartA(const string& buildPara) = 0;
	virtual void BuildPartB(const string& buildPara ) = 0;
	virtual void BuildPartC(const string& buildPara) = 0;
	virtual Builder_Product* GetProduct() = 0;
protected:
	Builder();
private:
};


class ConcreteBuilder: public Builder {
public:
	ConcreteBuilder();
	~ConcreteBuilder();
	void BuildPartA(const string& buildPara);
	void BuildPartB(const string& buildPara);
	void BuildPartC(const string& buildPara);
	Builder_Product* GetProduct();
protected:
private:
};
#endif //~_BUILDER_H_
