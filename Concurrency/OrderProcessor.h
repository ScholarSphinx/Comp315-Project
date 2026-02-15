#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include "../Core/Inventory.h"
#include "../IO/Logger.h"

struct Metrics
{
    std::atomic<int> total{ 0 };
    std::atomic<int> success{ 0 };
    std::atomic<int> failed{ 0 };
};

class OrderProcessor
{
public:
    OrderProcessor(Inventory& inventory);

    void simulate(int threads, int totalOrders);
    Metrics& getMetrics();

private:
    void worker(int ordersPerThread);

    Inventory& m_inventory;
    Metrics m_metrics;
    std::atomic<int> m_orderId{ 0 };
};
