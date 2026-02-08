#pragma once

#include "helper.hpp"
#include "order.hpp"
#include "level.hpp"



/*////////////////////////////////////////////////
Merge: Merges two sorted lists. Its a helper
utility for merge sort.
////////////////////////////////////////////////*/
template<typename List1, typename List2, bool Ascending>
struct Merge;

// Base case 1
template<typename List1, bool Ascending>
struct Merge<List1, List<>, Ascending>
{
    using type = List1;
};

// Base case 2
template<typename List2, bool Ascending>
struct Merge<List<>, List2, Ascending>
{
    using type = List2;
};

// Base case 3
// No ambiguous template instatiation error. This is strictly more
// specialized because it fixes 2 parameters whereas previous ones
// fixes only one.
template<bool Ascending>
struct Merge<List<>, List<>, Ascending>
{
    using type = List<>;
};

template<typename X1, typename... X2toN, typename Y1, typename... Y2toN, bool Ascending>
struct Merge<List<X1, X2toN...>, List<Y1, Y2toN...>, Ascending>
{
    using type = std::conditional_t<Ascending,
                                    std::conditional_t<X1::price <= Y1::price,
                                                       ListMerge_t<List<X1>,
                                                                   typename Merge<List<X2toN...>, List<Y1, Y2toN...>, Ascending>::type
                                                                  >,
                                                       ListMerge_t<List<Y1>,
                                                                   typename Merge<List<X1, X2toN...>, List<Y2toN...>, Ascending>::type
                                                                  >
                                                      >,
                                    std::conditional_t<X1::price >= Y1::price,
                                                       ListMerge_t<List<X1>,
                                                                   typename Merge<List<X2toN...>, List<Y1, Y2toN...>, Ascending>::type
                                                                  >,
                                                       ListMerge_t<List<Y1>,
                                                                   typename Merge<List<X1, X2toN...>, List<Y2toN...>, Ascending>::type
                                                                  >
                                                      >
                                   >;
};

template<typename List1, typename List2, bool Ascending>
using Merge_t = typename Merge<List1, List2, Ascending>::type;


/*////////////////////////////////////////////////
MergeSortListOfLevels: Sorts a lost of levels
using the merge sort algorithm.
////////////////////////////////////////////////*/
template<typename LevelsList, bool Ascending>
struct MergeSortListOfLevels;

template<bool Ascending>
struct MergeSortListOfLevels<List<>, Ascending>
{
    using type = List<>;
};

// Need this base case due to limitation of SubList metafunction.
// It can't handle empty sublist.
template<typename L1, bool Ascending>
struct MergeSortListOfLevels<List<L1>, Ascending>
{
    using type = List<L1>;
};

template<typename L1, typename... L2toN, bool Ascending>
struct MergeSortListOfLevels<List<L1, L2toN...>, Ascending>
{
    using L = List<L1, L2toN...>;
    static constexpr int sz = L::size;

    using leftHalf = SubList_t<L, 0, sz / 2>;
    using rightHalf = SubList_t<L, sz / 2, sz - sz / 2>;

    using sortedLeft = typename MergeSortListOfLevels<leftHalf, Ascending>::type;
    using sortedRight = typename MergeSortListOfLevels<rightHalf, Ascending>::type;

    using type = Merge_t<sortedLeft, sortedRight, Ascending>;
};

template<typename LevelsList, bool Ascending>
using MergeSortListOfLevels_t = typename MergeSortListOfLevels<LevelsList, Ascending>::type;


/*////////////////////////////////////////////////
AddOrderToListOfLevels: Sorts a lost of levels
using the merge sort algorithm.
////////////////////////////////////////////////*/
template<typename LevelsList, typename Order>
struct AddOrderToListOfLevels;

template<typename Order, typename... Levels>
struct AddOrderToListOfLevels<List<Levels...>, Order>
{
    using type = List<AddOrderToLevel_t<EmptyLevel<Order::price>, Order>>;
};

template<typename Order, typename L1, typename... L2toN>
struct AddOrderToListOfLevels<List<L1, L2toN...>, Order>
{
    static constexpr bool match = (L1::price == Order::price);

    using type = std::conditional_t<
                    match,
                    ListMerge_t<List<AddOrderToLevel_t<L1, Order>>,
                                List<L2toN...>
                               >,
                    ListMerge_t<List<L1>,
                                typename AddOrderToListOfLevels<List<L2toN...>, Order>::type
                               >
                    >;
};

template<typename LevelsList, typename Order>
using AddOrderToListOfLevels_t = typename AddOrderToListOfLevels<LevelsList, Order>::type;


/*////////////////////////////////////////////////
RemoveEmptyLevels: Remove empty levels i.e. levels
that don't have any order from list of levels.
////////////////////////////////////////////////*/
template<typename LevelsList>
struct RemoveEmptyLevels;

template<>
struct RemoveEmptyLevels<List<>>
{
    using type = List<>;
};

template<typename L1, typename... L2toN>
struct RemoveEmptyLevels<List<L1, L2toN...>>
{
    static constexpr int levelSize = L1::orderQueue::size;
    using remaining = typename RemoveEmptyLevels<List<L2toN...>>::type;
    using type = std::conditional_t<levelSize == 0,
                                    remaining,
                                    ListMerge_t<List<L1>, remaining>
                                   >;
};

template<typename LevelsList>
using RemoveEmptyLevels_t = typename RemoveEmptyLevels<LevelsList>::type;


/*////////////////////////////////////////////////
RemoveOrderFromListOfLevels: Removes all orders
in the list of levels matching the `OrderId`.
////////////////////////////////////////////////*/
template<int OrderId, typename LevelsList>
struct RemoveOrderFromListOfLevels;

template<int OrderId>
struct RemoveOrderFromListOfLevels<OrderId, List<>>
{
    using type = List<>;
};

template<int OrderId, typename L1, typename... L2toN>
struct RemoveOrderFromListOfLevels<OrderId, List<L1, L2toN...>>
{
    using remainingType = typename RemoveOrderFromListOfLevels<OrderId, List<L2toN...>>::type;
    using removedType = ListMerge_t<List<RemoveOrderFromLevel_t<OrderId, L1>>, remainingType>;
    using type = RemoveEmptyLevels_t<removedType>;
};

template<int OrderId, typename LevelsList>
using RemoveOrderFromListOfLevels_t = typename RemoveOrderFromListOfLevels<OrderId, LevelsList>::type;

