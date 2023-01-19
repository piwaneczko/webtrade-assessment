#ifndef IG_ORDERCHACHE_H
#define IG_ORDERCHACHE_H
// Additional include guard for some compilators, that doesn't support pragma once
#pragma once

#include <string>
#include <vector>

class Order {
   public:
    // do not alter signature of this constructor
    Order(const std::string& ordId, const std::string& secId, const std::string& side, const unsigned int qty,
          const std::string& user, const std::string& company)
        : m_orderId(ordId), m_securityId(secId), m_side(side), m_qty(qty), m_user(user), m_company(company) {}

    // do not alter these accessor methods
    std::string orderId() const {
        return m_orderId;
    }
    std::string securityId() const {
        return m_securityId;
    }
    std::string side() const {
        return m_side;
    }
    std::string user() const {
        return m_user;
    }
    std::string company() const {
        return m_company;
    }
    unsigned int qty() const {
        return m_qty;
    }

   private:
    // use the below to hold the order data
    // do not remove the these member variables
    std::string m_orderId;     // unique order id
    std::string m_securityId;  // security identifier
    std::string m_side;        // side of the order, eg Buy or Sell
    unsigned int m_qty;        // qty for this order
    std::string m_user;        // user name who owns this order
    std::string m_company;     // company for user
};

// Provide an implementation for the OrderCacheInterface interface class.
// Your implementation class should hold all relevant data structures you think
// are needed.
class OrderCacheInterface { /*<! I'm would use struct here ;) */
   public:
    // implememnt the 6 methods below, do not alter signatures

    // add order to the cache
    virtual void addOrder(Order order) = 0;

    // remove order with this unique order id from the cache
    virtual void cancelOrder(const std::string& orderId) = 0;

    // remove all orders in the cache for this user
    virtual void cancelOrdersForUser(const std::string& user) = 0;

    // remove all orders in the cache for this security with qty >= minQty
    virtual void cancelOrdersForSecIdWithMinimumQty(const std::string& securityId, unsigned int minQty) = 0;

    // return the total qty that can match for the security id
    virtual unsigned int getMatchingSizeForSecurity(const std::string& securityId) = 0;

    // return all orders in cache in a vector
    virtual std::vector<Order> getAllOrders() const = 0;
};

using std::string;
using std::vector;

class OrderCache : public OrderCacheInterface {
   private:
    vector<Order> allOrders_;

   public:
    // add order to the cache
    void addOrder(Order order) override {}

    // remove order with this unique order id from the cache
    void cancelOrder(const string& orderId) override {}

    // remove all orders in the cache for this user
    void cancelOrdersForUser(const string& user) override {}

    // remove all orders in the cache for this security with qty >= minQty
    void cancelOrdersForSecIdWithMinimumQty(const string& securityId, unsigned int minQty) override {}

    // return the total qty that can match for the security id
    unsigned int getMatchingSizeForSecurity(const string& securityId) override {
        return allOrders_.size();
    }

    // return all orders in cache in a vector
    vector<Order> getAllOrders() const {
        return allOrders_;
    }
};

#endif  // IG_ORDERCHACHE_H