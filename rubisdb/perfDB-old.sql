CREATE DATABASE IF NOT EXISTS perf_results;
USE perf_results;

CREATE TABLE workload_cache (
    id INT AUTO_INCREMENT PRIMARY KEY,
    t1 INT NOT NULL,
    t2 INT NOT NULL,
    t3 INT NOT NULL,
    t4 INT NOT NULL,
    t5 INT NOT NULL,
    t6 INT NOT NULL,
    t7 INT NOT NULL,
    t8 INT NOT NULL,
    t9 INT NOT NULL,
    t10 INT NOT NULL,
    t11 INT NOT NULL,
    t12 INT NOT NULL,
    t13 INT NOT NULL,
    t14 INT NOT NULL,
    t15 INT NOT NULL,
    avg_response_time FLOAT NOT NULL,
    error_rate FLOAT,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    UNIQUE KEY unique_combo (
        t1, t2, t3, t4, t5, t6, t7, t8, t9, t10,
        t11, t12, t13, t14, t15
    )
);
