#pragma once
#include<string>
#include"Item.h"

class Character {
private:
    std::string name;       // 캐릭터 이름
    int hp;                 // 현재 체력
    int maxHp;              // 최대 체력 (포션 회복 한계선)
    int atk;                // 공격력
    int jobType;            // 직업 종류 (1: 전사, 2: 마법사, 3: 궁수)
    int attackCount;        // 일반 공격 횟수

public:
    // 5칸짜리 인벤토리
    Item inventory[5];

    Character(std::string n, int h, int a, int job);

    std::string getName();
    int getHp();
    int getAtk();
    int getJobType();

    void takeDamage(int damage);
    void heal(int amount);
    void buffAtk(int amount);
    void debuffAtk(int amount);

    void attack(Character* target);    // 일반 공격
    void useSkill(Character* target);  // 고유 스킬 공격

    int getAttackCount();       // 현재 쌓인 공격 횟수 반환
    void resetAttackCount();    // 스킬 사용 후 공격 횟수 초기화

    void obtainItem(Item newItem);
};