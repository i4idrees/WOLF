# WOLF: Workload Optimization using Learning and Fitness

WOLF is an intelligent workload optimization framework that leverages Learning Classifier Systems (XCS) to automatically tune and optimize web application workloads. The system uses machine learning to discover optimal workload configurations that maximize throughput while maintaining acceptable response times and error rates.

## Table of Contents

- [Overview](#overview)
- [Key Features](#key-features)
- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Configuration](#configuration)
- [Usage](#usage)
- [System Under Test (SUT)](#system-under-test-sut)
- [Results and Analysis](#results-and-analysis)
- [Project Structure](#project-structure)
- [Algorithm Details](#algorithm-details)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)

## Overview

WOLF employs the Extended Classifier System (XCS), a reinforcement learning algorithm, to explore and exploit workload configurations for web applications. The system:

1. **Monitors** system performance metrics (average response time, error rate)
2. **Learns** optimal workload distributions across different transaction types
3. **Adapts** workload configurations to maximize throughput while respecting performance constraints
4. **Evolves** a population of classifiers that encode workload optimization rules

The framework currently supports two benchmark systems:
- **RUBiS**: Rice University Bidding System (auction site benchmark)
- **DVD Store 2 (DVD2)**: E-commerce benchmark

## Key Features

- **Intelligent Workload Optimization**: Uses XCS algorithm to learn optimal workload configurations
- **Multi-Objective Optimization**: Balances throughput maximization with response time and error rate constraints
- **Docker-Based Deployment**: Fully containerized setup for reproducible experiments
- **Flexible Configuration**: JSON-based configuration for experiments and system parameters
- **Multiple SUT Support**: Works with RUBiS and DVD Store 2 benchmarks
- **JMeter Integration**: Automated workload generation and testing using Apache JMeter
- **Real-Time Performance Monitoring**: Direct database queries for accurate performance metrics
- **Knowledge Persistence**: Saves and reuses learned classifiers across experiments

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                    WOLF Framework                        │
│  ┌───────────────────────────────────────────────────┐  │
│  │           XCS Learning Engine                     │  │
│  │  - Classifier Population Management               │  │
│  │  - Match Set Generation                           │  │
│  │  - Genetic Algorithm                              │  │
│  │  - Reinforcement Learning                         │  │
│  └───────────────────────────────────────────────────┘  │
│                         ↓ ↑                              │
│  ┌───────────────────────────────────────────────────┐  │
│  │      Environment & Performance Monitor           │  │
│  │  - JMeter Workload Generation                    │  │
│  │  - Database Performance Queries                  │  │
│  │  - Reward Calculation                            │  │
│  └───────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────┘
                          ↓ ↑
┌─────────────────────────────────────────────────────────┐
│              System Under Test (SUT)                     │
│  ┌──────────────┐              ┌──────────────┐         │
│  │  Web Server  │ ←──────────→ │   Database   │         │
│  │  (Apache)    │              │   (MySQL)    │         │
│  └──────────────┘              └──────────────┘         │
│       RUBiS or DVD2 Application                         │
└─────────────────────────────────────────────────────────┘
```

## Prerequisites

- **Docker**: Version 20.10 or higher
- **Docker Compose**: Version 2.x or higher
- **Operating System**: Linux (recommended) or macOS
- **RAM**: Minimum 8GB recommended
- **Disk Space**: At least 10GB free space

## Installation

### 1. Clone the Repository

```bash
git clone <repository-url>
cd WOLF
```

### 2. Build Docker Images

The project uses Docker Compose to orchestrate multiple containers. Build all required images:

```bash
docker-compose build
```

This will build:
- RUBiS web server and database containers
- DVD2 web server and database containers
- XCS development and production containers

### 3. Verify Installation

```bash
docker-compose ps
```

## Configuration

### Main Configuration File

The primary configuration is located at `XCS/config/config.json`. This JSON file defines experiment parameters:

```json
[
    {
        "SUT": "RUBIS",
        "transactions": ["HomePage", "RegisterPage", "RegisterUser", ...],
        "allPossibleActions": [2, 4, 5, 7, 8, 9],
        "workloadChangeBy": 1,
        "workloadChangeType": 1,
        "MaxEpochs": 1,
        "maxProblems": 100000,
        "Epsilon": 0.5,
        "ARTThreshhold": 50.0,
        "ERThreshhold": 20.0,
        "Testing": 0,
        "testFrequency": 1000,
        "repeatTimes": 1
    }
]
```

### Configuration Parameters

| Parameter | Description | Default |
|-----------|-------------|---------|
| `SUT` | System Under Test ("RUBIS" or "DVD") | "RUBIS" |
| `transactions` | List of transaction types to optimize | Varies by SUT |
| `allPossibleActions` | Workload adjustment actions (integers) | [2, 4, 5, 7, 8, 9] |
| `workloadChangeBy` | Workload increment/decrement step | 1 |
| `workloadChangeType` | Type of workload modification (1=increment) | 1 |
| `MaxEpochs` | Number of learning epochs | 1 |
| `maxProblems` | Maximum learning iterations per epoch | 100000 |
| `Epsilon` | Exploration vs exploitation rate (0.0-1.0) | 0.5 |
| `ARTThreshhold` | Average Response Time threshold (ms) | 50.0 |
| `ERThreshhold` | Error Rate threshold (%) | 20.0 |
| `testFrequency` | Write performance metrics every N iterations | 1000 |
| `repeatTimes` | Number of times to repeat the experiment | 1 |

### Transaction Configuration

Transactions are automatically appended with `_thread_group` suffix. For RUBiS:

- HomePage
- RegisterPage
- RegisterUser
- BrowsePage
- BrowseInCategories
- BrowseInRegions
- SellPage
- SellItem
- AboutMePage
- AboutMeUser
- BidOnItem
- BuyItem

### JMeter Test Plans

Test plans are located in:
- `XCS/config/RUBiS-testplan/testplan.jmx` - For RUBiS benchmark
- `XCS/config/DVD-testplan/testplan.jmx` - For DVD Store 2 benchmark

## Usage

### Running in Production Mode

To run a complete optimization experiment:

```bash
# Start the SUT (RUBiS or DVD2)
docker-compose up -d rubisweb rubisdb1

# Wait for services to initialize (about 30 seconds)
sleep 30

# Run the XCS optimizer in production mode
docker-compose --profile prod up xcs-prod
```

The system will:
1. Load configuration from `config.json`
2. Initialize the classifier population
3. Execute learning iterations (exploration and exploitation)
4. Generate workload using JMeter
5. Monitor performance metrics
6. Evolve the classifier population
7. Save results to `XCS/Results/`

### Running in Development Mode

For debugging and interactive development:

```bash
# Start the development container
docker-compose --profile dev up -d xcs-dev

# Attach to the container
docker exec -it xcs-dev /bin/bash

# Inside the container, compile and run
cd /app/src
g++ -g src/*.cpp -o xcs -I/usr/include/mariadb -ltinyxml2 -lmariadb
./xcs
```

### Switching Between Systems

Edit `XCS/config/config.json` and change the `SUT` parameter:

**For RUBiS:**
```json
{
    "SUT": "RUBIS",
    ...
}
```

**For DVD Store 2:**
```json
{
    "SUT": "DVD",
    ...
}
```

Then start the appropriate containers:

```bash
# For RUBiS
docker-compose up -d rubisweb rubisdb1

# For DVD2
docker-compose up -d dvd2web dvd2db
```

## System Under Test (SUT)

### RUBiS (Rice University Bidding System)

An auction site benchmark modeled after eBay. Supports 12 transaction types including browsing, bidding, and selling items.

**Services:**
- Web Server: `rubisweb` (Port 8080)
- Database: `rubisdb1` (Port 3306)

### DVD Store 2

An e-commerce benchmark simulating an online DVD store with customer registration, browsing, and purchasing.

**Services:**
- Web Server: `dvd2web` (Port 8080)
- Database: `dvd2db` (Port 3306)

### Database Reset Scripts

The system includes automatic database reset scripts:
- `XCS/src/reset-db-rubis.sh` - Resets RUBiS database
- `XCS/src/reset-db-dvd.sh` - Resets DVD Store database

## Results and Analysis

### Output Files

Results are stored in the `XCS/Results/` directory:

```
Results/
├── iterations_log.csv          # Per-iteration performance metrics
├── knowledge/                  # Saved classifier populations
│   └── epoch_1_classifiers.txt
└── performance_epoch_1.txt     # Aggregated performance data
```

### Iterations Log Format

The `iterations_log.csv` contains:
- Iteration number
- Solution number
- Episode number
- Episode step
- Elapsed time (ms)
- Workload configuration
- Average response time (ms)
- Error rate (%)
- Reward value
- Action taken
- Current observation state

### Backup and Archiving

Completed experiments are automatically backed up to:
```
Results_backup/SUT=<system>--ART=<threshold>---Population=<size>/
```

Example:
```
Results_backup/SUT=RUBIS--ART=50.000000---Population=2000/
```

## Project Structure

```
WOLF/
├── docker-compose.yml           # Docker orchestration
├── README.md                    # This file
│
├── rubisdb/                     # RUBiS database container
│   ├── Dockerfile
│   ├── rubisDB.sql
│   └── perfDB.sql
│
├── rubisweb/                    # RUBiS web server container
│   ├── Dockerfile
│   └── htdocs/                  # PHP application files
│
├── dvd2/                        # DVD Store 2 containers
│   ├── db/
│   │   ├── dockerfile
│   │   ├── DVD2DB.sql
│   │   └── perfDB.sql
│   └── web/
│       ├── dockerfile
│       └── htdocs/
│
└── XCS/                         # WOLF optimization engine
    ├── Dockerfile
    ├── bin/                     # Compiled binaries
    ├── config/                  # Configuration files
    │   ├── config.json          # Main configuration
    │   ├── RUBiS-testplan/      # JMeter test plan for RUBiS
    │   └── DVD-testplan/        # JMeter test plan for DVD2
    ├── src/                     # Source code
    │   ├── xcs.cpp              # Main XCS implementation
    │   ├── xcs.h
    │   ├── classifier.cpp       # Classifier operations
    │   ├── classifier.h
    │   ├── env.cpp              # Environment & performance monitoring
    │   ├── env.h
    │   ├── sut.cpp              # SUT interaction
    │   ├── configuration.h      # Configuration structures
    │   ├── xcsMacros.h          # XCS algorithm parameters
    │   ├── reset-db-rubis.sh    # Database reset scripts
    │   └── reset-db-dvd.sh
    └── Results/                 # Experimental results
        └── knowledge/           # Saved populations
```

## Algorithm Details

### XCS Learning Classifier System

WOLF uses the XCS (eXtended Classifier System) algorithm with the following characteristics:

#### Classifier Structure

Each classifier consists of:
- **Condition**: Code fragment-based representation of system states
- **Action**: Workload adjustment action
- **Prediction**: Expected reward
- **Prediction Error**: Accuracy measure
- **Fitness**: Selection probability
- **Experience**: Number of times used
- **Numerosity**: Number of identical classifiers

#### Learning Cycle

1. **Match Set Generation**: Find classifiers matching current state
2. **Prediction Array**: Calculate expected payoffs for each action
3. **Action Selection**: Choose action (explore vs exploit based on ε)
4. **Reward Calculation**: Execute action and measure system performance
5. **Parameter Update**: Update classifier predictions and fitness
6. **Genetic Algorithm**: Evolve new classifiers via crossover and mutation
7. **Subsumption**: Merge similar classifiers

#### Reward Function

The reward is calculated based on:
- **Response Time**: Lower is better
- **Error Rate**: Lower is better
- **Throughput**: Higher workload achieves higher rewards
- **Threshold Violations**: Penalties for exceeding ART/ER thresholds

$$
\text{Reward} = f(\text{Workload}, \text{ART}, \text{ER}, \text{Thresholds})
$$

#### Key Parameters (in `xcsMacros.h`)

- **maxPopSize**: 2000 (maximum population size)
- **condLength**: Dynamic (number of transactions + 1)
- **learningRate**: 0.2 (β)
- **discountFactor**: 0.71 (γ)
- **doGASubsumption**: true
- **predictionErrorReduction**: 0.25
- **fitnessReduction**: 0.1
- **thetaGA**: 25 (GA invocation threshold)
- **thetaDel**: 20 (deletion threshold)
- **thetaSub**: 20 (subsumption threshold)

### Exploration vs Exploitation

The `Epsilon` parameter controls the balance:

- **ε = 0.0**: Pure exploitation (always select best action)
- **ε = 0.5**: Alternating (explore then exploit)
- **ε = 0.8**: Mostly exploration (80% explore, 20% exploit)
- **ε = 1.0**: Dynamic epsilon-greedy (decreasing over time)

## Troubleshooting

### Database Connection Issues

If the XCS container cannot connect to the database:

```bash
# Check database container is running
docker ps

# Check database logs
docker logs rubisdb1  # or dvd2db

# Verify database initialization
docker exec -it rubisdb1 mysql -e "SHOW DATABASES;"
```

### JMeter Execution Failures

If JMeter tests fail:

```bash
# Verify JMeter installation
docker exec -it xcs-dev jmeter --version

# Check JMX file exists
docker exec -it xcs-dev ls -l /app/src/config/RUBiS-testplan/

# Test JMeter manually
docker exec -it xcs-dev jmeter -n -t config/RUBiS-testplan/testplan.jmx -l /tmp/test.jtl
```

### Compilation Errors

If C++ compilation fails:

```bash
# Check compiler version
docker exec -it xcs-dev g++ --version

# Verify libraries
docker exec -it xcs-dev pkg-config --libs --cflags tinyxml2 mariadb

# Clean and rebuild
docker exec -it xcs-dev bash -c "cd /app/src && rm -f xcs && make clean"
```

### Performance Issues

If the system runs slowly:

1. **Reduce maxProblems**: Lower the number of iterations
2. **Increase testFrequency**: Write results less frequently
3. **Reduce Population Size**: Modify `maxPopSize` in configuration
4. **Allocate More Resources**: Increase Docker memory/CPU limits

### Empty Results

If no results are generated:

```bash
# Check results directory permissions
docker exec -it xcs-dev ls -la /app/results

# Verify file handles
docker exec -it xcs-dev bash -c "lsof -p \$(pgrep xcs)"

# Check for crashes
docker logs xcs-prod
```

## Advanced Usage

### Running Multiple Experiments

Create multiple configuration files and run them sequentially:

```bash
# config-experiment1.json, config-experiment2.json, etc.

for config in config/config-experiment*.json; do
    cp "$config" config/config.json
    docker-compose --profile prod up xcs-prod
done
```

### Analyzing Results

Use the Python plotting script:

```bash
cd XCS/src
python3 PlotSC.py
```

Or MATLAB:

```matlab
cd XCS/src
PlotSC
```

### Extending to New Systems

To add support for a new SUT:

1. Create Docker containers for web and database
2. Add entry in `docker-compose.yml`
3. Create JMeter test plan in `XCS/config/`
4. Implement database connection in `env.cpp`
5. Add reset script in `XCS/src/`
6. Update `applyConfig()` function in `xcs.cpp`
