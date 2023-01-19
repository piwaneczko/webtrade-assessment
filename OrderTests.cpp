#include <gmock/gmock.h>

#include "OrderChache.h"

using testing::Test;

class OrderTests : public Test {
   public:
    OrderCache ordersCache;
    void SetUp() override {}
};

TEST_F(OrderTests, OrderMachingRulesExample) {
    ordersCache.addOrder({"1", "ABCD", "Buy", 10000, "u1", "c1"});
    ordersCache.addOrder({"2", "ABCD", "Sell", 2000, "u2", "c2"});
    ordersCache.addOrder({"3", "ABCD", "Sell", 1000, "u3", "c3"});

    EXPECT_EQ(ordersCache.getAllOrders().size(), 3u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("ABCD"), 3000u);
}

TEST_F(OrderTests, OrderMachingExample) {
    ordersCache.addOrder({"OrdId1", "SecId1", "Buy", 1000, "User1", "CompanyA"});
    ordersCache.addOrder({"OrdId2", "SecId2", "Sell", 3000, "User2", "CompanyB"});
    ordersCache.addOrder({"OrdId3", "SecId1", "Sell", 500, "User3", "CompanyA"});
    ordersCache.addOrder({"OrdId4", "SecId2", "Buy", 600, "User4", "CompanyC"});
    ordersCache.addOrder({"OrdId5", "SecId2", "Buy", 100, "User5", "CompanyB"});
    ordersCache.addOrder({"OrdId6", "SecId3", "Buy", 1000, "User6", "CompanyD"});
    ordersCache.addOrder({"OrdId7", "SecId2", "Buy", 2000, "User7", "CompanyE"});
    ordersCache.addOrder({"OrdId8", "SecId2", "Sell", 5000, "User8", "CompanyE"});

    EXPECT_EQ(ordersCache.getAllOrders().size(), 8u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId1"), 0u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId2"), 2700u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId3"), 0u);
}

TEST_F(OrderTests, MoreExamplesExample1) {
    ordersCache.addOrder({"OrdId1", "SecId1", "Sell", 100, "User10", "Company2"});
    ordersCache.addOrder({"OrdId2", "SecId3", "Sell", 200, "User8", "Company2"});
    ordersCache.addOrder({"OrdId3", "SecId1", "Buy", 300, "User13", "Company2"});
    ordersCache.addOrder({"OrdId4", "SecId2", "Sell", 400, "User12", "Company2"});
    ordersCache.addOrder({"OrdId5", "SecId3", "Sell", 500, "User7", "Company2"});
    ordersCache.addOrder({"OrdId6", "SecId3", "Buy", 600, "User3", "Company1"});
    ordersCache.addOrder({"OrdId7", "SecId1", "Sell", 700, "User10", "Company2"});
    ordersCache.addOrder({"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
    ordersCache.addOrder({"OrdId9", "SecId2", "Buy", 900, "User6", "Cuompany2"});
    ordersCache.addOrder({"OrdId10", "SecId2", "Sell", 1000, "User5", "Company1"});
    ordersCache.addOrder({"OrdId11", "SecId1", "Sell", 1100, "User13", "Company2"});
    ordersCache.addOrder({"OrdId12", "SecId2", "Buy", 1200, "User9", "Company2"});
    ordersCache.addOrder({"OrdId13", "SecId1", "Sell", 1300, "User1", "Company"});

    EXPECT_EQ(ordersCache.getAllOrders().size(), 13u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId1"), 300u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId2"), 1000u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId3"), 600u);
}

TEST_F(OrderTests, MoreExamplesExample2) {
    ordersCache.addOrder({"OrdId1", "SecId3", "Sell", 100, "User1", "Company1"});
    ordersCache.addOrder({"OrdId2", "SecId3", "Sell", 200, "User3", "Company2"});
    ordersCache.addOrder({"OrdId3", "SecId1", "Buy", 300, "User2", "Company1"});
    ordersCache.addOrder({"OrdId4", "SecId3", "Sell", 400, "User5", "Company2"});
    ordersCache.addOrder({"OrdId5", "SecId2", "Sell", 500, "User2", "Company1"});
    ordersCache.addOrder({"OrdId6", "SecId2", "Buy", 600, "User3", "Company2"});
    ordersCache.addOrder({"OrdId7", "SecId2", "Sell", 700, "User1", "Company1"});
    ordersCache.addOrder({"OrdId8", "SecId1", "Sell", 800, "User2", "Company1"});
    ordersCache.addOrder({"OrdId9", "SecId1", "Buy", 900, "User5", "Company2"});
    ordersCache.addOrder({"OrdId10", "SecId1", "Sell", 1000, "User1", "Company1"});
    ordersCache.addOrder({"OrdId11", "SecId2", "Sell", 1100, "User6", "Company2"});

    EXPECT_EQ(ordersCache.getAllOrders().size(), 11u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId1"), 900u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId2"), 600u);
    EXPECT_EQ(ordersCache.getMatchingSizeForSecurity("SecId3"), 0u);
}