#pragma once

#include "helper.hpp"
#include "order.hpp"
#include "level.hpp"
#include "listOfLevels.hpp"


/*////////////////////////////////////////////////
Bids: Represents bids in an orderbook.
The price levels in bids are soted in decreasing
order.
////////////////////////////////////////////////*/
template <typename LevelsList>
struct Bids
{
    using levels = LevelsList;
};


/*////////////////////////////////////////////////
AddOrderToBids: Adds an ordeer to bids. It maintains
sorted order of the levels based on price priority.
////////////////////////////////////////////////*/
template <typename Bids, typename Order>
struct AddOrderToBids
{
    using list = AddOrderToListOfLevels_t<typename Bids::levels, Order>;
    using sortedList = MergeSortListOfLevels_t<list, false>;

    using type = ::Bids<sortedList>;
};

template <typename Bids, typename Order>
using AddOrderToBids_t = typename AddOrderToBids<Bids, Order>::type;


/*////////////////////////////////////////////////
RemoveOrderFromBids: Removes an order from the bids.
////////////////////////////////////////////////*/
template<typename Bids, int OrderId>
struct RemoveOrderFromBids
{
    using type = ::Bids<RemoveOrderFromListOfLevels_t<OrderId, typename Bids::levels>>;
};

template<typename Bids, int OrderId>
using RemoveOrderFromBids_t = typename RemoveOrderFromBids<Bids, OrderId>::type;



/*////////////////////////////////////////////////
Asks: Represents asks in an orderbook.
The price levels in asks are sorted in increasing
order.
////////////////////////////////////////////////*/
template <typename LevelsList>
struct Asks
{
    using levels = LevelsList;
};


/*////////////////////////////////////////////////
AddOrderToAsk: Adds an ordeer to asks. It maintains
sorted order of the levels based on price priority.
////////////////////////////////////////////////*/
template <typename Asks, typename Order>
struct AddOrderToAsks
{
    using list = AddOrderToListOfLevels_t<typename Asks::levels, Order>;
    using sortedList = MergeSortListOfLevels_t<list, true>;

    using type = ::Asks<sortedList>;
};

template <typename Asks, typename Order>
using AddOrderToAsks_t = typename AddOrderToAsks<Asks, Order>::type;


/*////////////////////////////////////////////////
RemoveOrderFromAsks: Removes an order from the askss.
////////////////////////////////////////////////*/
template<typename Asks, int OrderId>
struct RemoveOrderFromAsks
{
    using type = ::Asks<RemoveOrderFromListOfLevels_t<OrderId, typename Asks::levels>>;
};


template<typename Asks, int OrderId>
using RemoveOrderFromAsks_t = typename RemoveOrderFromAsks<Asks, OrderId>::type;







/*////////////////////////////////////////////////
Orderbook: A price-time priority limit order book.
////////////////////////////////////////////////*/
template<typename Bids = Bids<List<>>, typename Asks = Asks<List<>>>
struct OrderBook
{
    using bids = Bids;
    using asks = Asks;
};


/*////////////////////////////////////////////////
AddOrder: Adds order to the order book.
////////////////////////////////////////////////*/
template<typename Orderbook, typename Order>
struct AddOrder
{
    using b = std::conditional_t<Order::side == BUY,
                                 AddOrderToBids_t<typename Orderbook::bids, Order>,
                                 typename Orderbook::bids
                                >;
    using a = std::conditional_t<Order::side == SELL,
                                 AddOrderToAsks_t<typename Orderbook::asks, Order>,
                                 typename Orderbook::asks
                                >;
    using type = OrderBook<b, a>;
};

template<typename Orderbook, typename Order>
using AddOrder_t = typename AddOrder<Orderbook, Order>::type;


/*////////////////////////////////////////////////
CancelOrder: Removes an order from the order book.
////////////////////////////////////////////////*/
template<typename Orderbook, int OrderId>
struct CancelOrder
{
    // Need to check both bids and asks because we don't know the order
    // is on which side.
    using b = RemoveOrderFromBids_t<typename Orderbook::bids, OrderId>;
    using a = RemoveOrderFromAsks_t<typename Orderbook::asks, OrderId>;
    using type = OrderBook<b, a>;
};

template<typename Orderbook, int OrderId>
using CancelOrder_t = typename CancelOrder<Orderbook, OrderId>::type;


/*////////////////////////////////////////////////
ModifyOrder: Modifies an existing order in the book.
If the order was not there, previously, then this
behaves exactly like placing a new order.
////////////////////////////////////////////////*/
template<typename Orderbook, typename Order>
struct ModifyOrder
{
    using book = CancelOrder_t<Orderbook, Order::orderId>;
    using type = AddOrder_t<book, Order>;
};

template<typename Orderbook, typename Order>
using ModifyOrder_t = typename ModifyOrder<Orderbook, Order>::type;

