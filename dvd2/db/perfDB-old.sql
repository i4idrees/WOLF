CREATE DATABASE IF NOT EXISTS perf_results;
USE perf_results;

CREATE TABLE workload_cache (
    id INT AUTO_INCREMENT PRIMARY KEY,
    combination VARCHAR(255) NOT NULL,  -- e.g. "1,2,5,3,0,0,4,6,..."
    avg_response_time FLOAT NOT NULL,
    error_rate FLOAT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY unique_combo (combination)
);

CREATE TABLE IF NOT EXISTS blacklist_combinations (
    combination VARCHAR(512) NOT NULL
);