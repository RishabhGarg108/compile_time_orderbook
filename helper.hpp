#pragma once

/*////////////////////////////////////////////////
Queue: Generic queue that can hold heterogeneous
data types. It provides ability to push at the end
of queue and pop from the front.
////////////////////////////////////////////////*/
template <typename... T>
struct Queue
{
    static constexpr int size = sizeof...(T);
};


/*////////////////////////////////////////////////
QueuePush: Pushes an element at the end of the
queue.
////////////////////////////////////////////////*/
template <typename Q, typename T>
struct QueuePush;

template <typename T, typename... QElem>
struct QueuePush<Queue<QElem...>, T>
{
    using type = Queue<QElem..., T>;
};

template <typename Q, typename T>
using QueuePush_t = typename  QueuePush<Q, T>::type;


/*////////////////////////////////////////////////
QueuePop: Pops an element front the front of the
queue.
////////////////////////////////////////////////*/
template <typename Q>
struct QueuePop;

template <typename T0, typename... T1toN>
struct QueuePop<Queue<T0, T1toN...>>
{
    using type = Queue<T1toN...>;
};

template <typename Q>
using QueuePop_t = typename QueuePop<Q>::type;


/*////////////////////////////////////////////////
QueueMerge: Merges two queues by pushing elements
from the second queue to the end of first queue.
////////////////////////////////////////////////*/
template<typename Q1, typename Q2>
struct QueueMerge;

template<typename... Q1, typename... Q2>
struct QueueMerge<Queue<Q1...>, Queue<Q2...>>
{
    using type = Queue<Q1...,Q2...>;
};

template<typename Q1, typename Q2>
using QueueMerge_t = typename QueueMerge<Q1, Q2>::type;


/*////////////////////////////////////////////////
List: Generic list container that can hold elements
of any type.
////////////////////////////////////////////////*/
template <typename... T>
struct List
{
    static constexpr int size = sizeof...(T);
};


/*////////////////////////////////////////////////
ListMerge: Merges two lists by adding elements of
the second list to the end of first list.
////////////////////////////////////////////////*/
template <typename L1, typename L2>
struct ListMerge;

template <typename... Elem1, typename... Elem2>
struct ListMerge<List<Elem1...>, List<Elem2...>>
{
    using type = List<Elem1..., Elem2...>;
};

template <typename L1, typename L2>
using ListMerge_t = typename ListMerge<L1, L2>::type;


/*////////////////////////////////////////////////
SubList: Returns a sub-list of some fixed `size`
at some starting `offset` in the original list.

Note: The `offset` must be a valid 0-based index
in the list.

Note: If we pass both offset and size as 0, then the
code fails with ambiguous template instantiation
error. Fix it!!
////////////////////////////////////////////////*/
template<typename L, int Offset, int Size>
struct SubList;

template<typename... L, int Offset>
struct SubList<List<L...>, Offset, 0>
{
    using type = List<>;
};

template<typename L1, typename... L2toN>
struct SubList<List<L1, L2toN...>, 0, 1>
{
    using type = List<L1>;
};

template<typename L1, typename... L2toN, int Size>
struct SubList<List<L1, L2toN...>, 0, Size>
{
    using remaining = typename SubList<List<L2toN...>, 0, Size - 1>::type; // recursive call
    using type = ListMerge_t<List<L1>, remaining>;
};

template<typename L1, typename... L2toN, int Offset, int Size>
struct SubList<List<L1, L2toN...>, Offset, Size>
{
    // Unsafe!! Passing an invalid offset will give some illegible templates
    // error. There should be a check somewhere. Maybe a static assert?
    using type = typename SubList<List<L2toN...>, Offset - 1, Size>::type;
};

template<typename L, int Offset, int Size>
using  SubList_t = typename SubList<L, Offset, Size>::type;


