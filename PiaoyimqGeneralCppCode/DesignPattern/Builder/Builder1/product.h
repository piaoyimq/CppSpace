//Pr oduct.h
#ifndef _PRODUCT_H_
#define _PRODUCT_H_


class Builder_Product {
public:
	Builder_Product();
	~Builder_Product();
	void ProducePart();
protected:
private:
};


class ProductPart {
public:
	ProductPart();
	~ProductPart();
	ProductPart* BuildPart();
protected:
private:
};

#endif //~_PRODUCT _H_
