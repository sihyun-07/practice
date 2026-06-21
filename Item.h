#pragma once
#include<string>

class Item {
public:
    std::string name;       // 아이템 이름
    std::string effectDesc; // 아이템 효과 설명 텍스트
    int value;              // 효과 수치 (회복량 또는 버프/디버프 수치)
    bool isUsed;            // 아이템 사용 여부 (true면 인벤토리에서 사용 불가 처리)

    Item();
    Item(std::string n, std::string desc, int val);
};