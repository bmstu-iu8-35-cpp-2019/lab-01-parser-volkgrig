// Copyright 2019 Volkov

#include <gtest/gtest.h>
#include "../include/json.hpp"
/*
TEST(Json, Operato_EQ) {
    std::string j1 = "{ \"lastname\" : \"Ivanov\", \"firstname\" : \"Ivan\", ";
    std::string j2 = "\"age\" : 25, \"islegal\" : false,";
    std::string j3 = "\"address\" : { \"city\" : \"Moscow\", \"street\" :";
    std::string j4 = "\"Vozdvijenka\"}, \"ad\":";
    std::string j5 = "[1,2,5]}";
    std::string jjson = j1 + j2 + j3 + j4 + j5;

    Json object = Json(jjson);
    EXPECT_EQ(object.is_object(), true);
    EXPECT_EQ(static_cast<std::string>("Ivanov"),
        std::any_cast<std::string>(object["lastname"]));

    EXPECT_EQ(std::any_cast<bool>(object["islegal"]), false);
    EXPECT_EQ(std::any_cast<float>(object["age"]), 25);

    auto address = Json(object["address"]);
    EXPECT_EQ(std::any_cast<std::string>(address["city"]), "Moscow");
    EXPECT_EQ(std::any_cast<std::string>(address["street"]), "Vozdvijenka");

    auto a = object["ad"];
    EXPECT_EQ(static_cast<float>(1),
        std::any_cast<float>((std::any_cast<std::vector<std::any>>(a))[0]));
    EXPECT_EQ(static_cast<float>(5),
        std::any_cast<float>((std::any_cast<std::vector<std::any>>(a))[2]));
}
*/
