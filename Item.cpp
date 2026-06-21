#include"Item.h"

Item::Item() {
    name = "비어있음";
    effectDesc = "";
    value = 0;
    isUsed = true; // 비어있는 칸이므로 사용된 상태(선택 불가)로 세팅
}

Item::Item(std::string n, std::string desc, int val) {
    name = n;
    effectDesc = desc;
    value = val;
    isUsed = false; // 생성 직후 미사용 상태로 설정
}