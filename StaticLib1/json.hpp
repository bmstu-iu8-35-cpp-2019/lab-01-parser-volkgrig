// Copyright 2019 Volkov

#ifndef INCLUDE_JSON_HPP_
#define INCLUDE_JSON_HPP_

#include "string"
#include <any>
#include <map>
#include <vector>
#include <stdexcept>
#include "iostream"
#include<stdio.h> 
#include<stdlib.h>
#include <fstream>
#include <cstdlib>

class Json {
private:
    std::any _data;

public:
    explicit Json(const std::string& s) {
        _data = parse(s);
    }


    explicit Json(std::any s) {
        try {
            _data = std::any_cast<std::map<std::string, std::any>>(s);
        }
        catch (...) {
            throw std::logic_error("is not an object");
        }
    }


    bool is_array() const {
        if (_data.type() == typeid(std::vector<std::any>)) {
            return true;
        } else {
            return false;
        }
    }

    bool is_object() const {
        if (_data.type() == typeid(std::map<std::string, std::any>)) {
            return true;
        } else {
            return false;
        }
    }


    std::any operator[](const std::string& key) {
        if (is_object()) {
            return (std::any_cast<std::map<std::string, std::any>>(_data))[key];
        } else {
            throw std::logic_error("is not an object");
        }
    }


    std::any operator[](int index) {
        if (is_array()) {
            auto pam = std::any_cast<std::vector<std::any>>(_data);
            return pam[index];
        } else {
            throw std::logic_error("is not an array");
        }
    }


    std::map<std::string, std::any> parse_object(const std::string& s, int& position) {
        int n = s.length();
        std::map<std::string, std::any> object;
        std::string state = "find_key_or_end";
        std::string key = "";
        while (position < n) {
            while (position < n) {
                if ((s[position] == ' ') || 
                    (s[position] == '/n') || (s[position] == '\t')) {
                    position++;
                } else {
                    break;
                }
            }
            if (s[position] == ',') {
                if (state == "find_comma_or_end") {
                    position++;
                    state = "find_key_or_end";
                    continue;
                } else {
                    throw std::invalid_argument("6666");
                }
            }
            if (s[position] == '"') {
                if (state == "find_key_or_end") {
                    position++;
                    key = parse_string(s, position);
                    state = "find_colon";
                }
                else {
                    if (state == "find_value") {
                        position++;
                        std::string value = parse_string(s, position);
                        object.insert(make_pair(key, value));
                        state = "find_comma_or_end";
                    } else {
                        throw std::invalid_argument("1111");
                    }
                }
            }
            if (isdigit(s[position])) {
                if (state == "find_value") {
                    float value = parse_number(s, position);
                    object.insert(make_pair(key, value));
                    state = "find_comma_or_end";
                } else {
                    throw std::invalid_argument("7777");
                }
            }

            if (isalpha(s[position])) {
                if (state == "find_value") {
                    bool value = parse_bool(s, position);
                    object.insert(make_pair(key, value));
                    state = "find_comma_or_end";
                } else {
                    throw std::invalid_argument("8888");
                }
            }
            if (s[position] == ':') {
                if (state == "find_colon") {
                    position++;
                    state = "find_value";
                } else {
                    throw std::invalid_argument("2222");
                }
            }
            if (s[position] == '}') {
                if ((state == "find_key_or_end") 
                    || (state == "find_comma_or_end")) {
                    position++;
                    return object;
                } else {
                    throw std::invalid_argument("3333");
                }
            }
            if (s[position] == '{') {
                if (state == "find_value") {
                    position++;
                    std::map<std::string, std::any> value = 
                        parse_object(s, position);
                    object.insert(make_pair(key, value));
                    state = "find_comma_or_end";
                } else {
                    throw std::invalid_argument("4444");
                }
            }
            if (s[position] == '[') {
                if (state == "find_value") {
                    position++;
                    std::vector<std::any> value = parse_array(s, position);
                    object.insert(make_pair(key, value));
                    state = "find_comma_or_end";
                } else {
                    throw std::invalid_argument("4444");
                }
            }
         }
         throw std::invalid_argument("18.18.18");;
    }


    float parse_number(const std::string& s, int& position) {
        int n = s.length();
        std::string str = "";
        while (position < n) {
            while (position < n) {
                if (s[position] == ' ' || s[position] == '/n' 
                    || s[position] == '\t') {
                    position++;
                } else {
                    break;
                }
            }
            if ((isdigit(s[position]) == false) && (s[position] != '.')) {
                if (str == "") {
                    throw std::invalid_argument("5555");
                }
                break;
            }
            str = str + s[position];
            position++;
        }
        return ::atof(str.c_str());
    }


    float parse_bool(const std::string& s, int& position) {
        int n = s.length();
        std::string str = "";
        while (position < n) {
            while (position < n) {
                if (s[position] == ' ' || s[position] == '/n' 
                    || s[position] == '\t') {
                    position++;
                } else {
                    break;
                }
            }
            if (isalpha(s[position]) == false) {
                if (str == "") {
                    throw std::invalid_argument("9999");
                }
                break;
            }
            str = str + s[position];
            position++;

            if (str.length() == 5 && str == "false") {
                return false; }
            if (str.length() == 4 && str == "true") {
                return true; }
            if (str.length() == 4 && str != "fals" ) {
                throw std::invalid_argument("11.11.11"); }
            if (str.length() == 5 && str != "false") {
                throw std::invalid_argument("10.10.10"); }
        }
        return ::atof(str.c_str());
    }


    std::string parse_string(const std::string& s,
        int& position) {
        int n = s.length();
        std::string str = "";
        while (position < n) {
            while (position < n) {
                if (s[position] == ' ' || s[position] == '/n' 
                    || s[position] == '\t') {
                    position++;
                } else {
                    break;
                }
            }
            if (s[position] == '"') {
                if (str == "") {
                    throw std::invalid_argument("5555");
                }
                position++;
                break;
            }
            str = str + s[position];
            position++;
        }
        return str;
    }


    std::vector<std::any> parse_array(const std::string& s,
        int& position) {
        int n = s.length();
        std::vector<std::any> arr;
        std::string state = "find_value";
        while (position < n) {
            while (position < n) {
                if ((s[position] == ' ') || (s[position] == '/n') 
                    || (s[position] == '\t')) {
                    position++;
                } else {
                    break;
                }
            }
            if (s[position] == '"') {
                if (state == "find_value") {
                    position++;
                }
            }
            if (s[position] == ',') {
                if (state == "find_comma_or_end") {
                    state = "find_value";
                    position++;
                }else{
                    throw std::invalid_argument("12.12.12");
                }
            }
            if (isdigit(s[position])) {
                if (state == "find_value") {
                    float value = parse_number(s, position);
                    arr.push_back(value);
                    state = "find_comma_or_end";
                } else {
                    throw std::invalid_argument("13.13.13");
                }
            }

            if (isalpha(s[position])) {
                if (state == "find_value") {
                    bool value = parse_bool(s, position);
                    arr.push_back(value);
                    state = "find_comma_or_end";
                } else {
                    throw std::invalid_argument("15.15.15");
                }
            }
            if (s[position] == '{') {
                if (state == "find_value") {
                    position++;
                    arr.push_back(parse_object(s, position));
                } else {
                    throw std::invalid_argument("14.14.14");
                }
            }
            if (s[position] == ']') {
                    position++;
                    return arr;
            }
        }
        throw std::invalid_argument("17.17.17");
    }


    Json parse(const std::string& s) {
        int n = s.length();
        int position = 0;
        while (position < n) {
            if (s[position] == ' ' || s[position] == '/n' 
                || s[position] == '\t') {
                position++;
                continue;
            }
            if (s[position] == '{') {
                position++;
                _data = parse_object(s, position);
                break;
            }
            if (s[position] == '[') {
                position++;
                _data = parse_array(s, position);
                auto pam = std::any_cast<std::vector<std::any>>(_data);
                break;
            }
        }
        return *this;
    }

    
    Json parseFile(char* path_to_file) {
        FILE *f;
        f = fopen(path_to_file, "r");
        std::ifstream fin(path_to_file);
        std::string s = "";
        fin >> s;
        fclose(f);
        return parse(s);
    }
};
#endif // INCLUDE_JSON_HPP_
