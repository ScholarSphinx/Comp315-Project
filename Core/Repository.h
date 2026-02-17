#pragma once
#include <vector>
#include <memory>
#include <algorithm>
//template class - C
template<typename T>
class Repository
{
protected:
	std::vector<std::shared_ptr<T>> items; ///Using shared_ptr to manage the lifetime of the items in the repository, allowing for polymorphism and safe memory management

public:
	void add(std::shared_ptr<T> item) //Adding a new item to the repository, using shared_ptr to manage the memory of the item and allow for polymorphism
    {
		items.push_back(item); //Adding a new item to the repository, using push_back to add the shared pointer to the vector
    }

    const std::vector<std::shared_ptr<T>>& getAll() const
    {
		return items; //Returning a const reference to the vector of items, allowing read-only access to the items in the repository without copying the vector
    }

	template<typename Predicate> //Filtering items in the repository based on a provided predicate function, returning a vector of shared pointers to the items that satisfy the predicate
	std::vector<std::shared_ptr<T>> filter(Predicate pred) const //Using a template parameter for the predicate allows for flexibility in the type of function or functor that can be used to filter the items, enabling users to define their own filtering criteria without being limited to a specific function signature
    {
		std::vector<std::shared_ptr<T>> result; //Creating a vector to hold the filtered results, which will contain shared pointers to the items that satisfy the predicate

        for (const auto& item : items)
        {
            if (pred(item))
				result.push_back(item); //If the item satisfies the predicate, it is added to the result vector, allowing for dynamic filtering of the items in the repository based on user-defined criteria
        }

        return result;
    }

	template<typename Comparator> //Sorting items in the repository based on a provided comparator function, which defines the sorting criteria, allowing users to sort the items in various ways without being limited to a specific sorting logic
    void sortBy(Comparator comp)
    {
		std::sort(items.begin(), items.end(), comp); //Using std::sort to sort the items in the repository based on the provided comparator, which allows for efficient sorting of the items according to user-defined criteria, enabling flexible sorting options for the items in the repository
    }
};
