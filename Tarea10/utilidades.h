#pragma once
#pragma once
#include <string>
#include <algorithm>
using namespace std;
// Elimina espacios al inicio y al final de un string
inline string trim(const string& s) {
    string result = s;
    result.erase(0, result.find_first_not_of(" \t"));
    result.erase(result.find_last_not_of(" \t") + 1);
    return result;
}