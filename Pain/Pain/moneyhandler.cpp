#include <iostream>
#include <iomanip>
#include <string>
#include <curl/curl.h>          //통신 헤더
#include "nlohmann/json.hpp"            //nlohmann의 json 읽기 헤더(Github)
#include "japdongsani.h"            //기타 함수들을 포함하는 사용자 헤더
#include <cstdlib>

using json = nlohmann::json;

struct aPIBasket {           //API에서 받은 데이터를 저장(Github 참고함)
    std::string DefaultCurrency;
    std::vector<std::string> Targets;
    std::vector<double> USDexchange;
};

//API의 콜백(Github 참고함)
size_t burnCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    response->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//API로부터 환율 값 가져오기(Github 참고함)
bool fetchExchangeRates(const std::string& DefaultCurrency, aPIBasket& Basket) {
    std::string url = "https://v6.exchangerate-api.com/v6/ff01c2fa5e04256a3bc48bc4/latest/" + DefaultCurrency;
    std::string response;

    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, burnCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res == CURLE_OK) {
            json jsonData = json::parse(response);

            if (jsonData["result"] == "success") {
                Basket.DefaultCurrency = jsonData["base_code"].get<std::string>();

                json rates = jsonData["conversion_rates"];
                for (auto it = rates.begin(); it != rates.end(); ++it) {
                    Basket.Targets.push_back(it.key());
                    Basket.USDexchange.push_back(it.value().get<double>());
                }

                curl_easy_cleanup(curl);
                return true;
            }
        }

        curl_easy_cleanup(curl);
    }

    return false;
}

double XKRW = 1380;
double XEUR = 0.9;
double XGBP = 0.8;
double XJPY = 160;

const int COL1 = 35;            //터미널의 너비 1
const int COL2 = 80;            //터미널의 너비 2

void wonToAnything();
void wonAskSeg();
void dollarToAnything();
void dollarAskSeg();
void selectYourMoneySeg();
void welcomeSeg();
void otherBuisnessSeg();

int main() {
    std::string DefaultCurrency;
    DefaultCurrency = "USD";            //기준 환율을 달러로 설정

    aPIBasket Basket;

    //연결 실패
    if (!fetchExchangeRates(DefaultCurrency, Basket)) {
        std::cout << "주의: 환율을 가져오지 못했습니다. 서비스 장애이거나 컴퓨터가 네트워크에 연결하고 싶어할 수 있습니다." << std::endl;
        std::cout << "주의: 연결이 오프라인입니다. 프로그램은 고정된 근사값을 사용하여 계산합니다." << std::endl;
        border(COL2);
    }
    else if (fetchExchangeRates(DefaultCurrency, Basket)) {          //연결 성공
        //달러 대 원 환율을 저장
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "KRW") {
                XKRW = Basket.USDexchange[i];
                break;
            }
        }

        //달러 대 유로 환율을 저장
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "EUR") {
                XEUR = Basket.USDexchange[i];
                break;
            }
        }

        //달러 대 파운드 환율을 저장
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "GBP") {
                XGBP = Basket.USDexchange[i];
                break;
            }
        }

        //달러 대 엔 환율을 저장
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "JPY") {
                XJPY = Basket.USDexchange[i];
                break;
            }
        }
    }

    //시작 부분 불러오기
    welcomeSeg();

    return 0;
}

void yenToAnything(double money, int choice) {
    std::string targetname = "달러";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "달러";
        target = XJPY;
        break;
    case 2:
        targetname = "원";
        target = XKRW;
        break;
    case 3:
        targetname = "유로";
        target = XEUR;
        break;
    case 4:
        targetname = "파운드";
        target = XGBP;
        break;
    }

    //std::cout << int(money) << "엔은 " << money / XJPY * target << targetname << "입니다." << std::endl;
    printf("%d엔은 %f%s입니다.", int(money), money / XJPY * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void yenAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "변환할 값어치를 입력하여 주십시오.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환할 값어치를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;

    std::cout << std::setw(COL1) << std::left << "미국 달러($)" << std::right << "1(기본값)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "대한민국 원(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "유로(€)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "영국 파운드(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    border(COL2);

    yenToAnything(pound, choice);
}

void poundToAnything(double money, int choice) {
    std::string targetname = "달러";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "달러";
        target = 1;
        break;
    case 2:
        targetname = "원";
        target = XKRW;
        break;
    case 3:
        targetname = "유로";
        target = XEUR;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "파운드는 " << money / XGBP * target << targetname << "입니다." << std::endl;
    printf("%d파운드는 %f%s입니다.", int(money), money / XGBP * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void poundAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "변환할 값어치를 입력하여 주십시오.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환할 값어치를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;

    std::cout << std::setw(COL1) << std::left << "미국 달러($)" << std::right << "1(기본값)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "대한민국 원(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "유로(€)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "일본 엔(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    border(COL2);

    poundToAnything(pound, choice);
}

void euroToAnything(double money, int choice) {
    std::string targetname = "달러";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "달러";
        target = 1;
        break;
    case 2:
        targetname = "원";
        target = XKRW;
        break;
    case 3:
        targetname = "파운드";
        target = XGBP;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "유로는 " << money / XEUR * target << targetname << "입니다." << std::endl;
    printf("%d유로는 %f%s입니다.", (int)money, money / XEUR * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void euroAskSeg() {
    long double eur;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "변환할 값어치를 입력하여 주십시오.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> eur;

        if (std::cin.fail() || eur <= 0 ) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환할 값어치를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;

    std::cout << std::setw(COL1) << std::left << "미국 달러($)" << std::right << "1(기본값)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "대한민국 원(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "영국 파운드(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "일본 엔(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    border(COL2);

    euroToAnything(eur, choice);
}

void wonToAnything(double money, int choice) {
    std::string targetname = "달러";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "달러";
        target = 1;
        break;
    case 2:
        targetname = "유로";
        target = XEUR;
        break;
    case 3:
        targetname = "파운드";
        target = XGBP;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "원은 " << money / XKRW * target << targetname << "입니다." << std::endl;
    printf("%d원은 %f%s입니다.", int(money), money / XKRW * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void wonAskSeg() {
    long double won;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "변환할 값어치를 입력하여 주십시오.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> won;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환할 값어치를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;

    std::cout << std::setw(COL1) << std::left << "미국 달러($)" << std::right << "1(기본값)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "유로(€)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "영국 파운드(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "일본 엔(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;
    
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    border(COL2);

    wonToAnything(won, choice);
}

void dollarToAnything(double money, int choice) {
    std::string targetname = "원";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "원";
        target = XKRW;
        break;
    case 2:
        targetname = "유로";
        target = XEUR;
        break;
    case 3:
        targetname = "파운드";
        target = XGBP;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "달러는 " << money * target << targetname << "입니다." << std::endl;
    printf("%d달러는 %f%s입니다.", int(money), money * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void dollarAskSeg() {
    long double dollar;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "변환할 값어치를 입력하여 주십시오.($)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> dollar;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환할 값어치를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;

    std::cout << std::setw(COL1) << std::left << "대한민국 원(\\)" << std::right << "1(기본값)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "유로(€)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "영국 파운드(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "일본 엔(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "올바르지 않은 입력입니다." << std::endl;
            border(COL2);
            std::cout << "변환하고자 하는 상대 재화를 입력하여 주십시오." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    border(COL2);

    dollarToAnything(dollar, choice);
}

void selectYourMoneySeg() {
    std::string choice;
    std::cout << ">>" << std::ends;
    std::cin >> choice;
    //scanf("%c", &choice);

    if (choice == "1") {
        wonAskSeg();
    }
    else if (choice == "2") {
        dollarAskSeg();
    }
    else if (choice == "3") {
        euroAskSeg();
    }
    else if (choice == "4") {
        poundAskSeg();
    }
    else if (choice == "5") {
        yenAskSeg();
    }
    else {
        std::cout << "올바른 형식의 입력이 아닙니다." << std::ends;
        selectYourMoneySeg();
    }
}

void welcomeSeg() {          //프로그램 시작
    std::cout << "환율 계산기에 오신 것을 환영합니다. 원하는 작업을 선택하여 번호를 입력하여 주십시오." << std::endl;
    std::cout << "최신의 환율을 가져오기 위해 인터넷 연결이 필요합니다." << std::endl;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "원\\에서 변환" << std::right << "1(기본값)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "달러$에서 변환" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1+1) << std::left << "유로€에서 변환 " << std::right << "3" << std::endl;           //유로 기호가 칼럼을 1칸 줄임.이유불명
    std::cout << std::setw(COL1) << std::left << "파운드?에서 변환" << std::right << "4" << std::endl;
    std::cout << std::setw(COL1) << std::left << "엔?에서 변환" << std::right << "5" << std::endl;

    selectYourMoneySeg();
}

void otherBuisnessSeg() {            //재시작 or 종료
    char inp;
    std::cout << std::left << "다른 작업을 하시겠습니까?" << std::endl;
    std::cout << std::setw(COL2) << std::right << "재시작:y(기본값)" << std::endl;
    std::cout << std::setw(COL2) << std::right << "취소 :n" << std::endl;
    std::cout << ">>" << std::ends;

    std::cin >> inp;

    switch (inp) {
        case 'y':
            reTerm();
            welcomeSeg();
            break;
        case 'n':
            break;
        case '\n':
            reTerm();
            welcomeSeg();
        default:
            std::cout << "올바른 형식의 입력이 아닙니다." << std::ends;
            otherBuisnessSeg();
    }
}


// 셰종어졔훈민졍ㅡ음
// 나라ㅅ말싸미 듕귁에달아 문짜와로서르사맛디아니할쌔 이런젼차로어린백셩이니르고져홇배이셔도 마침내제뜨들시러펴지몯홇노미하니라 내이룰위하야어엿비너겨 새로스물여듧짜룰맹그노니 사람마다하여수Vㅣ니겨날로쑤메뼌한킈하고져핧ㅅ따르미니라.