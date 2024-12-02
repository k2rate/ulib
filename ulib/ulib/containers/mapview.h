#pragma once
#include <iterator>
#include <tuple>
#include <type_traits>

#include <ulib/typetraits/missingtype.h>
#include <ulib/typetraits/typeordefault.h>

#include "iterators/baseiterator.h"

namespace ulib
{
    template <class SpanT, class Pred, class... Args>
    class FilterView;

    template <class SpanT, class Pred, class... Args>
    class MapView
    {
    public:
        using SelfT = MapView<SpanT, Pred, Args...>;

        using underlying_span_type = SpanT;
        using predicate_type = std::decay_t<Pred>;

        using underlying_value_type = typename underlying_span_type::value_type;
        using underlying_iterator = typename underlying_span_type::iterator;

        using value_type = std::invoke_result_t<predicate_type, underlying_value_type, Args...>;

    private:
        struct Iterator : public BaseIterator<std::remove_cv_t<value_type>, std::random_access_iterator_tag>
        {
            inline Iterator() {}
            inline Iterator(underlying_iterator iter, const SelfT *view) : iter(iter), view(view) {}

            static auto TupleIterHelper(underlying_value_type &obj) { return std::make_tuple(std::ref(obj)); }

            static auto TupleIterHelper(const underlying_value_type &obj) { return std::make_tuple(std::cref(obj)); }

            inline value_type operator*() const { return std::apply(view->mPred, std::tuple_cat(TupleIterHelper(*iter), view->mArgs)); }

            inline Iterator &operator++() { return ++iter, *this; }
            inline Iterator &operator++(int) { return iter++, *this; }
            inline Iterator &operator--() { return --iter, *this; }
            inline Iterator &operator--(int) { return iter--, *this; }

            inline Iterator operator+(size_t k) const { return Iterator{iter + k, view}; }
            inline Iterator operator-(size_t k) const { return Iterator{iter - k, view}; }
            inline Iterator operator+=(size_t k) { return Iterator{iter += k, view}; }
            inline Iterator operator-=(size_t k) { return Iterator{iter -= k, view}; }

            inline bool operator>(Iterator k) const { return iter > k.iter; }
            inline bool operator<(Iterator k) const { return iter < k.iter; }
            inline bool operator<=(Iterator k) const { return iter <= k.iter; }
            inline bool operator>=(Iterator k) const { return iter >= k.iter; }
            inline bool operator==(Iterator k) const { return iter == k.iter; }
            inline bool operator!=(Iterator k) const { return iter != k.iter; }
            inline size_t operator-(Iterator k) const { return iter - k.iter; }

            /*
                        inline bool operator>(Iterator k) const { return iter > k.iter; }
                        inline bool operator<(Iterator k) const { return iter < k.iter; }
                        inline bool operator<=(Iterator k) const { return iter <= k.iter; }
                        inline bool operator>=(Iterator k) const { return iter >= k.iter; }
                        inline bool operator==(Iterator k) const { return iter == k.iter; }
                        inline bool operator!=(Iterator k) const { return iter != k.iter; }
                        inline size_t operator-(Iterator k) const { return iter - k.iter; }
            */

            // inline T *operator->() const { return ptr; }

            // inline T *Raw() { return ptr; }
            // inline T *raw() { return ptr; }

            underlying_iterator iter;
            const SelfT *view;
        };

    public:
        using iterator = Iterator;

        SpanT &underlying_span() { return mSpan; }
        const SpanT &underlying_span() const { return mSpan; }

        MapView(underlying_span_type span, predicate_type &&pred, Args... args)
            : mSpan{span}, mPred{std::move(pred)}, mArgs{std::make_tuple<std::decay_t<Args>...>(args...)}
        {
        }

        MapView(underlying_span_type span, const predicate_type &pred, Args... args)
            : mSpan{span}, mPred{pred}, mArgs{std::make_tuple<std::decay_t<Args>...>(args...)}
        {
        }

        iterator begin() const { return iterator{std::begin(mSpan), this}; }
        iterator end() const { return iterator{std::end(mSpan), this}; }

        value_type operator[](size_t index) const { return *(begin() + index); }

        /// Map Operations

        template <class NewPred, class... NewArgs>
        auto map(NewPred &&pred, NewArgs &&...args) const
        {
            return MapView<SelfT, NewPred, NewArgs...>{*this, std::forward<NewPred>(pred), std::forward<NewArgs>(args)...};
        }

        template <class... FuncArgs, class RetVal, class VT = value_type, class RetContValT = ulib::type_if_t<RetVal, !std::is_same_v<RetVal, void>>>
        auto map(RetVal (VT::*fn)(FuncArgs &&...), FuncArgs &&...args) const
        {
            auto pred = [fn, ... args = std::forward<FuncArgs>(args)](VT &instance) { return ((instance).*fn)(args...); };
            return MapView<SelfT, decltype(pred)>{*this, std::move(pred)};
        }

        template <class... FuncArgs, class RetVal, class VT = value_type, class RetContValT = ulib::type_if_t<RetVal, !std::is_same_v<RetVal, void>>>
        auto map(RetVal (VT::*fn)(FuncArgs &&...) const, FuncArgs &&...args) const
        {
            auto pred = [fn, ... args = std::forward<FuncArgs>(args)](const VT &instance) { return ((instance).*fn)(args...); };
            return MapView<SelfT, decltype(pred)>{*this, std::move(pred)};
        }

        /// Filter Operations

        template <class NewPred, class... NewArgs>
        auto filter(NewPred &&pred, NewArgs &&...args) const
        {
            return FilterView<SelfT, NewPred, NewArgs...>{*this, std::forward<NewPred>(pred), std::forward<NewArgs>(args)...};
        }

        template <class... FuncArgs, class RetVal, class VT = value_type, class RetContValT = ulib::type_if_t<RetVal, !std::is_same_v<RetVal, void>>>
        auto filter(RetVal (VT::*fn)(FuncArgs &&...), FuncArgs &&...args) const
        {
            auto pred = [fn, ... args = std::forward<FuncArgs>(args)](VT &instance) { return ((instance).*fn)(args...); };
            return FilterView<SelfT, decltype(pred)>{*this, std::move(pred)};
        }

        template <class... FuncArgs, class RetVal, class VT = value_type, class RetContValT = ulib::type_if_t<RetVal, !std::is_same_v<RetVal, void>>>
        auto filter(RetVal (VT::*fn)(FuncArgs &&...) const, FuncArgs &&...args) const
        {
            auto pred = [fn, ... args = std::forward<FuncArgs>(args)](const VT &instance) { return ((instance).*fn)(args...); };
            return FilterView<SelfT, decltype(pred)>{*this, std::move(pred)};
        }

    private:
        underlying_span_type mSpan;
        predicate_type mPred;
        std::tuple<std::decay_t<Args>...> mArgs;
    };
} // namespace ulib
