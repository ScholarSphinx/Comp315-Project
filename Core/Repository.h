#pragma once
#include <vector>
#include <memory>
#include <algorithm>
//template class - C
template<typename T>
class Repository
{
protected:
    std::vector<std::shared_ptr<T>> items;

public:
    void add(std::shared_ptr<T> item)
    {
        items.push_back(item);
    }

    const std::vector<std::shared_ptr<T>>& getAll() const
    {
        return items;
    }

    template<typename Predicate>
    std::vector<std::shared_ptr<T>> filter(Predicate pred) const
    {
        std::vector<std::shared_ptr<T>> result;

        for (const auto& item : items)
        {
            if (pred(item))
                result.push_back(item);
        }

        return result;
    }

    template<typename Comparator>
    void sortBy(Comparator comp)
    {
        std::sort(items.begin(), items.end(), comp);
    }
};
