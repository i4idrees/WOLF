CREATE DATABASE IF NOT EXISTS perf_results;
USE perf_results;

-- Simplified workload_cache table (no id, no created_at)
CREATE TABLE IF NOT EXISTS workload_cache (
    combination VARCHAR(255) NOT NULL,  -- e.g. "1,2,5,3,0,0,4,6,..."
    avg_response_time FLOAT NOT NULL,
    error_rate FLOAT,
    UNIQUE KEY unique_combo (combination)
);

CREATE TABLE IF NOT EXISTS blacklist_combinations (
    combination VARCHAR(512) NOT NULL,
    UNIQUE KEY unique_blacklist_combo (combination)
);