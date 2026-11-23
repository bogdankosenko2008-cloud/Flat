#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cctype>

class SimpleJson {
public:
    // Зчитування всього файлу у рядок
    static std::string readFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Помилка відкриття файлу: " << path << std::endl;
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // Запис рядка у файл
    static bool writeFile(const std::string& path, const std::string& content) {
        std::ofstream file(path);
        if (!file.is_open()) return false;
        file << content;
        return true;
    }

    // Отримання рядкового значення за ключем ("key": "value")
    static std::string getString(const std::string& json, const std::string& key, size_t startPos = 0) {
        std::string search = "\"" + key + "\"";
        size_t pos = json.find(search, startPos);
        if (pos == std::string::npos) return "";
        pos = json.find(":", pos + search.length());
        size_t firstQuote = json.find("\"", pos + 1);
        if (firstQuote == std::string::npos) return "";
        size_t secondQuote = json.find("\"", firstQuote + 1);
        return json.substr(firstQuote + 1, secondQuote - firstQuote - 1);
    }

    // Отримання числового значення за ключем ("key": 12.5)
    static double getNumber(const std::string& json, const std::string& key, size_t startPos = 0) {
        std::string search = "\"" + key + "\"";
        size_t pos = json.find(search, startPos);
        if (pos == std::string::npos) return 0.0;
        pos = json.find(":", pos + search.length()) + 1;
        while (pos < json.size() && (std::isspace(json[pos]) || json[pos] == '\"')) pos++;
        size_t endPos = pos;
        while (endPos < json.size() && (std::isdigit(json[endPos]) || json[endPos] == '.' || json[endPos] == '-')) endPos++;
        if (pos == endPos) return 0.0;
        return std::stod(json.substr(pos, endPos - pos));
    }

    // Отримання булевого значення ("key": true/false)
    static bool getBool(const std::string& json, const std::string& key, size_t startPos = 0) {
        std::string search = "\"" + key + "\"";
        size_t pos = json.find(search, startPos);
        if (pos == std::string::npos) return false;
        pos = json.find(":", pos + search.length()) + 1;
        while (pos < json.size() && std::isspace(json[pos])) pos++;
        return json.substr(pos, 4) == "true";
    }

    // Розбиття JSON-масиву об'єктів [...] на окремі рядки об'єктів {...}
    static std::vector<std::string> getObjectArray(const std::string& json, const std::string& arrayKey) {
        std::vector<std::string> result;
        std::string search = "\"" + arrayKey + "\"";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return result;
        size_t arrayStart = json.find("[", pos);
        if (arrayStart == std::string::npos) return result;

        int braceCount = 0;
        size_t objStart = std::string::npos;
        for (size_t i = arrayStart + 1; i < json.size(); ++i) {
            if (json[i] == ']' && braceCount == 0) break;
            if (json[i] == '{') {
                if (braceCount == 0) objStart = i;
                braceCount++;
            } else if (json[i] == '}') {
                braceCount--;
                if (braceCount == 0 && objStart != std::string::npos) {
                    result.push_back(json.substr(objStart, i - objStart + 1));
                    objStart = std::string::npos;
                }
            }
        }
        return result;
    }

    // Отримання масиву чисел ("key":)
    static std::vector<int> getIntArray(const std::string& json, const std::string& arrayKey) {
        std::vector<int> result;
        std::string search = "\"" + arrayKey + "\"";
        size_t pos = json.find(search);
        if (pos == std::string::npos) return result;
        size_t start = json.find("[", pos);
        size_t end = json.find("]", start);
        if (start == std::string::npos || end == std::string::npos) return result;
        std::string content = json.substr(start + 1, end - start - 1);
        std::stringstream ss(content);
        std::string item;
        while (std::getline(ss, item, ',')) {
            if (!item.empty()) result.push_back(std::stoi(item));
        }
        return result;
    }
};