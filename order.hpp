#pragma once

#include <bits/stdc++.h>

enum Side
{
    BUY,
    SELL
};

template <int OrderId, int Side, int Price, int Qty>
struct Order
{
    static constexpr int orderId = OrderId;
    static constexpr int side = Side;
    static constexpr int price = Price;
    static constexpr int qty = Qty;
};





