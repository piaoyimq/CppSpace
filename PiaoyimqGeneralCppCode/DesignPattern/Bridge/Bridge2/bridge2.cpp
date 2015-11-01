/*
 * bridge2.cpp
 *
 *  Created on: 22-Apr-2015
 *      Author: ezhweib
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;


// 手机软件
class HandsetSoft {
public:
	virtual void Run()=0;
};


// 游戏软件
class HandsetGame: public HandsetSoft {
public:
	virtual void Run() {
		cout << "Run cellphone game." << endl;
	}
};


// 通讯录软件
class HandSetAddressList: public HandsetSoft {
public:
	virtual void Run() {
		cout << "cellphone contact" << endl;
	}
};


// 手机品牌
class HandsetBrand {
protected:
	HandsetSoft* m_soft;
public:
	void SetHandsetSoft(HandsetSoft* temp) {
		m_soft = temp;
	}
	virtual void Run()=0;
};


//M 品牌
class HandsetBrandM: public HandsetBrand {
public:
	virtual void Run() {
		m_soft->Run();
	}
};


//N 品牌
class HandsetBrandN: public HandsetBrand {
public:
	virtual void Run() {
		m_soft->Run();
	}
};


// 客户端
int main_bridge2() {
	HandsetBrand *brand;
	brand = new HandsetBrandM();
	brand->SetHandsetSoft(new HandsetGame());
	brand->Run();
	brand->SetHandsetSoft(new HandSetAddressList());
	brand->Run();

	return 0;
}

