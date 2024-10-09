//#pragma once
//#include "Object.h"
//
//enum EMonsterType { A, B, C, D };
//
//class Monster;
//
//extern std::vector<Monster*> vMonster;
//extern int monsterCount;
//
//class Monster : public Object
//{
//private:
//	EMonsterType monsterType;
//
//public:
//	Monster() : monsterType(A), Object()
//	{
//		Circle2D* c = new Circle2D(true, MONSTER);
//		SetObject(c);
//	}
//	Monster(POINT p) : monsterType(A), Object(p)
//	{
//		Circle2D* c = new Circle2D(true, MONSTER);
//		SetObject(c);
//	}
//	Monster(POINT p, EMonsterType a) : monsterType(a), Object(p)
//	{
//		Circle2D* c = new Circle2D(true, MONSTER);
//		SetObject(c);
//	}
//
//	EMonsterType	GetMonsterType() { return monsterType; }
//
//	void			SetMosterType(EMonsterType _monsterType) { this->monsterType = _monsterType; }
//
//	void			GenerateMonster();
//	void			DrawMonster(HDC&);
//};