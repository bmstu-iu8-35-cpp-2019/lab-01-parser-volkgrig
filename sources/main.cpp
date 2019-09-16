// Copyright 2019 Volkov
#include "../include/nlohmann/json.hpp"
#include <iostream>
#include <string>

using json = nlohmann::json;
/*
[
    ["Si-9.15", "RTS-9.15", "GAZP-9.15"],
    [100024, 100027, 100050],
    ["Futures contract for USD/RUB", "Futures contract for index RTS", "Futures contract for Gazprom shares"]
]
*/
/*
[
    {"ticker":"Si-9.15", "id": 100024, "description": "Futures contract for USD/RUB"},
    {"ticker":"RTS-9.15", "id": 100027, "description": "Futures contract for index RTS"},
    {"ticker":"GAZP-9.15", "id": 100050, "description": "Futures contract for GAZPROM shares"}
]
*/
int main() {
    std::string j1 = "[[\"Si-9.15\", \"RTS-9.15\", \"GAZP-9.15\"], ";
    std::string j2 = "[100024, 100027, 100050],";
    std::string j3 = "[\"Futures contract for USD / RUB\", \"Futures contract";
    std::string j4 = " for index RTS\", \"Futures contract for Gazprom shares\"]]";
    std::string jjson = j1 + j2 + j3 + j4;
    json j = json::parse(jjson);
    size_t n = (j[0]).size();
    json j_new;
    for (size_t i = 0; i < n; i++) {
        j_new[i]["ticker"] = j[0][i];
        j_new[i]["id"] = j[1][i];
        j_new[i]["description"] = j[2][i];
    }

    if (j_new[0]["id"] != 100024) { return 1; }
    if (j_new[1]["ticker"] != "RTS-9.15") { return 2; }
    if (j_new[2]["description"] !=
        "Futures contract for Gazprom shares") { return 3; }
    //std::cout << j_new.dump();
    return 0;
}