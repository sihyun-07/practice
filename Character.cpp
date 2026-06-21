#include<iostream>
#include<cstdlib>
#include"Character.h"

using std::cout;
using std::cin;
using std::string;

// 직업에 따른 고유 능력치 세팅 및 인벤토리 기본 아이템 배열 초기화
Character::Character(string n, int h, int a, int job) {
    name = n;
    hp = h;
    maxHp = h;
    atk = a;
    jobType = job;
    attackCount = 0;

    // 플레이어 캐릭터인 경우에만 인벤토리 배열에 기본 소모품 아이템을 지급
    if (jobType != 0) {
        inventory[0] = Item("빨간 포션", "HP를 40 회복합니다.", 40);
        inventory[1] = Item("힘의 비약", "공격력이 영구적으로 8 증가합니다.", 8);
        inventory[2] = Item("얼음 폭탄", "상대의 공격력을 5 낮춥니다.", 5);
    }
}

string Character::getName() { return name; }
int Character::getHp() { return hp; }
int Character::getAtk() { return atk; }
int Character::getJobType() { return jobType; }
int Character::getAttackCount() { return attackCount; }
void Character::resetAttackCount() { attackCount = 0; }

// 데미지 처리 함수
void Character::takeDamage(int damage) {
    hp -= damage;
    if (hp < 0) hp = 0;
}

// 체력 회복 함수
void Character::heal(int amount) {
    hp += amount;
    if (hp > maxHp) hp = maxHp;
}

// 공격력 버프 함수
void Character::buffAtk(int amount) {
    atk += amount;
}

// 상대방 공격력 디버프 함수
void Character::debuffAtk(int amount) {
    atk -= amount;
    if (atk < 1) atk = 1;
}


// 몬스터 체력 차감
void Character::attack(Character* target) {
    cout << name << "의 일반 공격! " << target->getName() << "에게 " << atk << "의 피해를 줍니다.\n";
    target->takeDamage(atk);

    // 플레이어 캐릭터가 공격할 때만 횟수 증가
    if (jobType != 0) {
        attackCount++;
        cout << "스킬 게이지 충전 중... (" << attackCount << "/2)\n";
    }
}

//직업별 고유 스킬 공격 구현
void Character::useSkill(Character* target) {
    int damage = 0;

    switch (jobType) {
    case 1: // 전사: 방패 쳐내기 (1.5배 데미지 + 흡혈)
        damage = (int)(atk * 1.5);
        cout << name << "의 고유 스킬 [방패 쳐내기] 발동!\n";
        cout << target->getName() << "에게 " << damage << "의 피해를 주고 자신의 HP를 10 회복합니다.\n";
        target->takeDamage(damage);
        this->heal(10); // 본인 체력 회복
        break;

    case 2: // 마법사: 메테오 스트라이크 (2.5배 강력한 데미지 + 본인 패널티 데미지)
        damage = (int)(atk * 2.5);
        cout <<  name << "의 고유 스킬 [메테오 스트라이크] 발동!\n";
        cout << target->getName() << "에게 " << damage << "의 핵폭탄급 피해를 주지만 반동으로 자신도 15의 피해를 입습니다.\n";
        target->takeDamage(damage);
        this->takeDamage(15); // 마법사 본인도 피해를 입음
        break;

    case 3: // 궁수: 더블 어택 (50% 확률로 연속 공격)
        cout << name << "의 고유 스킬 [더블 어택] 시도\n";
        damage = atk;
        cout << "첫 번째 화살! " << target->getName() << "에게 " << damage << "의 피해를 줍니다.\n";
        target->takeDamage(damage);

        // 50% 확률 계산
        if (rand() % 2 == 0) {
            cout << "연속 공격 성공! 두 번째 화살 추가 적중! " << damage << "의 추가 피해!\n";
            target->takeDamage(damage);
        }
        else {
            cout << "스킬 발동에 실패하였습니다.\n";
        }
        break;

    default: // 몬스터 혹은 예외 상황
        cout << name << "은 스킬을 사용할 수 없는 몬스터입니다. 일반 공격으로 대체합니다.\n";
        attack(target);
        break;
    }
}

void Character::obtainItem(Item newItem) {
    for (int i = 0; i < 5; i++) {
        if (inventory[i].isUsed == true) {
            inventory[i] = newItem;
            std::cout << "전리품 획득! [" << newItem.name << "]이(가) 인벤토리 " << i + 1 << "번 칸에 저장되었습니다.\n";
            return;
        }
    }
    std::cout << "인벤토리가 가득 차서 전리품 [" << newItem.name << "]을(를) 획득하지 못했습니다.\n";
}