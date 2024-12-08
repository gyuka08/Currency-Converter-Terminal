#include <iostream>
#include <iomanip>
#include <string>
#include <curl/curl.h>          //Communication Header
#include "nlohmann/json.hpp"            //json reading by nlohmann
#include "japdongsani.h"            //user header
#include <cstdlib>

using json = nlohmann::json;

struct aPIBasket {           //save data from API
    std::string DefaultCurrency;
    std::vector<std::string> Targets;
    std::vector<double> USDexchange;
};

//API Callback
size_t burnCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    response->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//get Currency value from API
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

const int COL1 = 35;            //Width 1
const int COL2 = 80;            //Width 2

void wonToAnything();
void wonAskSeg();
void dollarToAnything();
void dollarAskSeg();
void selectYourMoneySeg();
void welcomeSeg();
void otherBuisnessSeg();

int main() {
    std::string DefaultCurrency;
    DefaultCurrency = "USD";            //$ for default

    aPIBasket Basket;

    //Connection Failure
    if (!fetchExchangeRates(DefaultCurrency, Basket)) {
        std::cout << "WARNING : API NOT RESPONSIVE. YOU MAY NEED TO CONNECT INTERNET" << std::endl;
        std::cout << "WARNING : THE CONNECTION IS OFFLINE. THE PROGRAM WILL USE APPROXIMATE EXCHANGE RATE." << std::endl;
        border(COL2);
    }
    else if (fetchExchangeRates(DefaultCurrency, Basket)) {          //Connection
        //Rate USD : KRW
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "KRW") {
                XKRW = Basket.USDexchange[i];
                break;
            }
        }

        //Rate USD : EUR
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "EUR") {
                XEUR = Basket.USDexchange[i];
                break;
            }
        }

        //Rate USD : GBP
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "GBP") {
                XGBP = Basket.USDexchange[i];
                break;
            }
        }

        //Rate USD : JPY
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "JPY") {
                XJPY = Basket.USDexchange[i];
                break;
            }
        }
    }

    //Call Welcome
    welcomeSeg();

    return 0;
}

void yenToAnything(double money, int choice) {
    std::string targetname = "USD";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "USD";
        target = XJPY;
        break;
    case 2:
        targetname = "KRW";
        target = XKRW;
        break;
    case 3:
        targetname = "EUR";
        target = XEUR;
        break;
    case 4:
        targetname = "GBP";
        target = XGBP;
        break;
    }

    printf("%dYEN exchanges to %f%s.", int(money), money / XJPY * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void yenAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Input Value to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Value to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Input Currency to Exchange to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "USD($)" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "KRW(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "EUR(€)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "GBP(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Currency to Exchange to." << std::endl;
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
    std::string targetname = "USD";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "USD";
        target = 1;
        break;
    case 2:
        targetname = "KRW";
        target = XKRW;
        break;
    case 3:
        targetname = "EUR";
        target = XEUR;
        break;
    case 4:
        targetname = "YEN";
        target = XJPY;
        break;
    }

    printf("%dGBP exchanges to %f%s.", int(money), money / XGBP * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void poundAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Input Value to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Value to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Input Currency to Exchange to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "USD($)" << std::right << "1(Defalut)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "KRW(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "EUR(€)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "YEN(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Currency to Exchange to." << std::endl;
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
    std::string targetname = "USD";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "USD";
        target = 1;
        break;
    case 2:
        targetname = "KRW";
        target = XKRW;
        break;
    case 3:
        targetname = "GBP";
        target = XGBP;
        break;
    case 4:
        targetname = "YEN";
        target = XJPY;
        break;
    }

    printf("%dEUR exchanges to %f%s.", (int)money, money / XEUR * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void euroAskSeg() {
    long double eur;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Input Value to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> eur;

        if (std::cin.fail() || eur <= 0 ) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Value to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Input Currency to Exchange to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "USD($)" << std::right << "1(Defalut)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "KRW(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "GBP(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "YEN(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Currency to Exchange to." << std::endl;
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
    std::string targetname = "USD";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "USD";
        target = 1;
        break;
    case 2:
        targetname = "EUR";
        target = XEUR;
        break;
    case 3:
        targetname = "GBP";
        target = XGBP;
        break;
    case 4:
        targetname = "YEN";
        target = XJPY;
        break;
    }

    printf("%dKRW은 %f%s.", int(money), money / XKRW * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void wonAskSeg() {
    long double won;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Input Value to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> won;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Value to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Input Currency to Exchange to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "USD($)" << std::right << "1(Defalut)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "EUR(€)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "GBP(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "YEN(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;
    
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Currency to Exchange to." << std::endl;
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
    std::string targetname = "KRW";
    double target = XKRW;
    switch (choice) {
    case 1:
        targetname = "KRW";
        target = XKRW;
        break;
    case 2:
        targetname = "EUR";
        target = XEUR;
        break;
    case 3:
        targetname = "GBP";
        target = XGBP;
        break;
    case 4:
        targetname = "YEN";
        target = XJPY;
        break;
    }

    printf("%dUSD exchanges to %f%s.", int(money), money * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void dollarAskSeg() {
    long double dollar;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Input Value to Exchange.($)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> dollar;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Value to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Input Currency to Exchange to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "KRW(\\)" << std::right << "1(Defalut)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "EUR(€)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "GBP(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "YEN(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Input Currency to Exchange to." << std::endl;
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
        std::cout << "Invalid format." << std::ends;
        selectYourMoneySeg();
    }
}

void welcomeSeg() {          //Program start
    std::cout << "Welcome to Currency Converter. Choose a Currency to start with." << std::endl;
    std::cout << "Internet Connection is needed to get Latest Exchange Rate." << std::endl;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Exchange from KRW\\" << std::right << "1(Defalut)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Exchange from USD$" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1+1) << std::left << "Exchange from EUR€ " << std::right << "3" << std::endl;           //EUR 기호가 칼럼을 1칸 줄임.이유불명
    std::cout << std::setw(COL1) << std::left << "Exchange from GBP?" << std::right << "4" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Exchange from YEN?" << std::right << "5" << std::endl;

    selectYourMoneySeg();
}

void otherBuisnessSeg() {            //Restart eller Quit
    char inp;
    std::cout << std::left << "Choose wanted Task" << std::endl;
    std::cout << std::setw(COL2) << std::right << "Restart:y(Defalut)" << std::endl;
    std::cout << std::setw(COL2) << std::right << "Cancel :n" << std::endl;
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
            std::cout << "Invalid format." << std::ends;
            otherBuisnessSeg();
    }
}
