#include <catch2/catch_all.hpp>
#include "Order.h"

SCENARIO("Orders can be filled ")
{
    GIVEN("GoodTillCancel")
    {
        OrderId id = 123;
        Price price = 100;
        Quantity qty = 50;
        Order order{ OrderType::GoodTillCancel, id, Side::Buy, price, qty };

        THEN("The initial state is correctly set") {
            REQUIRE(order.GetOrderId() == id);
            REQUIRE(order.GetPrice() == price);
            REQUIRE(order.GetRemainingQuantity() == qty);
            REQUIRE(order.GetFilledQuantity() == 0);
            REQUIRE_FALSE(order.IsFilled());
        }
        WHEN("An overfill is attempted") {
            THEN("A logic_error is thrown") {
                REQUIRE_THROWS_AS(order.Fill(51), std::logic_error);
            }
        }
    }
    
}