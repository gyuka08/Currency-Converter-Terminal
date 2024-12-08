#include <iostream>
#include <iomanip>
#include <string>
#include <curl/curl.h>          //Communication Header
#include "nlohmann/json.hpp"            //nlohmann JSON
#include "japdongsani.h"
#include <cstdlib>

using json = nlohmann::json;

struct aPIBasket {           //Store data from API
    std::string DefaultCurrency;
    std::vector<std::string> Targets;
    std::vector<double> USDexchange;
};

//Callback API
size_t burnCallback(void* contents, size_t size, size_t nmemb, std::string* response) {
    response->append((char*)contents, size * nmemb);
    return size * nmemb;
}

//get Exchange Rates
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

const int COL1 = 35;            //Text Width 1
const int COL2 = 80;            //Text Width 2

void wonToAnything();
void wonAskSeg();
void dollarToAnything();
void dollarAskSeg();
void selectYourMoneySeg();
void welcomeSeg();
void otherBuisnessSeg();

int main() {
    std::string DefaultCurrency;
    DefaultCurrency = "USD";            //Make Dollar for Default Currency

    aPIBasket Basket;

    //Connectin failure
    if (!fetchExchangeRates(DefaultCurrency, Basket)) {
        std::cout << "WARNING : Cannot fetch Exchange Rates. Check if your Internet connection is stable." << std::endl;
        std::cout << "WARNING : The Connection is Offline. The Program will use Pre-typed Values for Exchange." << std::endl;
        border(COL2);
    }
    else if (fetchExchangeRates(DefaultCurrency, Basket)) {          //Connection Success
        //Store Dollar-Won Rates
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "KRW") {
                XKRW = Basket.USDexchange[i];
                break;
            }
        }

        //Store Dollar-Euro Rates
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "EUR") {
                XEUR = Basket.USDexchange[i];
                break;
            }
        }

        //Store Dollar-Pound Rates
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "GBP") {
                XGBP = Basket.USDexchange[i];
                break;
            }
        }

        //Store Dollar-Yen Rates
        for (size_t i = 0; i < Basket.Targets.size(); ++i) {
            if (Basket.Targets[i] == "JPY") {
                XJPY = Basket.USDexchange[i];
                break;
            }
        }
    }

    //Call Segment 'Welcome'
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
        targetname = "Euro";
        target = XEUR;
        break;
    case 4:
        targetname = "GBP";
        target = XGBP;
        break;
    }

    printf("%dYen Exchanges to %f%s.", int(money), money / XJPY * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void yenAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Enter Amount of Bills to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Amount of Bills to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "U.S. Dollar($)" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Republic of Korea Won(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Euro(€)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Great Britain Pound(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;
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
        targetname = "Euro";
        target = XEUR;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    printf("%dPound Exchanges to %f%s.", int(money), money / XGBP * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void poundAskSeg() {
    double pound;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Enter Amount of Bills to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> pound;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Amount of Bills to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "U.S. Dollar($)" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Republic of Korea Won(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Euro(€)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Japan Yen(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;
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
        targetname = "엔";
        target = XJPY;
        break;
    }

    printf("%d Euro Exchanges to %f%s.", (int)money, money / XEUR * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void euroAskSeg() {
    long double eur;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Enter Amount of Bills to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> eur;

        if (std::cin.fail() || eur <= 0 ) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Amount of Bills to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "U.S. Dollar($)" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Republic of Korea Won(\\)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Great Britain Pound(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Japan Yen(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;
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
        targetname = "Euro";
        target = XEUR;
        break;
    case 3:
        targetname = "GBP";
        target = XGBP;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    printf("%dWon Exchanges to %f%s.", int(money), money / XKRW * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void wonAskSeg() {
    long double won;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Enter Amount of Bills to Exchange.(\\)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> won;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Amount of Bills to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "U.S. Dollar($)" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Euro(€)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Great Britain Pound(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Japan Yen(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;
    
    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;
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
        targetname = "Euro";
        target = XEUR;
        break;
    case 3:
        targetname = "GBP";
        target = XGBP;
        break;
    case 4:
        targetname = "엔";
        target = XJPY;
        break;
    }

    printf("$%d Exchanges to %f%s.", int(money), money * target, targetname.c_str());
    std::cout << std::endl;

    border(COL2);

    otherBuisnessSeg();
}

void dollarAskSeg() {
    long double dollar;
    int choice;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Enter Amount of Bills to Exchange.($)" << std::endl;
    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> dollar;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Amount of Bills to Exchange." << std::endl;
            std::cout << ">>" << std::ends;
        }
        else {
            break;
        }
    }

    std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;

    std::cout << std::setw(COL1) << std::left << "Republic of Korea Won(\\)" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Euro(€)" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Great Britain Pound(?)" << std::right << "3" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Japan Yen(?)" << std::right << "4" << std::endl;

    std::cout << ">>" << std::ends;

    while (true) {
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cout << "Invalid." << std::endl;
            border(COL2);
            std::cout << "Enter Target Currency to Exchange Your value to." << std::endl;
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
        std::cout << "Invalid." << std::ends;
        selectYourMoneySeg();
    }
}

void welcomeSeg() {          //Program Start
    std::cout << "Welcome to Currency Converter. Enter a Number of wanted Task." << std::endl;
    std::cout << "Internet Connection Required to get latest Exchange Rates." << std::endl;

    border(COL2);

    std::cout << std::setw(COL1) << std::left << "Exchange from KRW\\" << std::right << "1(Default)" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Exchange from USD$" << std::right << "2" << std::endl;
    std::cout << std::setw(COL1+1) << std::left << "Exchange from Euro€" << " " << std::right << "3" << std::endl;           //Euro Character(€) will Reduce Column by 1.
    std::cout << std::setw(COL1) << std::left << "Exchange from GBP?" << std::right << "4" << std::endl;
    std::cout << std::setw(COL1) << std::left << "Exchange from JPY?" << std::right << "5" << std::endl;

    selectYourMoneySeg();
}

void otherBuisnessSeg() {            //Reboot and Cancel
    char inp;
    std::cout << std::left << "Will you Restart your Task again?" << std::endl;
    std::cout << std::setw(COL2) << std::right << "Reboot : y(Default)" << std::endl;
    std::cout << std::setw(COL2) << std::right << "Cancel : n" << std::endl;
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
            std::cout << "Invalid." << std::ends;
            otherBuisnessSeg();
    }
}