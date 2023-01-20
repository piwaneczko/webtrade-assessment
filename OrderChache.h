#ifndef IG_ORDERCHACHE_H
#define IG_ORDERCHACHE_H
// Additional include guard for some compilators, that doesn't support pragma once
#pragma once

#include <algorithm>
#include <iostream>
#include <list>
#include <mutex>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::find;
using std::find_if;
using std::for_each;
using std::list;
using std::lock_guard;
using std::min;
using std::mutex;
using std::next;
using std::pair;
using std::partition;
using std::remove_if;
using std::string;
using std::vector;

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
class OrderCacheInterface {
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

/**
 * @brief Order cache implementation
 *
 */
class OrderCache : public OrderCacheInterface {
   private:
    mutable mutex ordersLocker_; /*!< Multithreaded sync object */
    vector<Order> orders_;       /*!< Orders collection         */

   public:
    /**
     * @brief Add order to the cache
     *
     * @param order Order copy
     */
    void addOrder(Order order) override {
        lock_guard<mutex> ordersLock(ordersLocker_);
        orders_.emplace_back(order);
    }

    /**
     * @brief Remove order with unique order id from the cache
     *
     * @param orderId Unique order Id to remove
     */
    void cancelOrder(const string& orderId) override {
        lock_guard<mutex> ordersLock(ordersLocker_);
        orders_.erase(remove_if(orders_.begin(), orders_.end(),
                                [&orderId](const Order& order) { return order.orderId() == orderId; }),
                      orders_.end());
    }

    /**
     * @brief Remove all orders in the cache for user
     *
     * @param user User name for removing from cache
     */
    void cancelOrdersForUser(const string& user) override {
        lock_guard<mutex> ordersLock(ordersLocker_);
        orders_.erase(
            remove_if(orders_.begin(), orders_.end(), [&user](const Order& order) { return order.user() == user; }),
            orders_.end());
    }

    /**
     * @brief Remove all orders in the cache for this security with qty >= minQty
     *
     * @param securityId Secuirity Id of removed order
     * @param minQty Minimum quantity value of removed order
     */
    void cancelOrdersForSecIdWithMinimumQty(const string& securityId, unsigned int minQty) override {
        lock_guard<mutex> ordersLock(ordersLocker_);
        orders_.erase(remove_if(orders_.begin(), orders_.end(),
                                [&securityId, minQty](const Order& order) {
                                    return order.securityId() == securityId && order.qty() >= minQty;
                                }),
                      orders_.end());
    }

    /**
     * @brief Get the total qty that can match for the security id
     *
     * @param securityId Matching security id
     * @note Order matching occurs for orders with the same security id, different side (buy or sell), and different
     * company (company of person who requested the order)
     * - Can only match orders with the same security id
     * - Can only match a Buy order with a Sell order
     * - Buy order can match against multiple Sell orders (and vice versa)
     * - Any order quantity already allocated to a match cannot be reused as a match against a differnt order
     * - Some orders may not match entirely or at all
     * - Users in the same company cannot match against each other
     * @return unsigned int Calculated quantity matching for secId
     */
    unsigned int getMatchingSizeForSecurity(const string& securityId) override {
        using OrderIt = vector<Order>::iterator;
        uint32_t bought = 0;
        uint32_t sold = 0;
        list<OrderIt> boughtOrders;
        list<OrderIt> soldOrders;

        // Searching for all trace matches between companies with same securityId
        lock_guard<mutex> ordersLock(ordersLocker_);

        // move all (sort) orders with securityId to front to speed up the performance
        auto nextSecurityPartition = partition(orders_.begin(), orders_.end(), [&securityId](const Order& order) {
            return order.securityId() == securityId;
        });

        for (auto it1 = orders_.begin(); it1 < nextSecurityPartition; it1++) {
            for (auto it2 = next(it1); it2 < nextSecurityPartition; it2++) {
                // Search for multiple orders matching securityId for different side and company
                it2 = find_if(it2, nextSecurityPartition, [&it1](const auto& order) {
                    return order.company() != it1->company() && order.side() != it1->side();
                    // comparing securityId is unnecessary and redundant when searching in partition
                    // range <it2:nextSecurityPartition)
                });
                // If match has been found sum all bought orders
                if (it2 < nextSecurityPartition) {
                    auto sellOrder = it1->side() == "Sell" ? it1 : it2;
                    auto buyOrder = it2->side() == "Buy" ? it2 : it1;
                    if (find(boughtOrders.begin(), boughtOrders.end(), sellOrder) == boughtOrders.end()) {
                        // Quantity can be summed only once
                        bought += sellOrder->qty();
                        boughtOrders.emplace_back(sellOrder);
                    }
                    // store sold orders for future calculation
                    if (find(soldOrders.begin(), soldOrders.end(), buyOrder) == soldOrders.end())
                        soldOrders.emplace_back(buyOrder);
                }
            }
        };

        // Summarize all sold orders
        for (const auto& it : soldOrders) {
            // Matched sold security ids from different company cannot be higher from bought amount
            auto qty = min(it->qty(), bought);
            sold += qty;
            bought -= qty;
        }

        return sold;
    }

    /**
     * @brief Get the all orders in cache in a vector
     *
     * @return all orders in cache
     */
    vector<Order> getAllOrders() const {
        lock_guard<mutex> ordersLock(ordersLocker_);
        return orders_;
    }
};

#endif  // IG_ORDERCHACHE_H
