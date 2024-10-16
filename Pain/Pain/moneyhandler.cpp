#include <iostream>
#include <iomanip>
#include <string>
#include <curl/curl.h>          //��� ���
#include "nlohmann/json.hpp"            //nlohmann�� json �б� ���(Github)
#include "japdongsani.h"            //��Ÿ �Լ����� �����ϴ� ����� ���
#include <cstdlib>

using json = nlohmann::json;

struct aPIBasket {           //API���� ���� �����͸� ����(Github ������)
    std::string DefaultCurrency;
    std::vector<std::string> Targets;
    std::vector<double> USDexchange;
};

//API�� �ݹ�(Github ������)
size_t burnCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    response->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//API�κ��� ȯ�� �� ��������(Github ������)
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

const int COL1 = 35;            //�͹̳��� �ʺ� 1
const int COL2 = 80;            //�͹̳��� �ʺ� 2

void wonToAnything();
void wonAskSeg();
void dollarToAnything();
void dollarAskSeg();
void selectYourMoneySeg();
void welcomeSeg();
void otherBuisnessSeg();

int main() {
    std::string DefaultCurrency;
    DefaultCurrency = "USD";            //���� ȯ���� �޷��� ����

    aPIBasket Basket;

    //���� ����
    if (!fetchExchangeRates(DefaultCurrency, Basket)) {
        std::cout << "����: ȯ���� �������� ���߽��ϴ�. ���� ����̰ų� ��ǻ�Ͱ� ��Ʈ��ũ�� �����ϰ� �;��� �� �ֽ��ϴ�." << std::endl;
        std::cout << "����: ������ ���������Դϴ�. ���α׷��� ������ �ٻ簪�� ����Ͽ� ����մϴ�." << std::endl;
        border(COL2);
    }
    else if (fetchExchangeRates(DefaultCurrency, Basket)) {          //���� ����
        //�޷� �� �� ȯ���� ����
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "KRW") {
                XKRW = Basket.USDexchange[i];
                break;
            }
        }

        //�޷� �� ���� ȯ���� ����
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "EUR") {
                XEUR = Basket.USDexchange[i];
                break;
            }
        }

        //�޷� �� �Ŀ�� ȯ���� ����
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "GBP") {
                XGBP = Basket.USDexchange[i];
                break;
            }
        }

        //�޷� �� �� ȯ���� ����
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "JPY") {
                XJPY = Basket.USDexchange[i];
                break;
            }
        }
    }

    //���� �κ� �ҷ�����
    welcomeSeg();

    return 0;
}

void yenToAnything(double money, int choice) {
    std::string targetname = "�޷�";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "�޷�";
        target = XJPY;
        break;
    case 2:
        targetname = "��";
        target = XKRW;
        break;
    case 3:
        targetname = "����";
        target = XEUR;
        break;
    case 4:
        targetname = "�Ŀ��";
        target = XGBP;
        break;
    }

    //std::cout << int(money) << "���� " << money / XJPY * target << targetname << "�Դϴ�." << std::endl;
    printf("%d���� %f%s�Դϴ�.", int(money), money / XJPY * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void yenAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;

    std::cout << std::setw(COL1) << std::left << "�̱� �޷�($)" << std::right << "1(�⺻��)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���ѹα� ��(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "����(��)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���� �Ŀ��(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
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
    std::string targetname = "�޷�";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "�޷�";
        target = 1;
        break;
    case 2:
        targetname = "��";
        target = XKRW;
        break;
    case 3:
        targetname = "����";
        target = XEUR;
        break;
    case 4:
        targetname = "��";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "�Ŀ��� " << money / XGBP * target << targetname << "�Դϴ�." << std::endl;
    printf("%d�Ŀ��� %f%s�Դϴ�.", int(money), money / XGBP * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void poundAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;

    std::cout << std::setw(COL1) << std::left << "�̱� �޷�($)" << std::right << "1(�⺻��)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���ѹα� ��(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "����(��)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "�Ϻ� ��(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
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
    std::string targetname = "�޷�";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "�޷�";
        target = 1;
        break;
    case 2:
        targetname = "��";
        target = XKRW;
        break;
    case 3:
        targetname = "�Ŀ��";
        target = XGBP;
        break;
    case 4:
        targetname = "��";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "���δ� " << money / XEUR * target << targetname << "�Դϴ�." << std::endl;
    printf("%d���δ� %f%s�Դϴ�.", (int)money, money / XEUR * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void euroAskSeg() {
    long double eur;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> eur;

        if (std::cin.fail() || eur <= 0 ) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;

    std::cout << std::setw(COL1) << std::left << "�̱� �޷�($)" << std::right << "1(�⺻��)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���ѹα� ��(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���� �Ŀ��(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "�Ϻ� ��(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
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
    std::string targetname = "�޷�";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "�޷�";
        target = 1;
        break;
    case 2:
        targetname = "����";
        target = XEUR;
        break;
    case 3:
        targetname = "�Ŀ��";
        target = XGBP;
        break;
    case 4:
        targetname = "��";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "���� " << money / XKRW * target << targetname << "�Դϴ�." << std::endl;
    printf("%d���� %f%s�Դϴ�.", int(money), money / XKRW * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void wonAskSeg() {
    long double won;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> won;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;

    std::cout << std::setw(COL1) << std::left << "�̱� �޷�($)" << std::right << "1(�⺻��)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "����(��)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���� �Ŀ��(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "�Ϻ� ��(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;
    
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
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
    std::string targetname = "��";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "��";
        target = XKRW;
        break;
    case 2:
        targetname = "����";
        target = XEUR;
        break;
    case 3:
        targetname = "�Ŀ��";
        target = XGBP;
        break;
    case 4:
        targetname = "��";
        target = XJPY;
        break;
    }

    //std::cout << int(money) << "�޷��� " << money * target << targetname << "�Դϴ�." << std::endl;
    printf("%d�޷��� %f%s�Դϴ�.", int(money), money * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void dollarAskSeg() {
    long double dollar;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�.($)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> dollar;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�� ����ġ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;

    std::cout << std::setw(COL1) << std::left << "���ѹα� ��(\\)" << std::right << "1(�⺻��)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "����(��)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "���� �Ŀ��(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "�Ϻ� ��(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "�ùٸ��� ���� �Է��Դϴ�." << std::endl;
            border(COL2);
            std::cout << "��ȯ�ϰ��� �ϴ� ��� ��ȭ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
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
        std::cout << "�ùٸ� ������ �Է��� �ƴմϴ�." << std::ends;
        selectYourMoneySeg();
    }
}

void welcomeSeg() {          //���α׷� ����
    std::cout << "ȯ�� ���⿡ ���� ���� ȯ���մϴ�. ���ϴ� �۾��� �����Ͽ� ��ȣ�� �Է��Ͽ� �ֽʽÿ�." << std::endl;
    std::cout << "�ֽ��� ȯ���� �������� ���� ���ͳ� ������ �ʿ��մϴ�." << std::endl;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "��\\���� ��ȯ" << std::right << "1(�⺻��)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "�޷�$���� ��ȯ" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1+1) << std::left << "���΢濡�� ��ȯ " << std::right << "3" << std::endl;           //���� ��ȣ�� Į���� 1ĭ ����.�����Ҹ�
    std::cout << std::setw(COL1) << std::left << "�Ŀ��?���� ��ȯ" << std::right << "4" << std::endl;
    std::cout << std::setw(COL1) << std::left << "��?���� ��ȯ" << std::right << "5" << std::endl;

    selectYourMoneySeg();
}

void otherBuisnessSeg() {            //����� or ����
    char inp;
    std::cout << std::left << "�ٸ� �۾��� �Ͻðڽ��ϱ�?" << std::endl;
    std::cout << std::setw(COL2) << std::right << "�����:y(�⺻��)" << std::endl;
    std::cout << std::setw(COL2) << std::right << "��� :n" << std::endl;
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
            std::cout << "�ùٸ� ������ �Է��� �ƴմϴ�." << std::ends;
            otherBuisnessSeg();
    }
}


// ���������ƹ�������
// ���󤵸��ι� ��ο��޾� ��¥�ͷμ��������ƴ��ҽ� �̷������ξ����̴ϸ������I���̼ŵ� ��ħ�����ߵ�÷��������I����ϴ϶� ���̷����Ͼ߾��ʰ� ���ν�������¥��ͱ׳�� ��������Ͽ���V�Ӵϰܳ��ξ��ޖ��Ѵ��ϰ����C�������̴϶�.