#include "helper.hpp"
#include "order.hpp"
#include "level.hpp"
#include "listOfLevels.hpp"
#include "orderbook.hpp"

using namespace std;

const std::string RED = "\033[31m";
const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string BLUE = "\033[34m";
const std::string RESET = "\033[0m";


void print_queue(const Queue<>&)
{
}

template<typename T1, typename...T2toN>
void print_queue(const Queue<T1, T2toN...>&)
{
    print(T1());
    cout << ", ";
    print_queue(Queue<T2toN...>());
}

template <int OrderId,  int Side, int Price, int Qty>
void print(const Order<OrderId, Side, Price, Qty>& o)
{
    cout << '(' << OrderId << ' ' << Side << ' ' << Price << ' ' << Qty << ')';
}

template <int Price, typename OrderQueue>
void print(const Level<Price, OrderQueue>& l)
{
    cout << Price << ": ";
    print_queue(OrderQueue());
}

void print_list(const List<>&)
{
}

template<typename L1, typename... L2toN>
void print_list(const List<L1, L2toN...>&)
{
    print(L1());
    cout << endl;
    print_list(List<L2toN...>());
}



template<typename List>
void print_bids(const Bids<List>&)
{
    cout << "Bids: \n";
    print_list(List());
}

template<typename List>
void print_asks(const Asks<List>&)
{
    cout << "Asks: \n";
    print_list(List());
}

template<typename Bids, typename Asks>
void print_book(const OrderBook<Bids, Asks>&, const std::string& bookName)
{
    cout << RED << bookName << ": \n" << RESET;
    print_bids(Bids());
    print_asks(Asks());
}


int main()
{

    using o1 = Order<1, BUY, 100, 5>;
    using o2 = Order<2, BUY, 99, 10>;
    using o3 = Order<3, BUY, 101, 10>;
    using o4 = Order<4, BUY, 99, 10>;


    using o5 = Order<5, SELL, 102, 10>;
    using o6 = Order<6, SELL, 101, 5>;

    // cout << o1::orderId << endl;

    using l1 = List<>;
    using l2 = AddOrderToListOfLevels_t<l1, o1>;
    using l3 = AddOrderToListOfLevels_t<l2, o2>;

    using b1 = Bids<List<>>;
    using b2 = AddOrderToBids_t<b1, o1>;
    using b3 = AddOrderToBids_t<b2, o2>;
    using b4 = AddOrderToBids_t<b3, o3>;
    using b5 = AddOrderToBids_t<b4, o4>;

    using a1 = Asks<List<>>;
    using a2 = AddOrderToAsks_t<a1, o5>;
    using a3 = AddOrderToAsks_t<a2, o6>;

    // cout << "l1\n";
    // print_list(l1());
    // cout << "l2\n";
    // print_list(l2());
    // cout << "l3\n";
    // print_list(l3());

    // print_bids(b1());
    // print_bids(b2());
    // print_bids(b3());
    // print_bids(b4());
    // print_bids(b5());
    // print_asks(a3());

    using b6 = RemoveOrderFromBids_t<b5, 2>;
    using b7 = RemoveOrderFromBids_t<b6, 2>;
    // print_bids(b6());
    // print_bids(b7());

    using a4 = RemoveOrderFromAsks_t<a3, 5>;
    using a5 = RemoveOrderFromAsks_t<a4, 6>;
    // print_asks(a4());
    // print_asks(a5());

    using book0 = OrderBook<>;
    using book1 = AddOrder_t<book0, o1>;
    using book2 = AddOrder_t<book1, o5>;
    using book3 = AddOrder_t<book2, o6>;
    using book4 = AddOrder_t<book3, o4>;
    using book5 = AddOrder_t<book4, o3>;
    using book6 = AddOrder_t<book5, o2>;

    using book7 = ModifyOrder_t<book6, Order<4, BUY, 100, 10>>;
    using book8 = ModifyOrder_t<book7, Order<4, SELL, 100, 10>>;
    using book9 = ModifyOrder_t<book8, Order<6, SELL, 102, 100>>;

    using book10 = CancelOrder_t<book9, o2::orderId>;
    using book11 = CancelOrder_t<book10, o4::orderId>;
    using book12 = CancelOrder_t<book11, o6::orderId>;


    print_book(book0(), "book0");
    print_book(book1(), "book1");
    print_book(book2(), "book2");
    print_book(book3(), "book3");
    print_book(book4(), "book4");
    print_book(book5(), "book5");
    print_book(book6(), "book6");
    print_book(book7(), "book7");
    print_book(book8(), "book8");
    print_book(book9(), "book9");
    print_book(book10(), "book10");
    print_book(book11(), "book11");
    print_book(book12(), "book12");


    return 0;
}


