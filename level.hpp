#pragma once

#include "order.hpp"
#include "helper.hpp"

/*////////////////////////////////////////////////
Level: Represents a single price level in an
orderbook.
////////////////////////////////////////////////*/
template <int Price, typename OrderQueue>
struct Level
{
    static constexpr int price = Price;
    using orderQueue = OrderQueue;
};

template<int Price>
using EmptyLevel = Level<Price, Queue<>>;


/*////////////////////////////////////////////////
AddOrderToLevel: Adds a new order to a level.
This will add the order to the end of the queue.
////////////////////////////////////////////////*/
template <typename Level, typename Order>
struct AddOrderToLevel;

template <int Price, typename Order, typename OrderQueue>
struct AddOrderToLevel<Level<Price, OrderQueue>, Order>
{
    using _newQueue = QueuePush_t<OrderQueue, Order>;
    using type = Level<Price, _newQueue>;
};

template<typename Level, typename Order>
using AddOrderToLevel_t = typename AddOrderToLevel<Level, Order>::type;


/*////////////////////////////////////////////////
RemoveOrderFromQueue: Loops through the queue of
orders and remove the order matching the orderId.

This is not a regular operation as in a regular
queue. It can remove the element from any arbitrary
position in the queue.
////////////////////////////////////////////////*/
template<int OrderId, typename Queue>
struct RemoveOrderFromQueue;

template<int OrderId>
struct RemoveOrderFromQueue<OrderId, Queue<>>
{
    using type = Queue<>;
};

template<int OrderId, typename O1, typename... O2toN>
struct RemoveOrderFromQueue<OrderId, Queue<O1, O2toN...>>
{
    using remainingType = typename RemoveOrderFromQueue<OrderId, Queue<O2toN...>>::type; // recursive call
    using type = std::conditional_t<O1::orderId == OrderId,
                                    remainingType,
                                    QueueMerge_t<Queue<O1>, remainingType>
                                   >;
};

template<int OrderId, typename Queue>
using RemoveOrderFromQueue_t = typename RemoveOrderFromQueue<OrderId, Queue>::type;


/*////////////////////////////////////////////////
RemoveOrderFromLevel: Removes an order from a price
level.
////////////////////////////////////////////////*/
template<int OrderId, typename Level>
struct RemoveOrderFromLevel;

template<int OrderId, int Price, typename OrderQueue>
struct  RemoveOrderFromLevel<OrderId, Level<Price, OrderQueue>>
{
    using type = Level<Price, RemoveOrderFromQueue_t<OrderId, OrderQueue>>;
};

template<int OrderId, typename Level>
using RemoveOrderFromLevel_t = typename RemoveOrderFromLevel<OrderId, Level>::type;
