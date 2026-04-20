#include "tinyxml2.h"
#include "configuration.h"

#define WINDOW_SIZE 10

float ScaleRange(float Value,float FromMinValue, float FromMaxValue, float ToMinValue,float ToMaxValue);
void updateRange(DataSource data[],int totalRows);
void initializeInput(DataSource inputArray[],int numofRows);
void resetStateTesting(DataSource &state, int index);
float roundRealValue(float val, int num);
void loadData(DataSource data[]);
void initializeLimit();
void loadDataFile();
void loadFileString();
void splitData(DataSource totalData[],DataSource trainData[], DataSource testData[]);
bool isIntegerInArray(int integer, int array[], int highestPosition);

int Environment(int action);
//int ExecuteAction(int act);
//int GetObservation();

//Start: Added by Idrees
typedef struct {
    double avgResponseTime;
    double errorRate;
    int transactionsWorkload[12];
}MaxAchievements;


/*typedef struct {
    char* name;
    int workLoad;
} Transaction;
*/

//double calculateReward(double ART, double ER);

void displayUpdatedTransactions(Transaction[], int);
void increaseWorkloadOfAll(Transaction transactions[], int num_transactions, int workloadModificatoins);
void increaseWorkloadOfParticular(Transaction transactions[], int num_transactions, const char* transaction_name, int workloadModificatoins);

void resetWorkload(int);
//void resetWorkloadtoZero(Transaction transactions[], int num_transactions);
//void resetWorkloadtoZero(std::vector<Transaction>& transactions);
//void resetWorkloadtoOne(Transaction transactions[], int num_transactions);
//void resetWorkloadtoThree(Transaction transactions[], int num_transactions);
//int calculateRampupTime(Transaction[],int, int);
//void update_thread_groups(Transaction transactions[], int num_transactions, int rampUpTime, int loops, const char* filename);

//Different rampup for each transaction
void update_thread_group_num_threads(tinyxml2::XMLElement* element, Transaction transactions[], int num_transactions, int loops, int rampTime[]);
void updateThreadgroups(Transaction[], int, int[],int,const char*);

//Same rampup for all transactions
/*void update_thread_group_num_threads(tinyxml2::XMLElement* element, Transaction transactions[], int num_transactions, int loops, int rampTime);
void updateThreadgroups(Transaction[], int, int,int, char*);*/


std::tuple<double, double> runJMeterAndGetStats();


//this return both avg response time and error rate
//std::tuple<double, double> runJMeterAndGetStats(std::string jmeterPath, std::string jmxFilePath);
//int GetObservations(Transaction transactions[], int numTransactions,std::string jmeterPath, char* jmxFilePath);

//void CreateFiles(int);

inline int logHeaderWritten = 0;
inline int fullLogHeaderWritten = 0;

void resetDB();

//float getOrInsertCombination(float, float);

void buildCombination(char*, size_t);

float getCachedResponseTime();

//bool insertCombination(float, float); 

bool logCombination(float, float);

bool executeActionAndGetObservations(int);

bool initDBConnectionRUBIS();
bool initDBConnectionDVD();
bool initDBConnectionTEASTORE();

bool importWorkloadCacheFromCSV();
void exportWorkloadCacheToCSV();

bool clearBlacklistTable();

bool importBlacklistCombinationsFromCSV();
bool exportBlacklistCombinationsToCSV();


bool combinationExistsInBlacklist();

bool insertIntoBlacklist();

//bool readEnvConfigurations();


