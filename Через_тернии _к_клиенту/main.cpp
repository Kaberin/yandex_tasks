#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <utility>
using std::vector;
using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::cout;
using std::map;

enum class Status {
    Accepted,
    Boarding,
    Success,
    Cancelled,
    Empty //Only for default value
};

ostream& operator<<(ostream& os, const Status& status) {
    switch (status) {
    case Status::Accepted:
        os << "Accepted";
        break;
    case Status::Boarding:
        os << "Boarding";
        break;
    case Status::Success:
        os << "Success";
        break;
    case Status::Cancelled:
        os << "Cancelled";
        break;
    default:
        os << "Empty";
    }
    return os;
}

class Date {
public:
    int day;
    int hour;
    int minute;
    Date() : day{ 0 }, hour{ 0 }, minute{ 0 } {}
    Date(int d, int h, int m) : day{ d }, hour{ h }, minute{ m } {}
};
bool operator>(const Date& date1, const Date& date2) {
    if (date1.day > date2.day) {
        return true;
    } else if (date1.day < date2.day) {
        return false;
    }
    if (date1.hour > date2.hour) {
        return true;
    } else if (date1.hour < date2.hour) {
        return false;
    }
    if (date1.minute > date2.minute) {
        return true;
    } else if (date1.minute < date2.minute) {
        return false;
    }
    return false;
}
bool operator<(const Date& date1, const Date& date2) {
    return !(date1 > date2);
}
bool operator>=(const Date& date1, const Date& date2) {
    if (date1.day >= date2.day) {
        return true;
    }
    if (date1.hour >= date2.hour) {
        return true;
    }
    if (date1.minute >= date2.minute) {
        return true;
    }
    return false;
}
bool operator<=(const Date& date1, const Date& date2) {
    return !(date1 >= date2);
}
bool operator==(const Date& date1, const Date& date2) {
    if (date1.day == date2.day && date1.hour == date2.hour && date1.minute == date2.minute) {
        return true;
    }
    return false;
}
bool operator!=(const Date& date1, const Date& date2) {
    return !(date1 == date2);
}
//Возвращает время между датами в минутах
int operator-(Date& date1, Date& date2) {
    int minutes1 = (date1.day - 1) * 24 * 60 + date1.hour * 60 + date1.minute;
    int minutes2 = (date2.day - 1) * 24 * 60 + date2.hour * 60 + date2.minute;
    return fabs(minutes1 - minutes2);
}


istream& operator>>(istream& is, Date& date) {
    int day{}, hour{}, minute{};
    is >> day >> hour >> minute;
    date.day = day;
    date.hour = hour;
    date.minute = minute;
    return is;
}


class Entry {
public:
    Entry() : date{}, id{}, status{ Status::Empty } {}
    Date date;
    long int id;
    Status status;
};

istream& operator>>(istream& is, Entry& entry) {
    Date date{};
    Status status{};
    long int id{};
    is >> date >> id;
    char st{};
    is >> st;
    switch (st) {
    case 'A':
        status = Status::Accepted;
        break;
    case 'B':
        status = Status::Boarding;
        break;
    case 'S':
        status = Status::Success;
        break;
    case 'C':
        status = Status::Cancelled;
        break;
    }
    entry.date = date;
    entry.id = id;
    entry.status = status;
    return is;
}

ostream& operator<<(ostream& os, const Entry& entry) {
    os << "Ship ID: " << entry.id << '\n';
    os << "Drive Status: " << entry.status << '\n';
    os << "Entry Date: " << '\n' << "\t Day: " << entry.date.day << '\n' << "\t Hour: " << entry.date.hour << '\n' << "\t Minute: " << entry.date.minute << '\n' << '\n';
    return os;
}


void read_entries(vector<Entry>& entries, ifstream& input) {
    for (int i = 0; i < entries.size(); ++i) {
        Entry entry;
        std::string str{};
        std::getline(input, str);
        std::stringstream str_stream{ str };
        str_stream >> entry;
        entries[i] = entry;
        //cout << entry;
    }
}

void sort_by_date(vector<Entry>& data) {
    std::sort(data.begin(), data.end(), [](const Entry& e1, const Entry& e2) {
        return e1.date < e2.date;
    });
}
//Читаем всё в вектор
//Классифицируем по id
//Сортируем по дате
//Вычисляем время:
//  Если стаус принят то идем до статуса отмены или успеха
//  Время = Отмена - Принят или Время = Успех - Принят
//  Нужно перегрузить - '-' для даты 
//
//

int main() {
    ifstream input{ "input.txt" };
    if (!input) {
        return 1;
    }
    int entires_amount{};
    input >> entires_amount;
    input.ignore();

    vector<Entry> entries(entires_amount);
    read_entries(entries, input);
    input.close();
    map<long int, vector<Entry>> classified_entries{};

    for (auto entry : entries) {
        classified_entries[entry.id].push_back(entry);
    }
    entries.clear();

    for (auto& ship_log : classified_entries) {
        sort_by_date(ship_log.second);
    }

    // for (auto x : classified_entries) {
    //     cout << "Log for ship with ID: " << x.first << '\n' << '\n';
    //     for (auto y : x.second) {
    //         cout << y;
    //         cout << "\n\n";
    //     }
    //     cout << "\n\n";
    // }

    vector<std::pair<long int, int>> result{};

    for (auto ship_log : classified_entries) {
        vector<Entry> log = ship_log.second;
        int total_time{};
        Date d1 = log[0].date;
        Date d2{};
        for (int i = 1; i < log.size(); ++i) {
            if (log[i].status == Status::Cancelled || log[i].status == Status::Success) {
                d2 = log[i].date;
                total_time += (d2 - d1);
            } else if (log[i].status == Status::Boarding) {
                continue;
            } else if (log[i].status == Status::Accepted) {
                d1 = log[i].date;
            }
        }
        result.push_back(std::make_pair(ship_log.first, total_time));
    }

    // for (auto x : result) {
    //     std::cout << "Ship ID: " << x.first << " Flying time " << x.second << "\n\n";
    // }

    ofstream output{ "output.txt" };
    if (!output) {
        return 1;
    }
    for (auto x : result) {
        output << x.second << " ";
    }
    output.close();
    return 0;
}