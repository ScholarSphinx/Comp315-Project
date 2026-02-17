#include "OrderProcessor.h"
#include <algorithm>
#include "../Config.h"


OrderProcessor::OrderProcessor(Inventory& inventory)
    : m_inventory(inventory)
{
}

void OrderProcessor::simulate()
{
    const auto& productMap = m_inventory.getProducts();
    if (productMap.empty())
        return;

    if (!CUMULATIVE_ORDERS)
    {
        m_metrics.total.store(0);
        m_metrics.success.store(0);
        m_metrics.failed.store(0);
    }

    std::vector<std::shared_ptr<Product>> products;
    products.reserve(productMap.size());

    for (const auto& [id, product] : productMap)
        products.push_back(product);

    int actualThreads =
        std::min(THREAD_COUNT, MAX_CONCURRENT_THREADS);

    int ordersPerThread =
        TOTAL_ORDERS / actualThreads;

    //Worker Lambda
    auto worker = [this, &products, ordersPerThread]()
        {
            std::random_device rd;
            std::mt19937 gen(rd());

            std::uniform_int_distribution<> productDist(
                0, products.size() - 1);

            std::uniform_int_distribution<> qtyDist(
                MIN_ORDER_QUANTITY,
                MAX_ORDER_QUANTITY);

            for (int i = 0; i < ordersPerThread; ++i)
            {
                m_metrics.total.fetch_add(1);

                auto product = products[productDist(gen)];

                double revenue = 0.0;

                OrderStatus status =
                    m_inventory.processOrder(
                        product->getId(),
                        qtyDist(gen),
                        revenue
                    );

                if (status == OrderStatus::Success)
                    m_metrics.success.fetch_add(1);
                else
                    m_metrics.failed.fetch_add(1);
            }
        };

    std::vector<std::thread> threads;
    threads.reserve(actualThreads);

    for (int i = 0; i < actualThreads; ++i)
        threads.emplace_back(worker);

    for (auto& t : threads)
        if (t.joinable())
            t.join();
}



const Metrics& OrderProcessor::getMetrics() const
{
    return m_metrics;
}
