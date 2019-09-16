// Copyright 2019 Volkov
#include "../include/json.hpp"
#include <iostream>
#include <string>

int main() {
    std::string k = "{\"100024\" : \"100027\", \"a\":2}";
    Json j = Json(k);
    if (std::any_cast<std::string>(j["100024"]) != "100027") { return 1; }
    return 0;
}
