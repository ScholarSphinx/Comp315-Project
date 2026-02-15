#pragma once

#include "../Core/Inventory.h"
#include "../Core/Order.h"
#include "../IO/Logger.h"

#include <vector>
#include <thread>
#include <atomic>
#include <random>

struct Metrics
{
    std::atomic<int> total{ 0 };
    std::atomic<int> success{ 0 };
    std::atomic<int> failed{ 0 };
};

class OrderProcessor
{
private:
    Inventory& m_inventory;
    Metrics m_metrics;

public:
    explicit OrderProcessor(Inventory& inventory);

    void simulate(int threadCount, int totalOrders);

    const Metrics& getMetrics() const;
};
