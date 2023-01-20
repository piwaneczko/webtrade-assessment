#include <gmock/gmock.h>

#include "OrderChache.h"

using testing::Test;

class OrderTests : public Test {
   public:
    OrderCache orderCache0, orderCache1, orderCache2, orderCache3;
    void SetUp() override {
        orderCache0.addOrder({"1", "ABCD", "Sell", 10000, "u1", "c1"});
        orderCache0.addOrder({"2", "ABCD", "Buy", 2000, "u2", "c2"});
        orderCache0.addOrder({"3", "ABCD", "Buy", 1000, "u3", "c2"});

        orderCache1.addOrder({"OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"});
        orderCache1.addOrder({"OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB"});
        orderCache1.addOrder({"OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"});
        orderCache1.addOrder({"OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"});
        orderCache1.addOrder({"OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"});
        orderCache1.addOrder({"OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"});
        orderCache1.addOrder({"OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE"});
        orderCache1.addOrder({"OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE"});

        orderCache2.addOrder({"OrdId1", "SecId1", "Sell", 100, "User10", "Company2"});
        orderCache2.addOrder({"OrdId2", "SecId3", "Sell", 200, "User8", "Company2"});
        orderCache2.addOrder({"OrdId3", "SecId1", "Buy", 300, "User13", "Company2"});
        orderCache2.addOrder({"OrdId4", "SecId2", "Sell", 400, "User12", "Company2"});
        orderCache2.addOrder({"OrdId5", "SecId3", "Sell", 500, "User7", "Company2"});
        orderCache2.addOrder({"OrdId6", "SecId3", "Buy", 600, "User3", "Company1"});
        orderCache2.addOrder({"OrdId7", "SecId1", "Sell", 700, "User10", "Company2"});
        orderCache2.addOrder({"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
        orderCache2.addOrder({"OrdId9", "SecId2", "Buy", 900, "User6", "Company2"});  // typo Cuompany2
        orderCache2.addOrder({"OrdId10", "SecId2", "Sell", 1000, "User5", "Company1"});
        orderCache2.addOrder({"OrdId11", "SecId1", "Sell", 1100, "User13", "Company2"});
        orderCache2.addOrder({"OrdId12", "SecId2", "Buy", 1200, "User9", "Company2"});
        orderCache2.addOrder({"OrdId13", "SecId1", "Sell", 1300, "User1", "Company1"});

        orderCache3.addOrder({"OrdId1", "SecId3", "Sell", 100, "User1", "Company1"});
        orderCache3.addOrder({"OrdId2", "SecId3", "Sell", 200, "User3", "Company2"});
        orderCache3.addOrder({"OrdId3", "SecId1", "Buy", 300, "User2", "Company1"});
        orderCache3.addOrder({"OrdId4", "SecId3", "Sell", 400, "User5", "Company2"});
        orderCache3.addOrder({"OrdId5", "SecId2", "Sell", 500, "User2", "Company1"});
        orderCache3.addOrder({"OrdId6", "SecId2", "Buy", 600, "User3", "Company2"});
        orderCache3.addOrder({"OrdId7", "SecId2", "Sell", 700, "User1", "Company1"});
        orderCache3.addOrder({"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
        orderCache3.addOrder({"OrdId9", "SecId1", "Buy", 900, "User5", "Company2"});
        orderCache3.addOrder({"OrdId10", "SecId1", "Sell", 1000, "User1", "Company1"});
        orderCache3.addOrder({"OrdId11", "SecId2", "Sell", 1100, "User6", "Company"});
    }
};

TEST_F(OrderTests, CancelOrderTest) {
    orderCache0.cancelOrder("2");
    EXPECT_EQ(orderCache0.getAllOrders().size(), 2u);
    EXPECT_EQ(orderCache0.getMatchingSizeForSecurity("ABCD"), 1000u);
}

TEST_F(OrderTests, CancelOrdersForUserTest) {
    orderCache0.cancelOrdersForUser("u3");

    EXPECT_EQ(orderCache0.getAllOrders().size(), 2u);
    EXPECT_EQ(orderCache0.getMatchingSizeForSecurity("ABCD"), 2000u);
}

TEST_F(OrderTests, CancelOrdersForSecIdWithMinimumQtyTest) {
    orderCache0.cancelOrdersForSecIdWithMinimumQty("ABCD", 2000);

    EXPECT_EQ(orderCache0.getAllOrders().size(), 1u);
    EXPECT_EQ(orderCache0.getMatchingSizeForSecurity("ABCD"), 0u);
}

TEST_F(OrderTests, OrderMachingRulesTest) {
    EXPECT_EQ(orderCache0.getAllOrders().size(), 3u);
    EXPECT_EQ(orderCache0.getMatchingSizeForSecurity("ABCD"), 3000u);
}

TEST_F(OrderTests, OrderMachingTest) {
    EXPECT_EQ(orderCache1.getAllOrders().size(), 8u);
    EXPECT_EQ(orderCache1.getMatchingSizeForSecurity("SecId1"), 0u);
    EXPECT_EQ(orderCache1.getMatchingSizeForSecurity("SecId2"), 2700u);
    EXPECT_EQ(orderCache1.getMatchingSizeForSecurity("SecId3"), 0u);
}

TEST_F(OrderTests, MoreExamplesTest1) {
    EXPECT_EQ(orderCache2.getAllOrders().size(), 13u);
    EXPECT_EQ(orderCache2.getMatchingSizeForSecurity("SecId1"), 300u);
    EXPECT_EQ(orderCache2.getMatchingSizeForSecurity("SecId2"), 1000u);
    EXPECT_EQ(orderCache2.getMatchingSizeForSecurity("SecId3"), 600u);
}

TEST_F(OrderTests, MoreExamplesCancelOrdersForTwoUsersTest) {
    orderCache2.cancelOrdersForUser("User10");
    orderCache2.cancelOrdersForUser("User13");

    EXPECT_EQ(orderCache2.getAllOrders().size(), 9u);
    EXPECT_EQ(orderCache2.getMatchingSizeForSecurity("SecId1"), 0);
    EXPECT_EQ(orderCache2.getMatchingSizeForSecurity("SecId2"), 1000u);
    EXPECT_EQ(orderCache2.getMatchingSizeForSecurity("SecId3"), 600u);
}

TEST_F(OrderTests, MoreExamplesTest2) {
    EXPECT_EQ(orderCache3.getAllOrders().size(), 11u);
    EXPECT_EQ(orderCache3.getMatchingSizeForSecurity("SecId1"), 900u);
    EXPECT_EQ(orderCache3.getMatchingSizeForSecurity("SecId2"), 600u);
    EXPECT_EQ(orderCache3.getMatchingSizeForSecurity("SecId3"), 0u);
}

TEST_F(OrderTests, PerformanceTest) {
    for (size_t i = 0; i < 100000; i++) {
        orderCache3.getMatchingSizeForSecurity("SecId1");
        orderCache3.getMatchingSizeForSecurity("SecId2");
        orderCache3.getMatchingSizeForSecurity("SecId3");
    }
}