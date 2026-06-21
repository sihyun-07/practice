#include<iostream>
#include<cstdlib>
#include<ctime>
#include"Character.h"

using std::cout;
using std::cin;
using std::string;

// === 전역 함수 프로토타입 (원형) 선언 ===
int showMainMenu(int stage);
int showBattleMenu(Character& p, Character* e);
void processItemUse(Character& p, Character* e);

// === 메인 실행부 ===
int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    cout << "========================================\n";
    cout << "               TEXT RPG          \n";
    cout << "========================================\n";

    cout << "용사의 직업을 선택하세요:\n";
    cout << "[1] 전사 (HP: 130, ATK: 12, 스킬: 방패 쳐내기)\n";
    cout << "[2] 마법사 (HP: 80, ATK: 22, 스킬: 메테오 스트라이크)\n";
    cout << "[3] 궁수 (HP: 100, ATK: 16, 스킬: 더블 어택)\n";
    cout << "선택 : ";
    int jobChoice;
    cin >> jobChoice;

    if (jobChoice < 1 || jobChoice > 3) jobChoice = 1;

    int playerHp = (jobChoice == 1) ? 130 : (jobChoice == 2) ? 80 : 100;
    int playerAtk = (jobChoice == 1) ? 12 : (jobChoice == 2) ? 22 : 16;
    Character player("용사", playerHp, playerAtk, jobChoice);

    // 몬스터 객체 배열 생성
    Character monsters[3] = {
        Character("하급 슬라임", 35, 6, 0),
        Character("고블린 전사", 60, 11, 0),
        Character("마왕 (BOSS)", 130, 20, 0)
    };

    int currentStage = 0;

    // 메인 게임 루프
    while (player.getHp() > 0 && currentStage < 3) {
        int mainMenuChoice = showMainMenu(currentStage + 1);

        if (mainMenuChoice == 3) {
            cout << "게임을 안전하게 종료합니다.\n";
            return 0;
        }

        if (mainMenuChoice == 2) {
            cout << "\n--- 내 인벤토리 상태 (배열 출력) ---\n";
            for (int i = 0; i < 5; i++) {
                cout << "[" << i + 1 << "] " << player.inventory[i].name
                    << (player.inventory[i].isUsed ? " (사용 불가능/없음)" : " - " + player.inventory[i].effectDesc) << "\n";
            }
            continue;
        }

        if (mainMenuChoice == 1) {
            // 현재 스테이지 몬스터 타겟팅
            Character* enemy = &monsters[currentStage];
            cout << "\n[STAGE " << currentStage + 1 << "] " << enemy->getName() << "(이)가 출현했습니다!\n";

            // 전투 루프
            while (player.getHp() > 0 && enemy->getHp() > 0) {
                int battleChoice = showBattleMenu(player, enemy);

                if (battleChoice == 1) {
                    player.attack(enemy);
                }
                else if (battleChoice == 2) {
                    // 플레이어의 공격 횟수가 2번 쌓였는지 검사
                    if (player.getAttackCount() >= 2) {
                        player.useSkill(enemy);      // 스킬 발동
                        player.resetAttackCount();   // 발동 후 0으로 리셋
                        if (player.getHp() <= 0) {
                            cout << "\n과도한 스킬 반동으로 사망하였습니다... GAME OVER\n";
                            return 0;
                        }
                    }
                    else {
                        // 조건 미달 시 경고 메시지를 띄우고 턴을 소모하지 않게 처리
                        cout << "고유 스킬을 사용할 수 없습니다! (일반 공격을 "
                            << (2 - player.getAttackCount()) << "번 더 해야 합니다.)\n";
                        continue; // 전투 메뉴 선택으로 다시 돌아감
                    }
                }
                else if (battleChoice == 3) {
                    processItemUse(player, enemy);
                }

                // 몬스터 반격
                if (enemy->getHp() > 0) {
                    enemy->attack(&player);
                }
            }

            // 전투 결과 판정
            if (player.getHp() <= 0) {
                cout << "\n용사가 전투 중 전사했습니다... GAME OVER\n";
                return 0;
            }
            else if (enemy->getHp() <= 0) {
                cout << "\n" << enemy->getName() << "을(를) 토벌했습니다!\n";

                if (currentStage == 0) {
                    player.obtainItem(Item("말랑한 젤리", "HP를 20 회복합니다.", 20));
                }
                else if (currentStage == 1) {
                    player.obtainItem(Item("녹슨 철검", "공격력이 영구적으로 5 증가합니다.", 5));
                }

                currentStage++;
            }
        }
    }

    cout << "\n축하합니다! 모든 스테이지를 클리어하고 대륙의 평화를 되찾았습니다!\n";
    return 0;
}

// === 전역 함수 정의부 ===

int showMainMenu(int stage) {
    cout << "\n--- 메인 메뉴 (현재 스테이지: " << stage << "/3) ---\n";
    cout << "[1] 사냥터 입장  [2] 인벤토리 확인  [3] 게임 종료\n";
    cout << "선택 : ";
    int choice;
    cin >> choice;
    return choice;
}

int showBattleMenu(Character& p, Character* e) {
    cout << "\n[" << p.getName() << " HP: " << p.getHp() << "]"
        << " vs [" << e->getName() << " HP: " << e->getHp() << "]\n";
    cout << "--- 전투 행동 메뉴 ---\n";
    cout << "[1] 일반 공격  [2] 고유 스킬 사용  [3] 아이템 사용\n";
    cout << "선택 : ";
    int choice;
    cin >> choice;
    return choice;
}

void processItemUse(Character& p, Character* e) {
    cout << "\n어떤 아이템을 사용하시겠습니까?\n";
    for (int i = 0; i < 5; i++) {
        cout << "[" << i + 1 << "] " << p.inventory[i].name
            << (p.inventory[i].isUsed ? " (X)" : "") << "\n";
    }
    cout << "선택 : ";
    int itemChoice;
    cin >> itemChoice;
    itemChoice--;

    if (itemChoice >= 0 && itemChoice < 5 && !p.inventory[itemChoice].isUsed) {
        p.inventory[itemChoice].isUsed = true; // 아이템 소모 처리

        // 1. 빨간 포션 효과
        if (p.inventory[itemChoice].name == "빨간 포션") {
            p.heal(p.inventory[itemChoice].value);
            cout << "포션을 사용하여 HP를 " << p.inventory[itemChoice].value << " 회복했습니다!\n";
        }
        // 2. 말랑한 젤리 효과
        else if (p.inventory[itemChoice].name == "말랑한 젤리") {
            p.heal(p.inventory[itemChoice].value);
            cout << "[전리품] 말랑한 젤리를 먹어 HP를 " << p.inventory[itemChoice].value << " 회복했습니다!\n";
        }
        // 3. 힘의 비약 효과
        else if (p.inventory[itemChoice].name == "힘의 비약") {
            p.buffAtk(p.inventory[itemChoice].value);
            cout << "비약을 마셔 공격력이 영구적으로 " << p.inventory[itemChoice].value << " 증가했습니다!\n";
        }
        // 4. 녹슨 철검 효과
        else if (p.inventory[itemChoice].name == "녹슨 철검") {
            p.buffAtk(p.inventory[itemChoice].value);
            cout << "[전리품] 녹슨 철검을 갈아 공격력이 영구적으로 " << p.inventory[itemChoice].value << " 증가했습니다!\n";
        }
        // 5. 얼음 폭탄 효과
        else if (p.inventory[itemChoice].name == "얼음 폭탄") {
            e->debuffAtk(p.inventory[itemChoice].value);
            cout << "얼음 폭탄을 던져 " << e->getName() << "의 공격력을 " << p.inventory[itemChoice].value << " 낮췄습니다!\n";
        }
    }
    else {
        cout << "사용할 수 없는 아이템입니다! 잘못된 선택으로 턴을 허비했습니다.\n";
    }
}