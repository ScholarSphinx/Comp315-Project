#include "OrderProcessor.h"
#include "../Utils/RandomGenerator.h"

OrderProcessor::OrderProcessor(Inventory& inventory)
    : m_inventory(inventory) {
}

Metrics& OrderProcessor::getMetrics()
{
    return m_metrics;
}

void OrderProcessor::simulate(int threads, int totalOrders)
{
    if (threads <= 0 || totalOrders <= 0)
        return;

    std::vector<std::thread> pool;
    int perThread = totalOrders / threads;

    for (int i = 0; i < threads; ++i)
        pool.emplace_back(&OrderProcessor::worker,
            this, perThread);

    for (auto& t : pool)
        if (t.joinable()) t.join();
}

void OrderProcessor::worker(int ordersPerThread)
{
    for (int i = 0; i < ordersPerThread; ++i)
    {
        int id = ++m_orderId;
        int productId = RandomGenerator::getInt(1, 15);
        int qty = RandomGenerator::getInt(1, 5);

        Order order(id, productId, qty);

        bool success = m_inventory.processOrder(order);

        m_metrics.total++;
        success ? m_metrics.success++ : m_metrics.failed++;

        Logger::log("Order " + std::to_string(id) +
            (success ? " SUCCESS" : " FAILED"));
    }
}
