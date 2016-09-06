/*
 * builder3.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: piaoyimq
 */


#include <string>
#include <iostream>
#include <vector>
using namespace std;

class Meal
{
public:
std::string& getDrink()
{
	return drink;
}

std::string& getFood()
{
	return food;
}

void setDrink(std::string& dr)
{
	drink = dr;
}

void setFood(std::string& fo)
{
	drink = fo;
}

private:
	std::string drink;
	std::string food;
};


class MealBuilder
{
public:
	MealBuilder() {}
	~MealBuilder() {}

	void buildDrink()
	{
		meal.setDrink("Cocola");
	}

	void buildFood()
	{
		meal.setFood("Rice");
	}

	void construct()
	{
		buildDrink();
		buildFood();
	}

	Meal& getMeal()
	{
		return meal;
	}

private:
	Meal meal;
}



/*
 * if only have one concerate builder, we can omit the abstract builder, and omit the director,
 * Use the concerate builder as the abstract builder and director.
 *
 * */

int main()
{
	MealBuilder mb;
	mb.construct();
	mb.getMeal();
}
