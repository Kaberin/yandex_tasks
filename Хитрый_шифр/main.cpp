#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <map>
#include <iomanip>
using std::string;
using std::istream;
using std::ofstream;
using std::vector;
using std::map;
int digits_sum(int);
//!ЭТО ПИЗДЕЦ А НЕ РЕШЕНИЕ
struct date {
    date() :day{}, month{}, year{} {}
    int day;
    int month;
    int year;
};

istream& operator>>(istream& is, date& d) {
    char skip_char{};
    int day, month, year;
    is >> day >> skip_char >> month >> skip_char >> year;
    d.day = day; d.month = month; d.year = year;
    return is;
};


//Govnokod on
int alphabetic_number(char c) {
    switch (c) {
    case 'a': case 'A':
        return 1;
    case 'b': case 'B':
        return 2;
    case 'c': case 'C':
        return 3;
    case 'd': case 'D':
        return 4;
    case 'e': case 'E':
        return 5;
    case 'f': case 'F':
        return 6;
    case 'g':case 'G':
        return 7;
    case 'h':case 'H':
        return 8;
    case 'i':case 'I':
        return 9;
    case 'j': case 'J':
        return 10;
    case 'k':case 'K':
        return 11;
    case 'l': case 'L':
        return 12;
    case 'm': case 'M':
        return 13;
    case 'n': case 'N':
        return 14;
    case 'o': case 'O':
        return 15;
    case 'p':case 'P':
        return 16;
    case 'q': case 'Q':
        return 17;
    case 'r': case 'R':
        return 18;
    case 's':case 'S':
        return 19;
    case 't':case 'T':
        return 20;
    case 'u': case 'U':
        return 21;
    case 'v':case 'V':
        return 22;
    case 'w': case 'W':
        return 23;
    case 'x': case 'X':
        return 24;
    case 'y': case 'Y':
        return 25;
    case 'z': case 'Z':
        return 26;
    default:
        return 0;
    }
}
//Govnokod off
struct user {
    user() :name{}, surname{}, patronymic{}, birthdate{} {}
    string name;
    string surname;
    string patronymic; //Отчество
    date birthdate;
    map<char, int> symbols_amount{};
    int symbols_sum{};
    int birthdate_sum{};
    int surname_firstletter_number{};
    int total_sum{};

    void find_surname_firstletter_number() {
        surname_firstletter_number = alphabetic_number(surname[0]);
        surname_firstletter_number *= 256;
    }
    void calcuate_birthday_sum() {
        int result;
        result += digits_sum(birthdate.day);
        result += digits_sum(birthdate.month);
        result *= 64;
        birthdate_sum = result;
    }
    void count_symbols() {
        string fullname{};
        fullname += name;
        fullname += surname;
        fullname += patronymic;
        for (auto x : fullname) {
            symbols_amount[x]++;
        }
        for (auto x : symbols_amount) {
            symbols_sum++;
        }
    }
    int result{};
};

istream& operator>>(istream& is, user& u) {
    date d{};
    string name{};
    string surname{};
    string patronymic{};

    int comma_counter = 0;
    char ch{};
    while (is >> ch) {
        if (ch == '\n') break;
        if (ch == ',') {
            comma_counter++;
            continue;
        }

        switch (comma_counter) {
        case 0:
            surname += ch;
            break;
        case 1:
            name += ch;
            break;
        case 2:
            patronymic += ch;
            break;
        case 3:
            is.putback(ch);
            is >> d;
            comma_counter = -1;
            break;
        }
    }

    u.name = name;
    u.surname = surname;
    u.patronymic = patronymic;
    u.birthdate = d;

    return is;
}

vector<user> users{};

void read_users() {
    std::ifstream file{ "input.txt" };
    if (!file) {
        std::cout << "Error opening\n";
        return;
    }
    int number{};
    file >> number;
    file.ignore();
    for (int i = 0; i < number; i++) {
        user u;
        string s;
        std::getline(file, s);
        std::stringstream ss{ s };
        ss >> u;
        users.push_back(u);
    }
    file.close();
}

// void print_users() {
//     for (auto x : users) {
//         std::cout << x.surname << ' ' << x.name << ' ' << x.patronymic << ' ' << x.birthdate.day << ' ' << x.birthdate.month << ' ' << x.birthdate.year << '\n';
//     }
// }

int digits_sum(int number) {
    int result{};
    while (number != 0) {
        int digit = number % 10;
        result += digit;
        number /= 10;
    }
    return result;
}

int three_radix(int x) {
    int res{};
    int mask = 0xFFF;
    res = (x & mask);
    // std::cout << std::hex << res << std::endl;
    return res;
}





int main() {
    read_users();
    ofstream output{ "output.txt" };
    if (!output) {
        return 1;
    }

    for (auto x : users) {
        x.count_symbols();
        x.calcuate_birthday_sum();
        x.find_surname_firstletter_number();
        x.total_sum += x.birthdate_sum;
        x.total_sum += x.symbols_sum;
        x.total_sum += x.surname_firstletter_number;
        x.result = three_radix(x.total_sum);

        output << std::hex << std::uppercase << std::setfill('0') << std::setw(3) << x.result << std::endl;
    }



    return 0;
}