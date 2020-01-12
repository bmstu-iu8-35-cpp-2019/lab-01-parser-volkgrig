// Copyright 2019 Volkov

#ifndef INCLUDE_JSON_HPP_
#define INCLUDE_JSON_HPP_

#include <string>
#include <any>
#include <map>
#include <vector>
#include <stdexcept>
#include "iostream"
#include<stdio.h>
#include<stdlib.h>
#include <fstream>
#include <cstdlib>

enum state {find_value, find_colon, find_comma_or_end, find_key_or_end};

class Json {
private:
    std::any _data;

public:
    explicit Json(const std::string& s) {
        int n = s.length();
        int position = 0;
        while (position < n) {
            if (s[position] == ' ' || s[position] == '\n'
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


    std::map<std::string, std::any> parse_object(
        const std::string& s, int& position) {
        int n = s.length();
        std::map<std::string, std::any> object;
        auto state = find_key_or_end;
        std::string key = "";
        while (position < n) {
            while (position < n) {
                if ((s[position] == ' ') ||
                    (s[position] == '\n') || (s[position] == '\t')) {
                    position++;
                } else {
                    break;
                }
            }
            if (s[position] == ',') {
                if (state == find_comma_or_end) {
                    position++;
                    state = find_key_or_end;
                    continue;
                } else {
                    throw std::invalid_argument(" , found; state not 'comma_or_end'");
                }
            }
            if (s[position] == '"') {
                if (state == find_key_or_end) {
                    position++;
                    key = parse_string(s, position);
                    state = find_colon;
                } else {
                    if (state == find_value) {
                        position++;
                        std::string value = parse_string(s, position);
                        object.insert(make_pair(key, value));
                        state = find_comma_or_end;
                    } else {
                        throw std::invalid_argument(
                            "double apostrophe found; state not key_or_end or find_value");
                    }
                }
            }
            if (isdigit(s[position])) {
                if (state == find_value) {
                    float value = parse_number(s, position);
                    object.insert(make_pair(key, value));
                    state = find_comma_or_end;
                } else {
                    throw std::invalid_argument("number found; state not find_value");
                }
            }

            if (isalpha(s[position])) {
                if (state == find_value) {
                    bool value = parse_bool(s, position);
                    object.insert(make_pair(key, value));
                    state = find_comma_or_end;
                } else {
                    throw std::invalid_argument("alpha found; state not find_value");
                }
            }
            if (s[position] == ':') {
                if (state == find_colon) {
                    position++;
                    state = find_value;
                } else {
                    throw std::invalid_argument(": found; state not find_colon");
                }
            }
            if (s[position] == '}') {
                if ((state == find_key_or_end)
                    || (state == find_comma_or_end)) {
                    position++;
                    return object;
                } else {
                    throw std::invalid_argument("} found; state not find_key_or_end");
                }
            }
            if (s[position] == '{') {
                if (state == find_value) {
                    position++;
                    std::map<std::string, std::any> value =
                        parse_object(s, position);
                    object.insert(make_pair(key, value));
                    state = find_comma_or_end;
                } else {
                    throw std::invalid_argument("} found; state not find_value");
                }
            }
            if (s[position] == '[') {
                if (state == find_value) {
                    position++;
                    std::vector<std::any> value = parse_array(s, position);
                    object.insert(make_pair(key, value));
                    state = find_comma_or_end;
                } else {
                    throw std::invalid_argument("[ found; state not find_value");
                }
            }
         }
         throw std::invalid_argument("position number > n");;
    }


    float parse_number(const std::string& s, int& position) {
        int n = s.length();
        std::string str = "";
        while (position < n) {
            while (position < n) {
                if (s[position] == ' ' || s[position] == '\n'
                    || s[position] == '\t') {
                    position++;
                } else {
                    break;
                }
            }
            if ((isdigit(s[position]) == false) && (s[position] != '.')) {
                if (str == "") {
                    throw std::invalid_argument("false value of number");
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
                if (s[position] == ' ' || s[position] == '\n'
                    || s[position] == '\t') {
                    position++;
                } else {
                    break;
                }
            }
            if (isalpha(s[position]) == false) {
                if (str == "") {
                    throw std::invalid_argument("false value of alpha");
                }
                break;
            }
            str = str + s[position];
            position++;

            if (str.length() == 5 && str == "false") {
                return false; }
            if (str.length() == 4 && str == "true") {
                return true; }
            if ((str.length() == 4) && (str != "fals")) {
                throw std::invalid_argument("bad bool value"); }
            if (str.length() == 5 && str != "false") {
                throw std::invalid_argument("bad bool value"); }
        }
        return ::atof(str.c_str());
    }


    std::string parse_string(const std::string& s,
        int& position) {
        int n = s.length();
        std::string str = "";
        while (position < n) {
            while (position < n) {
                if (s[position] == ' ' || s[position] == '\n'
                    || s[position] == '\t') {
                    position++;
                } else {
                    break;
                }
            }
            if (s[position] == '"') {
                if (str == "") {
                    throw std::invalid_argument("empty string");
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
        auto state = find_value;
        while (position < n) {
            while (position < n) {
                if ((s[position] == ' ') || (s[position] == '\n')
                    || (s[position] == '\t')) {
                    position++;
                } else {
                    break;
                }
            }
            if (s[position] == '"') {
                if (state == find_value) {
                    position++;
                }
            }
            if (s[position] == ',') {
                if (state == find_comma_or_end) {
                    state = find_value;
                    position++;
                }else{
                    throw std::invalid_argument(" , found; state not 'comma_or_end'");
                }
            }
            if (isdigit(s[position])) {
                if (state == find_value) {
                    float value = parse_number(s, position);
                    arr.push_back(value);
                    state = find_comma_or_end;
                } else {
                    throw std::invalid_argument("number found; state not find_value");
                }
            }

            if (isalpha(s[position])) {
                if (state == find_value) {
                    bool value = parse_bool(s, position);
                    arr.push_back(value);
                    state = find_comma_or_end;
                } else {
                    throw std::invalid_argument("alpha found; state not find_value");
                }
            }
            if (s[position] == '{') {
                if (state == find_value) {
                    position++;
                    arr.push_back(parse_object(s, position));
                } else {
                    throw std::invalid_argument("{ found; state not find_value");
                }
            }
            if (s[position] == ']') {
                    position++;
                    return arr;
            }
        }
        throw std::invalid_argument("position number > n");
    }


    Json parse(const std::string& s) {
        int n = s.length();
        int position = 0;
        while (position < n) {
            if (s[position] == ' ' || s[position] == '\n'
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
