#pragma once

#include <optional>
#include <vector>

template <typename T>
class LinearProbingHashSet
{
    size_t _size;
    std::hash<T> _hasher;
    std::vector<std::optional<T>> _items;

    size_t getHash(const std::vector<std::optional<T>>& items, const T& item) const
    {
        return _hasher(item) % items.size();
    }

    bool insert(std::vector<std::optional<T>>& items, const T& item)
    {
        const size_t hashIndex = getHash(items, item);
        size_t i = hashIndex;
        do
        {
            auto& curr = items.at(i);
            if (curr == std::nullopt)
            {
                curr = item;
                return true;
            }
            else if (curr == item)
            {
                return false;
            }
            i = (i + 1) % items.size();
        } while (i != hashIndex);
        return false;
    }

public:
    LinearProbingHashSet(const size_t capacity) : _size(0), _items(capacity) {}

    bool insert(const T& item)
    {
        if (insert(_items, item))
        {
            ++_size;
            return true;
        }
        return false;
    }

    bool remove(const T& item)
    {
        const size_t hashIndex = getHash(_items, item);
        size_t i = hashIndex;
        do
        {
            auto& curr = _items.at(i);
            if (curr == item)
            {
                curr = std::nullopt;
                --_size;
                return true;
            }
            i = (i + 1) % _items.size();
        } while (i != hashIndex);
        return false;
    }

    bool contains(const T& item) const
    {
        const size_t hashIndex = getHash(_items, item);
        size_t i = hashIndex;
        do
        {
            auto& curr = _items.at(i);
            if (curr == item)
            {
                return true;
            }
            i = (i + 1) % _items.size();
        } while (i != hashIndex);
        return false;
    }

    size_t size() const { return _size; }

    void resize(const size_t capacity)
    {
        std::vector<std::optional<T>> newItems(capacity);
        _size = 0;
        for (size_t i = 0; i < _items.size(); ++i)
        {
            auto& item = _items.at(i);
            if (item.has_value())
            {
                if (insert(newItems, item.value()))
                {
                    ++_size;
                }
            }
        }
        _items = std::move(newItems);
    }
};
