#pragma once

// --- Core Simulation Settings ---
constexpr int THREAD_COUNT = 5;
constexpr int TOTAL_ORDERS = 50;

// --- Behaviour Settings ---
constexpr bool CUMULATIVE_ORDERS = true; //set to false for each simulation to reset metrics

// --- Order Quantity Range ---
constexpr int MIN_ORDER_QUANTITY = 1;
constexpr int MAX_ORDER_QUANTITY = 5;

// --- Concurrency Limiter ---
constexpr int MAX_CONCURRENT_THREADS = 8;

//LOW_STOCK_WARNING_THRESHOLD = 5 ???
//ENABLE_REVENUE_TRACKING = true ???