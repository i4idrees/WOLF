#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring> // C-style string manipulation
#include <memory>  // For std::unique_ptr
#include <array>   // For std::array
#include <cmath>   // Replaced <math.h> with <cmath> (C++ standard header)
#include <tinyxml2.h>
// #include <windows.h>     // Windows-specific headers
#include <thread>        // For multi-threading
#include <chrono>        // For time utilities
#include <algorithm>     // For algorithms
#include <utility>       // For std::pair and utility functions
#include <unordered_map> // For hash maps
#include <cctype>        // For character classification
#include <regex>         // For regular expressions
#include <mysql.h>       // For database operations

#include <sys/stat.h>
#include <cstdlib>

#include "env.h"

//#include "xcsMacros.h" // Project-specific headers
//#include "classifier.h"
#include "xcscfc/xcscfc_api.h"
#include "configuration.h"

// start1of2: pause and resume the code
#include <stdbool.h>
#ifdef _WIN32
// Windows-specific headers
#include <conio.h>
#include <windows.h>
#define sleep_ms(ms) Sleep(ms)
#else
// Linux-specific headers
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#define sleep_ms(ms) usleep((ms) * 1000)

// Equivalent to _kbhit()
int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

// Equivalent to _getch()
char getch(void)
{
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif
// end1of2: pause and resume the code

#ifdef _WIN32
#include <windows.h> // Windows-specific headers
#else
#include <unistd.h> // Linux-specific headers
#include <sys/wait.h>
#endif

extern char path[50];
extern char outputFile[512];
extern char outputFile2[512];
extern char outputFile3[512];

extern int maxEpochs;

bool isActCorrect;

double toleranceForNoise = 0.5;

bool checkPreviousBeatAgain = false;

int prevEpisode = 1;

extern ClassifierSet *population;

extern double epsilon;

double prev_AvgRT = 0;
double prev_ER = 0;

int prev_AvgRT_int = 0;

double temp_AvgRT = 0;

bool crossCheckRT = false;

extern double elapsed_time_ms;

extern std::string resetDBFile;

extern bool exploit;

int retryCount = 0;

float cachedRT = -1;

MYSQL *conn = NULL;

extern FILE *filePerformance, *testPerformance, *fileClassifierPopulation, *cfWritingFilePointer, *cfReadingFilePointer;

extern int episod;
extern int episodStep;
extern int solSteps;

int tempSteps = 0;

int solutionsDiscovered[50000][13];
int noOfSolutionsDiscovered = 0;

int threadsPerSecond = 500;
int loops = 3;

float reward;

int action = 0;
int counter = 0;

float lowerLimit[maxCondLength];
float upperLimit[maxCondLength];

const int maxWorkloadAllowed = 500;

bool initFlag = 1;

float current_AvgRT;
float current_ER;

float actual_AvgRT;

int currentTotalWorkload = 0;

extern float testObjective1_AvgRT;
extern float testObjective2_ER;

int totalWorkload = 0;
int rampUpTime[maxNum_transactions];

int actionsTried = 0;

int actionTried[maxNum_transactions] = {0};

const int preStates = 500;
int preStatesCount = 0;
int preTransactionsState[preStates][maxNum_transactions] = {0};
double pre_beat[preStates] = {0};
double preRT = 0;

int prepreTransactionsState[maxNum_transactions] = {0};
double prepreRT = 0;

int preAction = -1;

int prepreAction = -1;

std::string jmeterPath = "/opt/apache-jmeter-5.6.2/bin/jmeter";

std::string DOCKER_JTL_PATH = "config/results.jtl";

const char *iterationsLogFile = "results/iterations_log.csv";
const char *solutionsLogFile = "results/solutions_log.csv";

void initializeLimit()
{
    for (int i = 0; i < condLength; i++)
    {
        lowerLimit[i] = 0.0;
        upperLimit[i] = 0.0;
    }
}

void initializeInput(DataSource inputArray[], int numofRows)
{
    int featureIndex, docIndex;
    for (docIndex = 0; docIndex < numofRows; docIndex++)
    {
        inputArray[docIndex].state = new float[condLength];
        for (featureIndex = 0; featureIndex < condLength; featureIndex++)
        {
            inputArray[docIndex].state[featureIndex] = 0.0;
        }
        inputArray[docIndex].action = 0;
    }
}

float roundRealValue(float val, int num)
{
    float p = (float)pow(10.0, num);
    val = val * p;
    float tmp = roundf(val);
    return tmp / p;
}

float ScaleRange(float Value, float FromMinValue, float FromMaxValue, float ToMinValue, float ToMaxValue)
{
    return (Value - FromMinValue) * (ToMaxValue - ToMinValue) / (FromMaxValue - FromMinValue) + ToMinValue;
}

void updateRange(DataSource data[], int totalRows)
{
    for (int docNo = 0; docNo < totalRows; docNo++)
    {
        for (int featureNo = 0; featureNo < condLength; featureNo++)
        {
            data[docNo].state[featureNo] = ScaleRange(data[docNo].state[featureNo], lowerLimit[featureNo], upperLimit[featureNo], 0.0, 1.0);
            data[docNo].state[featureNo] = roundRealValue(data[docNo].state[featureNo], precisionDigits);
        }
    }
}

bool isIntegerInArray(int integer, int array[totalNumInstances], int highestPosition)
{
    for (int i = 0; i < highestPosition; i++)
    {
        if (array[i] == integer)
            return true;
    }
    return false;
}

int Environment(int action)
{
    return executeActionAndGetObservations(action);
}

void displayUpdatedTransactions(Transaction transactions[], int num_transactions)
{
    printf("\n\n======================\n\n");
    printf("Updated transactions:\n");
    printf("\n\n======================\n\n");
    for (int i = 0; i < num_transactions; i++)
    {
        printf("Transaction: %s, Workload: %d\n", transactions[i].name, transactions[i].workLoad);
    }

    printf("\n\n======================================\n\n");
}

void increaseWorkloadOfAll(Transaction transactions[], int num_transactions, int workloadModificatoins)
{
    for (int i = 0; i < num_transactions; i++)
    {
        if (transactions[i].workLoad + workloadModificatoins > 0)
        {
            transactions[i].workLoad += workloadModificatoins;
        }
        else
            transactions[i].workLoad = 0;
    }
}

void resetWorkload(int userPerTransaction)
{
    for (int i = 0; i < num_transactions; i++)
    {
        transactions[i].workLoad = userPerTransaction;
    }
}

void increaseWorkloadOfParticular(Transaction transactions[], const char *transaction_name, int workloadModificatoins)
{
    for (int i = 0; i < num_transactions; i++)
    {

        if (strcmp(transactions[i].name, transaction_name) == 0)
        {
            if (transactions[i].workLoad + workloadModificatoins > 0)
            {
                transactions[i].workLoad += workloadModificatoins;
                // printf("Updated %s: Workload: %d\n", transactions[i].name, transactions[i].workLoad);
                return; // Exit the function after updating the specified transaction
            }
            else
                transactions[i].workLoad = 0;
        }
    }
    printf("Transaction with name '%s' not found.\n", transaction_name);
}

// Function to update thread group num_threads, LoopController.loops, and ThreadGroup.ramp_time based on transactions array
void update_thread_group_num_threads(tinyxml2::XMLElement *element, Transaction transactions[], int num_transactions, int loops, int rampTime[])
{
    if (!element)
        return; // Check for null element

    // Iterate over child elements
    for (tinyxml2::XMLElement *child = element->FirstChildElement(); child; child = child->NextSiblingElement())
    {
        // Check if the element is a ThreadGroup
        if (strcmp(child->Name(), "ThreadGroup") == 0)
        {
            // Get the testname attribute from the ThreadGroup element
            const char *testname = child->Attribute("testname");
            if (!testname)
                continue; // Check for null attribute

            // Iterate over transactions to find a match
            for (int i = 0; i < num_transactions; ++i)
            {
                if (strcmp(transactions[i].name, testname) == 0)
                {
                    // Find and update the relevant properties
                    for (tinyxml2::XMLElement *property = child->FirstChildElement(); property; property = property->NextSiblingElement())
                    {
                        const char *name = property->Attribute("name");
                        if (name)
                        {
                            if (strcmp(property->Name(), "stringProp") == 0)
                            {
                                if (strcmp(name, "ThreadGroup.num_threads") == 0)
                                {
                                    // Update the num_threads with the workload value
                                    property->SetText(std::to_string(transactions[i].workLoad).c_str());
                                    // std::cout << "Updated " << testname << " num_threads to " << transactions[i].workLoad << std::endl;
                                }
                                else if (strcmp(name, "ThreadGroup.ramp_time") == 0)
                                {
                                    // Update the ramp_time
                                    property->SetText(std::to_string(rampTime[i]).c_str());
                                    // std::cout << "Updated " << testname << " ramp_time to " << rampTime << std::endl;
                                }
                            }
                            else if (strcmp(property->Name(), "elementProp") == 0 && strcmp(name, "ThreadGroup.main_controller") == 0)
                            {
                                // Look for LoopController.loops within the elementProp for main_controller
                                for (tinyxml2::XMLElement *subProperty = property->FirstChildElement(); subProperty; subProperty = subProperty->NextSiblingElement())
                                {
                                    if (strcmp(subProperty->Name(), "stringProp") == 0 && strcmp(subProperty->Attribute("name"), "LoopController.loops") == 0)
                                    {
                                        // Update the loops
                                        subProperty->SetText(std::to_string(loops).c_str());
                                        // std::cout << "Updated " << testname << " loops to " << loops << std::endl;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    break; // Exit the loop once the matching transaction is processed
                }
            }
        }
        // Recursively process child elements
        update_thread_group_num_threads(child, transactions, num_transactions, loops, rampTime);
    }
}

void updateThreadgroups(Transaction transactions[], int num_transactions, int rampUpTime[], int loops, const char *filename)
{
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLError result = doc.LoadFile(filename);
    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error: could not parse file " << filename << std::endl;
    }

    tinyxml2::XMLElement *root = doc.RootElement();
    update_thread_group_num_threads(root, transactions, num_transactions, loops, rampUpTime);

    result = doc.SaveFile(filename);
    if (result != tinyxml2::XML_SUCCESS)
    {
        std::cerr << "Error: could not save file " << filename << std::endl;
    }
}



std::tuple<double, double> calculateStats(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return {-1.0, -1.0};
    }

    char line[2048];
    long totalElapsed = 0;
    int totalCount = 0;
    int errorCount = 0;

    // Skip header
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return {-1.0, -1.0};
    }

    while (fgets(line, sizeof(line), file))
    {
        char *token;
        int column = 0;
        long elapsed = 0;
        bool success = true;

        token = strtok(line, ",");

        while (token != NULL)
        {
            // Column 1 = elapsed
            if (column == 1)
            {
                elapsed = atol(token);
                totalElapsed += elapsed;
                totalCount++;
            }

            // Column 7 = success (true/false in JMeter CSV default)
            if (column == 7)
            {
                if (strcmp(token, "false") == 0)
                {
                    errorCount++;
                }
                break;  // no need to parse further
            }

            token = strtok(NULL, ",");
            column++;
        }
    }

    fclose(file);

    if (totalCount == 0)
        return {0.0, 0.0};

    double avgResponse = (double)totalElapsed / totalCount;
    double errorRate = ((double)errorCount / totalCount) * 100.0;

    return {avgResponse, errorRate};
}


double calculateAverageResponseTime(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1.0;
    }

    char line[1024];
    long totalElapsed = 0;
    int count = 0;

    // Skip header
    if (!fgets(line, sizeof(line), file))
    {
        fclose(file);
        return -1.0;
    }

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int column = 0;

        while (token != NULL)
        {
            if (column == 1) // elapsed time
            {
                totalElapsed += atol(token);
                count++;
                break;
            }
            token = strtok(NULL, ",");
            column++;
        }
    }

    fclose(file);

    return (count == 0) ? 0.0 : (double)totalElapsed / count;
}
double calculateErrorRate(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1.0;
    }

    char line[1024];
    int total = 0;
    int errors = 0;

    // Skip header
    if (!fgets(line, sizeof(line), file))
    {
        fclose(file);
        return -1.0;
    }

    while (fgets(line, sizeof(line), file))
    {
        char *token = strtok(line, ",");
        int column = 0;

        while (token != NULL)
        {
            if (column == 7) // success column
            {
                if (strcmp(token, "false") == 0)
                {
                    errors++;
                }
                total++;
                break;
            }
            token = strtok(NULL, ",");
            column++;
        }
    }

    fclose(file);

    return (total == 0) ? 0.0 : (double)errors / total;
}


//only average response time
/*double calculateAverageResponseTime(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening file");
        return -1.0;
    }

    char line[1024];
    long totalElapsed = 0;
    int count = 0;

    // Skip header line
    if (fgets(line, sizeof(line), file) == NULL)
    {
        fclose(file);
        return -1.0;
    }

    while (fgets(line, sizeof(line), file))
    {
        char *token;
        int column = 0;
        long elapsed = 0;

        token = strtok(line, ",");
        while (token != NULL)
        {
            if (column == 1) // Elapsed time is at column index 1
            {
                elapsed = atol(token);
                totalElapsed += elapsed;
                count++;
                break; // no need to parse further columns
            }
            token = strtok(NULL, ",");
            column++;
        }
    }

    fclose(file);

    if (count == 0)
        return 0.0;

    return (double)totalElapsed / count;
}*/



void calculate_average_elapsed(const char *input_file, const char *output_csv)
{
#define MAX_GROUPS 12
#define MAX_NAME_LEN 100

    typedef struct
    {
        char name[MAX_NAME_LEN];
        long total_elapsed;
        int count;
    } ThreadGroupStats;

    ThreadGroupStats groups[MAX_GROUPS];
    int num_groups = 0;

    FILE *file = fopen(input_file, "r");
    if (!file)
    {
        perror("Cannot open input file");
        return;
    }

    char line[1024];
    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file))
    {
        char *token;
        char *elapsed_str = NULL;
        char *threadName = NULL;
        int col = 0;

        token = strtok(line, ",");
        while (token)
        {
            if (col == 1)
                elapsed_str = token;
            if (col == 5)
                threadName = token;
            token = strtok(NULL, ",");
            col++;
        }

        if (elapsed_str && threadName)
        {
            char clean_name[MAX_NAME_LEN] = {0};
            int i = 0;
            while (threadName[i] && !(isdigit(threadName[i]) && threadName[i - 1] == ' '))
            {
                clean_name[i] = threadName[i];
                i++;
            }
            clean_name[i] = '\0';

            int index = -1;
            for (int j = 0; j < num_groups; j++)
            {
                if (strcmp(groups[j].name, clean_name) == 0)
                {
                    index = j;
                    break;
                }
            }
            if (index == -1 && num_groups < MAX_GROUPS)
            {
                strcpy(groups[num_groups].name, clean_name);
                groups[num_groups].total_elapsed = 0;
                groups[num_groups].count = 0;
                index = num_groups++;
            }

            if (index != -1)
            {
                groups[index].total_elapsed += atol(elapsed_str);
                groups[index].count++;
            }
        }
    }
    fclose(file);

    FILE *out = fopen(output_csv, "a"); // <-- changed to append
    if (!out)
    {
        perror("Cannot open output file");
        return;
    }

    // Only write averages (values), not headers
    for (int i = 0; i < num_groups; i++)
    {
        double avg = groups[i].count ? (double)groups[i].total_elapsed / groups[i].count : 0.0;
        fprintf(out, "%.2f", avg);
        if (i < num_groups - 1)
            fprintf(out, ",");
    }
    fprintf(out, "\n");

    fclose(out);
}


std::tuple<double, double> runJMeterAndGetStats()
{
    resetDB();
    remove(DOCKER_JTL_PATH.c_str());

    std::string cmd = "jmeter -n -t " + std::string(jmxFile) +
                      " -l " + std::string(DOCKER_JTL_PATH) +
                      " 2>&1";

    FILE *pipe = popen(cmd.c_str(), "r");

    if (!pipe)
    {
        std::cerr << "Failed to execute JMeter command." << std::endl;
        return {0.0, 0.0};
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        // optional: print logs
    }

    pclose(pipe);

    double avgRT = calculateAverageResponseTime(DOCKER_JTL_PATH.c_str());
    double errRate = calculateErrorRate(DOCKER_JTL_PATH.c_str());

    return {avgRT, errRate};
}
// only for average response time
/*std::tuple<double, double> runJMeterAndGetStats()
{
    resetDB();

    remove(DOCKER_JTL_PATH.c_str());

    // std::string cmd = "jmeter -n -t " + jmxFile + " -l " + DOCKER_JTL_PATH + " 2>&1";

    std::string cmd = "jmeter -n -t " + std::string(jmxFile) +
                      " -l " + std::string(DOCKER_JTL_PATH) +
                      " 2>&1";

    FILE *pipe = popen(cmd.c_str(), "r");

    if (!pipe)
    {
        std::cerr << "Failed to execute JMeter command." << std::endl;
        return {0.0, 0.0}; // Or handle as appropriate
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), pipe) != NULL)
    {
        // std::cout << buffer;
    }

    pclose(pipe);

    double averageResponseTime = 0.0;
    double errorRate = 0.0;

    //sleep(10); // for linux

    averageResponseTime = calculateAverageResponseTime(DOCKER_JTL_PATH.c_str());

    return {averageResponseTime, errorRate};
}*/

void resetDB()
{
    std::string command = "src/" + resetDBFile;
    int ret = system(command.c_str());

    if (ret != 0)
    {
        std::cerr << "Failed to reset database, script returned " << ret << std::endl;
    }
    else
    {
        // std::cout << "Database reset done.\n";
    }
}

bool initDBConnectionRUBIS()
{
    const char *HOST = "rubisdb1";
    const char *DATABASE = "perf_results";
    const char *USERNAME = "root";
    const char *PASSWORD = "";
    unsigned int PORT = 3306;

    conn = mysql_init(NULL);
    if (!conn)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return false;
    }

    if (!mysql_real_connect(conn, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0))
    {
        fprintf(stderr, "mysql_real_connect() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        conn = NULL;
        return false;
    }

    return true;
}

bool initDBConnectionDVD()
{
    const char *HOST = "dvd2db";
    const char *DATABASE = "perf_results";
    const char *USERNAME = "root";
    const char *PASSWORD = "";
    unsigned int PORT = 3306;

    conn = mysql_init(NULL);
    if (!conn)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return false;
    }

    if (!mysql_real_connect(conn, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0))
    {
        fprintf(stderr, "mysql_real_connect() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        conn = NULL;
        return false;
    }

    return true;
}

bool initDBConnectionTEASTORE()
{
    const char *HOST = "db";             // TeaStore DB container name in Compose
    const char *DATABASE = "perf_results";
    const char *USERNAME = "teauser";    // use the TeaStore user
    const char *PASSWORD = "teapassword";
    unsigned int PORT = 3306;

    conn = mysql_init(NULL);
    if (!conn)
    {
        fprintf(stderr, "mysql_init() failed\n");
        return false;
    }

    if (!mysql_real_connect(conn, HOST, USERNAME, PASSWORD, DATABASE, PORT, NULL, 0))
    {
        fprintf(stderr, "mysql_real_connect() failed. Error: %s\n", mysql_error(conn));
        mysql_close(conn);
        conn = NULL;
        return false;
    }

    printf("Connected to TeaStore perf_results database successfully!\n");
    return true;
}

void buildCombination(char *combination, size_t bufferSize)
{
    combination[0] = '\0';

    for (int i = 0; i < num_transactions; i++)
    {
        char temp[16];
        snprintf(temp, sizeof(temp), "%d", transactions[i].workLoad);

        // Prevent buffer overflow
        if (strlen(combination) + strlen(temp) + 2 >= bufferSize)
        {
            fprintf(stderr, "Combination buffer overflow risk!\n");
            return;
        }

        strcat(combination, temp);

        if (i < num_transactions - 1)
            strcat(combination, ",");
    }
}

// new
float getCachedResponseTime()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return -1.0f;
    }

    char combination[1024];
    buildCombination(combination, sizeof(combination));

    printf("SELECT combination: '%s'\n", combination);

    char query[2048];
    snprintf(query, sizeof(query),
             "SELECT avg_response_time FROM workload_cache WHERE combination='%s'",
             combination);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return -1.0f;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
    {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return -1.0f;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row)
    {
        float result = atof(row[0]);
        mysql_free_result(res);
        return result;
    }

    mysql_free_result(res);
    return -1.0f;
}

// old
/*float getCachedResponseTime()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return -1.0f;
    }

    // Build combination string (comma separated)
    char combination[512] = "";
    for (int i = 0; i < maxCondLength; i++)
    {
        char temp[16];
        sprintf(temp, "%d", transactions[i].workLoad);
        strcat(combination, temp);
        if (i < 24)
            strcat(combination, ",");
    }

    char query[1024];
    snprintf(query, sizeof(query),
             "SELECT avg_response_time FROM workload_cache WHERE combination='%s'",
             combination);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return -1.0f;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
    {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return -1.0f;
    }

    MYSQL_ROW row = mysql_fetch_row(res);
    if (row)
    {
        float cRT = atof(row[0]);
        mysql_free_result(res);
        return cRT; // Cached avgRT found
    }

    mysql_free_result(res);
    return -1.0f; // Not found
}*/

// new
/*bool insertCombinationInDB(float avgRT, float errorRate)
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    char combination[1024];
    buildCombination(combination, sizeof(combination));

    printf("INSERT combination: '%s'\n", combination);

    char query[2048];
    snprintf(query, sizeof(query),
             "INSERT INTO workload_cache (combination, avg_response_time, error_rate) "
             "VALUES ('%s', %.3f, %.3f)",
             combination, avgRT, errorRate);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
        return false;
    }

    return true;
}*/

bool logCombination(float avgRT, float errorRate)
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    char combination[1024];
    buildCombination(combination, sizeof(combination));

    printf("INSERT combination: '%s'\n", combination);

    char query[2048];
    snprintf(query, sizeof(query),
             "INSERT INTO workload_cache (combination, avg_response_time, error_rate) "
             "VALUES ('%s', %.3f, %.3f)",
             combination, avgRT, errorRate);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
        return false;
    }

    // -------- CSV LOGGING START --------
    system("mkdir -p results/knowledge");

    const char *filePath = "results/knowledge/workload_cache_export.csv";

    FILE *fp = fopen(filePath, "a");
    if (!fp)
    {
        perror("CSV open failed");
        return true;  // DB success, so don't fail whole function
    }

    // Write header if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0)
    {
        fprintf(fp, "combination,avg_response_time,error_rate\n");
        fflush(fp);
    }

    // Write row
    fprintf(fp, "\"%s\",%.3f,%.3f\n", combination, avgRT, errorRate);

    fflush(fp);
    fsync(fileno(fp));   // 🔥 strong durability (optional but best)

    fclose(fp);
    // 🔥 -------- CSV LOGGING END --------

    return true;
}

// old
/*bool insertCombinationInDB(float avgRT, float errorRate)
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    // Build combination string
    char combination[512] = "";
    for (int i = 0; i < maxNum_transactions; i++)
    {
        char temp[16];
        sprintf(temp, "%d", transactions[i].workLoad);
        strcat(combination, temp);
        if (i < (maxNum_transactions - 1))
            strcat(combination, ",");
    }

    char query[1024];
    snprintf(query, sizeof(query),
             "INSERT INTO workload_cache (combination, avg_response_time, error_rate) "
             "VALUES ('%s', %.3f, %.3f)",
             combination, avgRT, errorRate);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "INSERT failed: %s\n", mysql_error(conn));
        return false;
    }

    return true;
}*/

bool combinationExistsInBlacklist()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    // Build combination string exactly as stored in DB (comma-separated, no quotes)
    char combination[512] = "";
    for (int i = 0; i < num_transactions; i++)
    {
        char temp[16];
        sprintf(temp, "%d", transactions[i].workLoad);
        strcat(combination, temp);
        if (i < 24)
            strcat(combination, ",");
    }

    // Escape for SQL safety
    char escaped[1024];
    mysql_real_escape_string(conn, escaped, combination, strlen(combination));

    char query[2048];
    snprintf(query, sizeof(query),
             "SELECT 1 FROM blacklist_combinations WHERE combination='%s' LIMIT 1",
             escaped);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        return false;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
    {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return false;
    }

    bool exists = mysql_num_rows(res) > 0;
    mysql_free_result(res);

    return exists;
}

bool insertIntoBlacklist()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    // Build combination string (exact same format as DB)
    char combination[512] = "";
    for (int i = 0; i < num_transactions; i++)
    {
        char temp[16];
        sprintf(temp, "%d", transactions[i].workLoad);
        strcat(combination, temp);
        if (i < 24)
            strcat(combination, ",");
    }

    // Escape for SQL safety
    char escaped[1024];
    mysql_real_escape_string(conn, escaped, combination, strlen(combination));

    char query[2048];
    snprintf(query, sizeof(query),
             "INSERT INTO blacklist_combinations (combination) VALUES ('%s') "
             "ON DUPLICATE KEY UPDATE combination = VALUES(combination)",
             escaped);

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "INSERT failed for combination '%s': %s\n",
                combination, mysql_error(conn));
        return false;
    }

    return true;
}

std::string trimThreadGroup(const std::string& src)
{
    size_t pos = src.find("_thread_group");
    if (pos != std::string::npos)
    {
        return src.substr(0, pos);
    }
    return src;
}

void writeLogCSV(const char *logFileName, int episode, int totalWorkload, int episodeStep, float responseTime,
                 float errorRate)
{
    // Open the file in append mode
    FILE *file = fopen(logFileName, "a");
    if (!file)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Write header if the file is empty
    if (!logHeaderWritten)
    {
        fprintf(file, "ART_Threshold,ER_Threshold,Epsilon\n");
        fprintf(file, "%f,%f,%f\n", testObjective1_AvgRT, testObjective2_ER, epsilon);

        fprintf(file, "episode,totalWorkload,episodeStep,responseTime,errorRate\n");
        logHeaderWritten = 1;
    }

    // Write episode row
    fprintf(file, "%d,%d,%d,%.2f,%.2f\n",
            episode, totalWorkload, episodeStep, responseTime, errorRate);

    // Close the file
    fclose(file);
}

void writeFullLogCSV(const char *fullLogFileName, int episode, int episodeStep, bool exploit,
                     double elapsedTime, int solutionNo, const std::string &stepReply, double reward,
                     double responseTime, float errorRate, int totalWorkload)
{
    FILE *file = fopen(fullLogFileName, "a");
    if (!file)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    // Write header only once
    if (!fullLogHeaderWritten)
    {
        // Write test objectives
        fprintf(file, "ART_Threshold,ER_Threshold,Epsilon\n");
        fprintf(file, "%f,%f,%f\n", testObjective1_AvgRT, testObjective2_ER, epsilon);

        // Common columns
        fprintf(file, "episode,episodeStep,exploit,elapsedTime,solutionNo,stepReply,reward,responseTime,errorRate,totalWorkload");

        // Add transaction names as header columns dynamically (truncate after first '_')
        for (int i = 0; i < num_transactions; i++)
        {
            char tempName[100];
            strncpy(tempName, transactions[i].name, sizeof(tempName) - 1);
            tempName[sizeof(tempName) - 1] = '\0'; // safety

            char *underscore = strchr(tempName, '_');
            if (underscore)
                *underscore = '\0'; // truncate at first underscore

            fprintf(file, ",%s", tempName);
        }
        fprintf(file, "\n");

        fullLogHeaderWritten = 1;
    }

    // Write row data
    fprintf(file, "%d,%d,%d,%.5f,%d,%s,%.5f,%.5f,%.2f,%d",
            episode, episodeStep, exploit, elapsedTime, solutionNo,
            stepReply.c_str(), reward, responseTime, errorRate, totalWorkload);

    // Add workloads dynamically
    for (int i = 0; i < num_transactions; i++)
    {
        fprintf(file, ",%d", transactions[i].workLoad);
    }
    fprintf(file, "\n");

    fclose(file);
}

void copy_file_to_docker()
{
    char command[512];

    snprintf(command, sizeof(command), "docker cp %s %s:%s > NUL 2>&1", jmxFile, "jmeter", jmxFile);

    int status = system(command);
    if (status == 0)
    {
        // Success (output suppressed)
    }
    else
    {
        printf("Failed to replace JMX file in Docker.\n");
    }
}

/*void CreateFiles(int j)
{
    int currentProblemLevel = 1;

    char path[10] = "";
    char curLevel[2] = "";
    char prevLevel[2] = "";

    sprintf(curLevel, "%d", currentProblemLevel);
    sprintf(prevLevel, "%d", currentProblemLevel-1);

    sprintf(path, "%d", j);
    mkdir(path);
    setSeed(seeds[j%30]);

    char outputFile[512];
    sprintf(outputFile,"%s\\%s%s.txt", path, outputFileName, curLevel);
    filePerformance = fopen(outputFile, "w"); //open outputFile in writing mode
    if (filePerformance == NULL)
    {
        printf("Error in opening a file.. %s", outputFile);
        exit(1);
    }

    char outputFile2[512];
    sprintf(outputFile2,"%s\\%s%s.txt", path, featureFileName, curLevel);
    cfWritingFilePointer = fopen(outputFile2, "w"); //open outputFile2 in writing mode

    if(cfWritingFilePointer == NULL)
    {
        printf("Error in opening a file.. %s", outputFile2);
        exit(1);
    }

    char outputFile3[512];
    sprintf(outputFile3,"%s\\%s%s.txt", path, ruleFileName, curLevel);
    fileClassifierPopulation = fopen(outputFile3, "w"); //open outputFile3 in writing mode
    if (fileClassifierPopulation == NULL)
    {
        printf("Error in opening a file.. %s", outputFile3);
        exit(1);
    }

    if (currentProblemLevel > 1)
    {
        char inputFile[512];
        sprintf(inputFile,"%s\\%s%s.txt", path, featureFileName, prevLevel );
        cfReadingFilePointer = fopen(inputFile, "r"); //open inputFile in reading mode
        if(cfReadingFilePointer == NULL)
        {
            printf("Error in opening a file.. %s", inputFile);
            exit(1);
        }
    }
}*/

void initEnviournment()
{

    //sleep(3); // parameter is second no milliseconds

    resetWorkload(0); // Reset workload to zero

    prev_AvgRT = 0;

    currentTotalWorkload = 0;

    for (int i = 0; i < noOfObservations; i++)
    {
        observations[i] = 0;
    }
}

// Compare function for qsort
int compare(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

// Function to calculate average of 3 closest integers in value
double averageOfThreeClosest(int arr[5])
{
    qsort(arr, 5, sizeof(int), compare);

    int minDiff = INT_MAX;
    int bestStartIndex = 0;

    // Find the subarray of size 3 with the smallest range
    for (int i = 0; i <= 2; i++)
    {
        int diff = arr[i + 2] - arr[i];
        if (diff < minDiff)
        {
            minDiff = diff;
            bestStartIndex = i;
        }
    }

    // Compute the average
    int sum = arr[bestStartIndex] + arr[bestStartIndex + 1] + arr[bestStartIndex + 2];
    return sum / 3.0;
}

bool verifyBeatPrevRT()
{
    std::tuple<double, double> stats2;
    double tempRTTT = 0;
    double minRTTemp = current_AvgRT;
    std::cout << "\nLets cross check = ";
    for (int i = 0; i < 3; i++)
    {
        stats2 = runJMeterAndGetStats();
        tempRTTT = std::get<0>(stats2);

        std::cout << tempRTTT << "  :  ";

        if (tempRTTT < minRTTemp)
        {
            minRTTemp = tempRTTT;
        }
    }
    /*std::cout<<"\nMin is = "<<minRTTemp<<"\n";
    std::cout<<"\nprev_AvgRT is = "<<prev_AvgRT<<"\n";*/
    if ((minRTTemp - prev_AvgRT) >= -toleranceForNoise)
    {
        // prev_AvgRT = minRTTemp;
        current_AvgRT = minRTTemp;
        return true;
    }
    current_AvgRT = minRTTemp;

    return false;
}

void combinationUtil(int arr[], int data[], int start, int end, int index, int r, int transaction_number, int ***result, int *count)
{
    if (index == r)
    {
        // Store one combination: fixed transaction + selected others
        (*result)[*count][0] = transaction_number;
        for (int j = 0; j < r; j++)
        {
            (*result)[*count][j + 1] = data[j];
        }
        (*count)++;
        return;
    }

    for (int i = start; i <= end && end - i + 1 >= r - index; i++)
    {
        data[index] = arr[i];
        combinationUtil(arr, data, i + 1, end, index + 1, r, transaction_number, result, count);
    }
}

void generate_combinations(int transaction_number, int factor, int ***result, int *count)
{
    int transactions[12];
    int idx = 0;

    // Initialize transactions 0 to 11
    for (int i = 0; i < 12; i++)
    {
        if (i != transaction_number)
        {
            transactions[idx++] = i;
        }
    }

    int n = 11; // Now 11 transactions after removing one

    // Calculate number of combinations: nCr where r = factor - 1
    int r = factor - 1;
    int total = 1;
    for (int i = 0; i < r; i++)
    {
        total *= (n - i);
        total /= (i + 1);
    }

    // Allocate memory for result
    *result = (int **)malloc(total * sizeof(int *));
    for (int i = 0; i < total; i++)
    {
        (*result)[i] = (int *)malloc(factor * sizeof(int)); // each combination of 'factor' numbers
    }

    *count = 0;

    // Helper array to keep track of current combination
    int *data = (int *)malloc(r * sizeof(int));

    combinationUtil(transactions, data, 0, n - 1, 0, r, transaction_number, result, count);

    free(data);
}

/*bool readEnvConfigurations(){
    double maxPayoff_test;
    unsigned int noOfObservations_test;

    int num_transactions_test;
    std::vector<Transaction> transactions_test;

    std::ifstream envConfig("config/env_config.txt");
    if (!envConfig) {
        std::cerr << "Error: could not open env_config.txt\n";
        return 1;
    }

    std::string line;
    while (std::getline(envConfig, line)) {
        if (line.find("numActions=") == 0) {
            numActions = atoi(line.substr(11).c_str());
        }
        else if (line.find("actions=") == 0) {
            std::string values = line.substr(8);
            std::stringstream ss(values);
            std::string token;
            while (std::getline(ss, token, ',')) {
                allPossibleActions.push_back(atoi(token.c_str()));
            }
        }
        else if (line.find("noOfObservations=") == 0) {
            noOfObservations_test = (unsigned int)atoi(line.substr(17).c_str());
        }
        else if (line.find("maxPayoff=") == 0) {
            maxPayoff_test = atof(line.substr(10).c_str());
        }
        else if (line.find("numTransactions=") == 0) {
            num_transactions_test = atoi(line.substr(15).c_str());
        }
        else if (line.find("transactions=") == 0) {
            std::string values = line.substr(13);
            std::stringstream ss(values);
            std::string token;
            while (std::getline(ss, token, ',')) {
                Transaction t;
                t.name = new char[token.size() + 1];
                strcpy(t.name, token.c_str());
                t.workLoad = 0;
                transactions_test.push_back(t);
            }
        }
    }
    envConfig.close();

    // Print out for verification
    std::cout << "numActions = " << numActions << "\n";
    std::cout << "actions: ";
    for (size_t i = 0; i < allPossibleActions.size(); ++i)
        std::cout << allPossibleActions[i] << " ";
    std::cout << "\n";

    std::cout << "noOfObservations = " << noOfObservations_test << "\n";
    std::cout << "maxPayoff = " << maxPayoff_test << "\n";
    std::cout << "num_transactions = " << num_transactions_test << "\n";

    std::cout << "transactions:\n";
    for (size_t i = 0; i < transactions_test.size(); ++i) {
        std::cout << "  " << transactions_test[i].name
                  << " -> " << transactions_test[i].workLoad << "\n";
    }

}*/

bool exportBlacklistCombinationsToCSV()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    const char *filepath = "results/knowledge/blacklist_combinations.csv";

    // Delete old file if exists
    remove(filepath);

    FILE *fp = fopen(filepath, "w");
    if (!fp)
    {
        fprintf(stderr, "Failed to open %s for writing.\n", filepath);
        return false;
    }

    const char *query = "SELECT combination FROM blacklist_combinations";
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "SELECT query failed: %s\n", mysql_error(conn));
        fclose(fp);
        return false;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
    {
        fprintf(stderr, "Failed to store result: %s\n", mysql_error(conn));
        fclose(fp);
        return false;
    }

    MYSQL_ROW row;
    int exportedCount = 0;
    while ((row = mysql_fetch_row(res)))
    {
        if (row[0])
        {
            fprintf(fp, "%s\n", row[0]);
            exportedCount++;
        }
    }

    mysql_free_result(res);
    fclose(fp);
    printf("Exported %d blacklist combinations to CSV successfully.\n", exportedCount);

    return true;
}


bool clearBlacklistTable()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    const char *query = "TRUNCATE TABLE blacklist_combinations";

    if (mysql_query(conn, query))
    {
        fprintf(stderr, "Failed to clear blacklist table: %s\n", mysql_error(conn));
        return false;
    }

    printf("Blacklist table cleared successfully.\n");
    return true;
}

bool importBlacklistCombinationsFromCSV()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    const char *filepath = "results/knowledge/blacklist_combinations.csv";

    struct stat buffer;
    if (stat(filepath, &buffer) != 0)
    {
        printf("No blacklist_combinations.csv file found — skipping import.\n");
        return true; // Not an error, just skip loading
    }

    FILE *fp = fopen(filepath, "r");
    if (!fp)
    {
        fprintf(stderr, "Failed to open %s for reading.\n", filepath);
        return false;
    }

    char line[1024];
    int importedCount = 0;

    while (fgets(line, sizeof(line), fp))
    {
        // Remove newline and carriage return
        line[strcspn(line, "\r\n")] = 0;

        // Skip empty lines
        if (strlen(line) == 0)
            continue;

        // Remove surrounding double quotes if they exist
        size_t len = strlen(line);
        if (len > 1 && line[0] == '"' && line[len - 1] == '"')
        {
            memmove(line, line + 1, len - 2);
            line[len - 2] = '\0';
        }

        // Escape single quotes for SQL safety
        char escaped[2048];
        mysql_real_escape_string(conn, escaped, line, strlen(line));

        char query[4096];
        snprintf(query, sizeof(query),
                 "INSERT INTO blacklist_combinations (combination) VALUES ('%s') "
                 "ON DUPLICATE KEY UPDATE combination = VALUES(combination)",
                 escaped);

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "Insert failed for combination '%s': %s\n",
                    line, mysql_error(conn));
        }
        else
        {
            importedCount++;
        }
    }

    fclose(fp);
    printf("Imported %d blacklist combinations successfully from CSV (quotes removed).\n", importedCount);
    return true;
}

bool importWorkloadCacheFromCSV()
{
    if (!conn)
    {
        fprintf(stderr, "Database connection not initialized!\n");
        return false;
    }

    FILE *fp = fopen("results/knowledge/workload_cache_export.csv", "r");
    if (!fp)
    {
        fprintf(stderr, "No workload_cache_export.csv file found — skipping import.\n");
        return false;
    }

    char line[2048];
    bool firstLine = true;
    int lineNum = 0;

    while (fgets(line, sizeof(line), fp))
    {
        lineNum++;
        // Remove newline characters
        line[strcspn(line, "\r\n")] = 0;

        // Skip header line
        if (firstLine)
        {
            firstLine = false;
            continue;
        }

        if (strlen(line) == 0)
            continue;

        // Find the boundaries of the combination field (between the first and second quotes)
        char *first_quote = strchr(line, '"');
        if (!first_quote)
        {
            fprintf(stderr, "No opening quote found on line %d: %s\n", lineNum, line);
            continue;
        }

        char *second_quote = strchr(first_quote + 1, '"');
        if (!second_quote)
        {
            fprintf(stderr, "No closing quote found for combination on line %d: %s\n", lineNum, line);
            continue;
        }

        // Extract combination (between the quotes)
        size_t combo_len = second_quote - first_quote - 1;
        char combination[256] = {0};
        if (combo_len < sizeof(combination))
        {
            strncpy(combination, first_quote + 1, combo_len);
            combination[combo_len] = '\0';
        }
        else
        {
            fprintf(stderr, "Combination too long on line %d\n", lineNum);
            continue;
        }

        // The remaining fields are after the second quote: ",avg_response_time,error_rate
        char *remaining_fields = second_quote + 2; // Skip the quote and the following comma

        // Parse the remaining 2 fields
        char *avg_rt_str = strtok(remaining_fields, ",");
        char *error_rate_str = strtok(NULL, ",");

        if (!avg_rt_str || !error_rate_str)
        {
            fprintf(stderr, "Could not parse remaining fields on line %d: %s\n", lineNum, line);
            continue;
        }

        printf("Line %d - combination: '%s', avg_rt: '%s', error_rate: '%s'\n",
               lineNum, combination, avg_rt_str, error_rate_str);

        // Insert into database
        char query[1024];
        snprintf(query, sizeof(query),
                 "INSERT INTO workload_cache (combination, avg_response_time, error_rate) "
                 "VALUES ('%s', %f, %f) "
                 "ON DUPLICATE KEY UPDATE avg_response_time = VALUES(avg_response_time), "
                 "error_rate = VALUES(error_rate)",
                 combination, atof(avg_rt_str), atof(error_rate_str));

        if (mysql_query(conn, query))
        {
            fprintf(stderr, "Insert failed for combination '%s': %s\n",
                    combination, mysql_error(conn));
        }
        else
        {
            printf("Successfully inserted combination: %s\n", combination);
        }
    }

    fclose(fp);
    printf("Workload cache import completed.\n");
    return true;
}

void exportWorkloadCacheToCSV()
{
    FILE *fp = fopen("results/knowledge/workload_cache_export.csv", "w");
    if (!fp)
    {
        perror("File open failed");
        return;
    }

    if (mysql_query(conn, "SELECT combination, avg_response_time, error_rate FROM workload_cache"))
    {
        fprintf(stderr, "SELECT failed: %s\n", mysql_error(conn));
        fclose(fp);
        return;
    }

    MYSQL_RES *res = mysql_store_result(conn);
    if (!res)
    {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        fclose(fp);
        return;
    }

    // Write header manually
    fprintf(fp, "combination,avg_response_time,error_rate\n");

    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)))
    {
        fprintf(fp, "\"%s\",%s,%s\n",
                row[0] ? row[0] : "",
                row[1] ? row[1] : "0",
                row[2] ? row[2] : "0");
    }

    mysql_free_result(res);
    fclose(fp);
    printf("CSV exported successfully (no id, no created_at).\n");
}

// Testing specific combinition
void testingCombinition()
{
    int arr[] = {0, 0, 1, 0, 0, 0}; // Build testing combinition
    for (int i = 0; i < num_transactions; i++)
    {
        transactions[i].workLoad = arr[i];
    }
}

void recordAndPrintAllSolutions()
{
    noOfSolutionsDiscovered++;

    solutionsDiscovered[noOfSolutionsDiscovered - 1][0] = currentTotalWorkload;

    for (int i = 1; i <= num_transactions; i++)
    {
        solutionsDiscovered[noOfSolutionsDiscovered - 1][i] = transactions[i - 1].workLoad;
    }

    //Temporary stop printing all solutions to avoid console clutter, can be enabled for debugging
    /*for (int i = 0; i < noOfSolutionsDiscovered; i++)
    {
        std::cout << "Solution # " << i + 1 << "=";
        for (int j = 0; j <= num_transactions; j++)
        {
            std::cout << solutionsDiscovered[i][j] << ",";
        }
        std::cout << "\n";
    }*/
}

void updateKnowledge()
{
    cfWritingFilePointer = fopen(outputFile2, "w"); // open outputFile2 in writing mode

    if (cfWritingFilePointer == NULL)
    {
        printf("Error in opening a file.. %s", outputFile2);
        exit(1);
    }

    fileClassifierPopulation = fopen(outputFile3, "w"); // open outputFile3 in writing mode
    if (fileClassifierPopulation == NULL)
    {
        printf("Error in opening a file.. %s", outputFile3);
        exit(1);
    }

    population = sortClassifierSet(&population, 2); // sort according to 'fitness'
    fprintClassifierSet(fileClassifierPopulation, cfWritingFilePointer, population);

    fclose(cfWritingFilePointer);
    fclose(fileClassifierPopulation);
    if (cfReadingFilePointer != NULL)
        fclose(cfReadingFilePointer);
}

void resetActionsTried()
{
    actionsTried = 0;
    for (int i = 0; i < numActions; i++)
    {
        actionTried[i] = 0;
    }
}

void displayTriedActions()
{
    std::cout << "Actions tried=" << actionsTried << ":{";
    for (int i = 0; i < numActions; i++)
    {
        if (actionTried[i] == 1)
        {
            std::cout << i << ",";
        }
    }
    std::cout << "}  ";
}

bool isActionTried(int tranNo)
{
    if (actionTried[transactionNo] == 1)
    {
        solSteps--;
        episodStep--;
        std::cout << "\nSkipped----As already checked\n";
        return true;
    }

    actionTried[transactionNo] = 1;
    actionsTried++;

    return false;
}

void handleSolutionFound()
{
    preStatesCount = 0;

    exportBlacklistCombinationsToCSV();
    //exportWorkloadCacheToCSV();

    updateKnowledge();

    recordAndPrintAllSolutions();

    writeLogCSV(solutionsLogFile, episod, currentTotalWorkload, solSteps, current_AvgRT, current_ER);

    solSteps = 0;

    prev_AvgRT_int = 0;

    initEnviournment();
}

void updateObservations()
{
    observations[0] = currentTotalWorkload;
    for (int i = 0; i < num_transactions; i++)
    {
        observations[i + 1] = transactions[i].workLoad;
    }
}

int consecutiveFalse = 0;

bool executeActionAndGetObservations(int act)
{
    isActCorrect = false;

    // build combinition as per suggested action

    //currentTotalWorkload = 0;

    if (episod > prevEpisode)
    {
        initEnviournment();
        prevEpisode = episod;
    }

    printf("============================================================================================\n");

    transactionNo = allPossibleActions[act];

    if (isActionTried(transactionNo))
    {
        reward = -maxPayoff;
        return false;
    }

    // Increase worklaod and calculate total workload
    printf("T No: %02d  ", transactionNo);

    transactions[transactionNo].workLoad += workloadChangeBy;
    currentTotalWorkload = currentTotalWorkload + 1;

    if (combinationExistsInBlacklist())
    {
        transactions[transactionNo].workLoad -= workloadChangeBy;
        currentTotalWorkload = currentTotalWorkload - 1;

        updateObservations();

        resetActionsTried();

        reward = -maxPayoff;

        return true;
    }

    std::cout << "CTW=" << currentTotalWorkload << "  ";

    std::cout << "<Epoch,Total Steps, Current solution steps, tempSteps>=<" << episod << " , " << episodStep << " , " << solSteps << " , " << tempSteps << ">";

    displayTriedActions();

    // testingCombinition(); //This function is to test specific combinition

    cachedRT = getCachedResponseTime();

    if (cachedRT >= 0)
    {
        current_AvgRT = cachedRT;
    }
    else
    {
    checkPreviousBeatAgain:

        for (int i = 0; i < num_transactions; i++)
        {
            rampUpTime[i] = transactions[i].workLoad / threadsPerSecond;
        }

        std::cout << "\n";

        updateThreadgroups(transactions, num_transactions, rampUpTime, loops, jmxFile);

        std::tuple<double, double> stats1;

        double firstRT = 0;

	/*for (int i = 0; i < 3; i++)
        {
            std::cout << "Warm-up run: " << i+1 << std::endl;
            runJMeterAndGetStats();  // Run without recording stats to warm up the system
        }*/

    repeatExp:
        double rt = 0;
        double er = 0;

        for (int i = 1; i <= repeatTimes; i++)
        {
            stats1 = runJMeterAndGetStats();

            rt += std::get<0>(stats1);
            er += std::get<1>(stats1);
        }


        current_AvgRT = rt / repeatTimes;

        if ((current_AvgRT - prev_AvgRT) >= -toleranceForNoise && crossCheckRT == false)
        {
            std::cout << "------------<first,second>:<" << current_AvgRT << ",";
            firstRT = current_AvgRT;
            crossCheckRT = true;
            goto repeatExp;
        }
        if (crossCheckRT)
        {
            std::cout << current_AvgRT << ">------------\n";
        }

        if (current_AvgRT > firstRT && crossCheckRT == true)
        {
            current_AvgRT = firstRT;
        }

        crossCheckRT = false;
    }

    if (checkPreviousBeatAgain == true && current_AvgRT < preRT)
    {
        std::cout << "\n-----Previous ART was incorrect-----\n";

        preRT = current_AvgRT;
        prev_AvgRT = current_AvgRT;
        prev_AvgRT_int = current_AvgRT;
        checkPreviousBeatAgain = false;

        solSteps -= tempSteps;
        episodStep -= tempSteps;

        tempSteps = 0;
    }
    else if (checkPreviousBeatAgain == true)
    {
        std::cout << "\n-----Previous of pevious best combination is set-----\n";

        for (int i = 0; i < num_transactions; i++)
        {
            transactions[i].workLoad = prepreTransactionsState[i];
        }

        prev_AvgRT = prepreRT;

        preRT = prepreRT;

        checkPreviousBeatAgain = false;

        tempSteps = 0;
    }

    std::cout << "<Current ART,Previous ART,Objective RT>:<" << current_AvgRT << " , " << prev_AvgRT << " , " << testObjective1_AvgRT << ">";

    std::string trimmedName = trimThreadGroup(transactions[transactionNo].name);

    if ((current_AvgRT - prev_AvgRT) >= -toleranceForNoise /*&& verifyBeatPrevRT() && current_AvgRT>=maxAchievements[currentTotalWorkload].avgResponseTime*/)
    {
        consecutiveFalse = 0;

        //insertCombinationInDB(current_AvgRT, 0.0);
        logCombination(current_AvgRT, 0.0);
        
        tempSteps = 0;

        resetActionsTried();

        for (int i = 0; i < num_transactions; i++)
        {
            prepreTransactionsState[i] = preTransactionsState[preStatesCount][i];

            preTransactionsState[preStatesCount][i] = transactions[i].workLoad;
        }

        actual_AvgRT = current_AvgRT;

        prepreRT = preRT;

        preRT = current_AvgRT;

        prepreAction = preAction;

        preAction = transactionNo;

        pre_beat[preStatesCount] = prev_AvgRT;

        preStatesCount++;

        updateObservations();

        if (current_AvgRT < prev_AvgRT)
        {
            // current_AvgRT = prev_AvgRT;
            reward = 10;
        }
        else
        {
            reward = 10 + (100 * (current_AvgRT - prev_AvgRT));
        }

        writeFullLogCSV(iterationsLogFile, episod, solSteps, exploit, elapsed_time_ms, noOfSolutionsDiscovered + 1, trimmedName, reward, actual_AvgRT, current_ER, currentTotalWorkload);

        prev_AvgRT = current_AvgRT;

        isActCorrect = true;
    }
    else
    {
        reward = 100 * (current_AvgRT - prev_AvgRT);

        //insertCombinationInDB(current_AvgRT, 0.0);
        logCombination(current_AvgRT, 0.0);

        //tempSteps++;

        writeFullLogCSV(iterationsLogFile, episod, solSteps, exploit, elapsed_time_ms, noOfSolutionsDiscovered + 1, trimmedName, reward, current_AvgRT, current_ER, currentTotalWorkload);

        transactions[transactionNo].workLoad -= workloadChangeBy;
        currentTotalWorkload = currentTotalWorkload - 1;

        updateObservations();

        consecutiveFalse++;

        /*if(consecutiveFalse == 25)
        {
            consecutiveFalse = 0;
            resetWorkloadtoZero(transactions, num_transactions);
            prev_AvgRT = 0;
            episodStep = 1;
            prev_AvgRT_int = 0;
            solSteps = 0;
        }*/

        if (actionsTried >= actionsTriedTh || consecutiveFalse == 15)
        {
            insertIntoBlacklist();
            
            preStatesCount -= 2;

            currentTotalWorkload = 0;
            for (int i = 0; i < num_transactions; i++)
            {
                transactions[i].workLoad = preTransactionsState[preStatesCount][i];
                currentTotalWorkload = currentTotalWorkload + transactions[i].workLoad;
            }
            updateObservations();
            

            // prev_AvgRT = preRT;

            prev_AvgRT = pre_beat[preStatesCount];

            // testing start
            /* consecutiveFalse = 0;
                resetWorkloadtoZero(transactions, num_transactions);
                prev_AvgRT = 0;
                episodStep = 1;
                prev_AvgRT_int = 0;
                solSteps = 0;
            */
            // testing end

            // Added to resolve repitition issue

            reward = -maxPayoff;

            consecutiveFalse = 0;
            resetActionsTried();

            return false;

            // Commented to resolve repitition issue
            //  checkPreviousBeatAgain = true;
            //  goto checkPreviousBeatAgain;
        }
    }

    std::cout << "\nReward = " << reward;

    if (current_AvgRT >= testObjective1_AvgRT) // Check-if solution found
    {
        handleSolutionFound();
    }

    std::cout << "    Solutions found=" << noOfSolutionsDiscovered << "\n";

    return isActCorrect;
}
