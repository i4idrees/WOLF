#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert> // Replaced <assert.h> with <cassert> (C++ standard header)
#include <cmath>   // Replaced <math.h> with <cmath> (C++ standard header)
#include <cstring>
#include <string>
// #include <windows.h>
#include <utility>
#include <unordered_map>
#include <cctype>
#include <tuple>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <chrono>


#include "time.h"
#include "configuration.h"

//#include "xcsMacros.h"
//#include "classifier.h"
// #include "codeFragment.h"
#include "xcscfc/xcscfc_api.h"


#include "env.h"
#include "xcs.h"

#include "json.hpp"
using json = nlohmann::json;

#include <regex>

using namespace std;

// Start: Set these from file
int maxEpochs;
double epsilon;
float testObjective1_AvgRT;
float testObjective2_ER;

int performanceWriteCount = 1;

int testing;
int testFrequency;
// End: Set these from file

bool exploit = false;

string SUT;

int episod = 1;
int episodStep = 1;

int solSteps = 1;

ClassifierSet *population;

int goal = 0;
int exploreProbC = 0;
extern float reward;

inline string resetDBFile;

// extern const unsigned int noOfObservations;
// extern float observations[];

int currentProblemLevel;
int clfrCondLength = 7; // 32;// 12 6 4 3

extern int currentTotalWorkload;

clock_t start_time = 0;
clock_t end_time = 0;
double elapsed_time_ms = 0;

// extern const int num_transactions=12;
// extern Transaction transactions[num_transactions];
extern int threadPerSecond;
extern int totalWorkload;
extern int rampUpTime[maxNum_transactions];

extern int flag;

extern float current_AvgRT;
extern float current_ER;

char path[100];
char outputFile[512];
char outputFile2[512];
char outputFile3[512];

/**************************** Single Step Experiments ***************************/


void doOneSingleStepProblemExplore(ClassifierSet **population, float *state, int counter) // Executes one main learning loop for a single step problem.
{
    std::cout << "\n*****Explored*****\n";

    exploit = false;

    bool wasCorrect = false;
    ClassifierSet *mset, *aset, *killset = NULL;

    start_time = clock(); // Start timing

    mset = getMatchSet(population, &killset, state, counter);
    freeSet(&killset);

    getPredictionArray(mset);
    int actionWinner = randomActionWinner();
    aset = getActionSet(actionWinner, mset);

    end_time = clock(); // End timing

    // Convert to milliseconds
    elapsed_time_ms = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
    printf("Execution time: %.2f ms\n", elapsed_time_ms);

    Environment(actionWinner);

    updateActionSet(&aset, 0.0, reward, population, &killset);
    freeSet(&killset);

    discoveryComponent(&aset, population, &killset, counter, state);

    freeSet(&killset);
    freeSet(&mset);
    freeSet(&aset);
}


void doOneSingleStepProblemExploit(ClassifierSet **population, float *state, int counter, int correct[], double sysError[]) // Executes one main performance evaluation loop for a single step problem.
{

    std::cout << "\n*****Exploit*****\n";

    exploit = true;

    int wasCorrect = 0;
    ClassifierSet *mset, *killset = NULL;

    start_time = clock(); // Start timing

    mset = getMatchSet(population, &killset, state, counter);
    freeSet(&killset);

    getPredictionArray(mset);
    int actionWinner = bestActionWinner();

    end_time = clock(); // End timing

    // Convert to milliseconds
    elapsed_time_ms = (double)(end_time - start_time) * 1000.0 / CLOCKS_PER_SEC;
    // printf("Execution time: %.2f ms\n", elapsed_time_ms);

    wasCorrect = Environment(actionWinner);

    if (wasCorrect)
        correct[counter % testFrequency] = 1;
    else
        correct[counter % testFrequency] = 0;

    sysError[counter % testFrequency] = absoluteValue(reward - getBestValue());

    freeSet(&mset);
}

/*void doOneSingleStepExperiment(ClassifierSet **population)  //Executes one single-step experiment monitoring the performance.
{
    int explore = 0;
    int correct[testFrequency];
    double sysError[testFrequency];

    exploreProbC = 0;

    for(exploreProbC = 0; exploreProbC <= maxProblems; exploreProbC += explore)
    {
        explore = (explore+1)%2;


        if(explore == 1) doOneSingleStepProblemExplore(population, observations, exploreProbC);
        else doOneSingleStepProblemExploit(population, observations, exploreProbC, correct, sysError);

        if(exploreProbC%testFrequency == 0 && explore == 0 && exploreProbC > 0)
            writePerformance(*population, correct, sysError, exploreProbC);

        checkConvergance();

        //if(exploreProbC % 1000 == 0 && explore == 1) std::cout<<exploreProbC<<":";
    }
}*/

void doOneSingleStepExperiment(ClassifierSet **population) // Executes one single-step experiment monitoring the performance.
{
    int explore = 0;
    int correct[testFrequency];
    double sysError[testFrequency];

    // double epsilon = 1.0;  // Initial epsilon
    // int totalEpisodes = maxProblems; // Total number of episodes

    for (exploreProbC = 0; exploreProbC <= maxProblems; exploreProbC += explore)
    {
        if (epsilon == .2)
        {
            if ((double)rand() / RAND_MAX < epsilon)
            {
                doOneSingleStepProblemExplore(population, observations, exploreProbC);
                explore = 1;
            }
            else
            {
                doOneSingleStepProblemExploit(population, observations, exploreProbC, correct, sysError);
                explore = 0;
            }
        }
        else if (epsilon == .5)
        {
            explore = (explore + 1) % 2;
            if (explore == 1)
                doOneSingleStepProblemExplore(population, observations, exploreProbC);
            else
                doOneSingleStepProblemExploit(population, observations, exploreProbC, correct, sysError);
        }
        else if (epsilon == .8)
        {
            if ((double)rand() / RAND_MAX < epsilon)
            {
                doOneSingleStepProblemExplore(population, observations, exploreProbC);
                explore = 1;
            }
            else
            {
                doOneSingleStepProblemExploit(population, observations, exploreProbC, correct, sysError);
                explore = 0;
            }
        }
        else if (epsilon == 1.0)
        {
            epsilon = 1.0 - (double)episod / maxEpochs;

            std::cout << "Epsilon = " << epsilon << "\n";

            if ((double)rand() / RAND_MAX < epsilon)
            {
                doOneSingleStepProblemExplore(population, observations, exploreProbC);
                explore = 1;
            }

            else
            {
                doOneSingleStepProblemExploit(population, observations, exploreProbC, correct, sysError);
                explore = 0;
            }
            epsilon = 1.0;
        }
        else if (epsilon == 0)
        {
            epsilon = 0;

            std::cout << "Epsilon = " << epsilon << "\n";

            if ((double)rand() / RAND_MAX < epsilon)
            {
                doOneSingleStepProblemExplore(population, observations, exploreProbC);
                explore = 1;
            }
            else
            {
                doOneSingleStepProblemExploit(population, observations, exploreProbC, correct, sysError);
                explore = 0;
            }
        }

        std::cout << "\n---Write performance after " << (testFrequency * performanceWriteCount) - exploreProbC << " iterations---\n";
        if (exploreProbC % testFrequency == 0 && explore == 0 && exploreProbC > 0)
        {
            performanceWriteCount++;
            writePerformance(*population, correct, sysError, exploreProbC);
        }

        episodStep++;
        solSteps++;
        // checkConvergance();
    }
}

void doOneSingleStepTest(ClassifierSet *population)
{
    int cc = 0;
    int wasCorrect;
    int correctCounter = 0;
    int tmpnotmatched = 0;
    int tmpcorrectcounter = 0;
    int correct[testNumInstances];
    double sysError[testFrequency];
    int TI = 0, TD = 0, TM = 0, FI = 0, FD = 0, FM = 0;

    std::ofstream resFile;
    std::string wLine = "";
    std::string wLine2 = "";

    for (int t = 0; t < testNumInstances; t++)
    {
        ClassifierSet *mset = NULL, *poppointer;
        bool isMatched = false;

        for (poppointer = population; poppointer != NULL; poppointer = poppointer->next)
        {
            if (isConditionMatched(poppointer->classifier->condition, observations))
            {
                isMatched = true;
                tmpcorrectcounter++;
                addNewClassifierToSet(poppointer->classifier, &mset); // add matching classifier to the matchset
            }
        }
        if (isMatched == false)
        {
            int i = 0;
            int mK = 0, mT = 0;
            cc++;
            tmpnotmatched++;
            int popSize = getSetSize(population);
            distanceInputClassifier distanceArray[popSize];
            int k = popSize * tournamentSize;
            assert(k > 0); // make sure that k > 0

            for (poppointer = population; poppointer != NULL; poppointer = poppointer->next)
            {
                distanceArray[i].posClassifier = i;
                distanceArray[i].distance = computeDistance(poppointer->classifier->condition, observations);
                i++;
            }
            sortAll(distanceArray, popSize);
            sortK(distanceArray, k);
            for (poppointer = population; poppointer != NULL && mK < k; poppointer = poppointer->next, mT++)
            {
                if (distanceArray[mK].posClassifier == mT)
                {
                    addNewClassifierToSet(poppointer->classifier, &mset); // add classifier to the matchset
                    mK++;
                }
            }
        }

        getPredictionArray(mset);
        int actionWinner = bestActionWinner();
        wasCorrect = Environment(actionWinner);
        sysError[t % testFrequency] = absoluteValue(reward - getBestValue());

        // float error = computeError(computedSaliencyMap,state.output);
        if (wasCorrect == 1)
        {
            if (actionWinner == 0)
                TM++;
            else if (actionWinner == 1)
                TI++;
            else if (actionWinner == 2)
                TD++;

            correctCounter++;
            correct[t % testFrequency] = 1;
        }
        else
        {
            if (actionWinner == 0)
                FM++;
            else if (actionWinner == 1)
                FI++;
            else if (actionWinner == 2)
                FD++;
        }
        freeSet(&mset);
    }

    std::cout << std::endl
              << ":" << correctCounter;
    std::cout << "TI(" << TI << "), TD(" << TD << ") TM(" << TM << "), FI(" << FI << "), FD(" << FD << "), FM(" << FM << ")";

    char resultfile1[512];
    sprintf(resultfile1, "testing_%d_%d.txt", goal, currentProblemLevel);

    resFile.open(resultfile1, std::ios::app);
    wLine = "\n" + NumberToString(correctCounter) + " " + NumberToString(TI) + " " + NumberToString(FI) + " " + NumberToString(TD) + " " + NumberToString(FD) + " " + NumberToString(TM) + " " + NumberToString(FM) + "\n";
    resFile << wLine;
    resFile.close();
}

/*void checkConvergance()
{
    if(testObjective1_AvgRT-current_AvgRT <= 0 || testObjective2_ER-current_ER <= 0)//If agent converge
    {
        noOfSolutionsDiscovered++;
        episod++;
        episodStep=1;

        solutionsDiscovered[noOfSolutionsDiscovered-1][0] = currentTotalWorkload;

        for(int i=1; i<=12; i++)
        {
            solutionsDiscovered[noOfSolutionsDiscovered-1][i] = transactions[i-1].workLoad;
        }

        for(int i=0; i<noOfSolutionsDiscovered; i++)
        {
            std::cout<<"Solution # "<<i+1<<"=";
            for(int j=0; j<13; j++)
            {
                std::cout<<solutionsDiscovered[i][j]<<",";
            }
            std::cout<<"\n";
        }
        resetWorkloadtoZero(transactions,num_transactions);
        observations[0]=0;
        observations[1]=0;
        observations[2]=0;

        population = sortClassifierSet(&population, 2); // sort according to 'fitness'
        fprintClassifierSet(fileClassifierPopulation, cfWritingFilePointer, population);
        //freeClassifierSet(&population); // free population for this experiment


        fclose(filePerformance);
        fclose(cfWritingFilePointer);
        fclose(fileClassifierPopulation);
        if(cfReadingFilePointer!= NULL) fclose(cfReadingFilePointer);


        if(noOfSolutionsDiscovered == maxEpochs)
        {
            population = sortClassifierSet(&population, 2); // sort according to 'fitness'
            fprintClassifierSet(fileClassifierPopulation, cfWritingFilePointer, population);
            freeClassifierSet(&population); // free population for this experiment


            fclose(filePerformance);
            fclose(cfWritingFilePointer);
            fclose(fileClassifierPopulation);
            if(cfReadingFilePointer!= NULL) fclose(cfReadingFilePointer);

            exit(0);
        }

        CreateFiles(noOfSolutionsDiscovered+1);
    }
}*/

/*void readConfig(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open config file: " + filename);
    }

    json j;
    file >> j;

    // ----- Transactions -----
    num_transactions = j["num_transactions"].get<int>();
    if (num_transactions > maxNum_transactions)
        throw std::runtime_error("num_transactions exceeds maxNum_transactions");

    for (int i = 0; i < num_transactions; i++)
    {
        std::string temp = j["transactions"][i]["name"].get<std::string>();
        strncpy(transactions[i].name, temp.c_str(), sizeof(transactions[i].name) - 1);
        transactions[i].name[sizeof(transactions[i].name) - 1] = '\0'; // ensure null-terminated
        transactions[i].workLoad = j["transactions"][i]["workLoad"].get<int>();
    }

    // ----- Actions -----
    numActions = j["numActions"].get<int>();
    if (numActions > maxNumActions)
        throw std::runtime_error("numActions exceeds MAX_ACTIONS");

    for (int ii = 0; ii < numActions; ii++)
    {
        allPossibleActions[ii] = j["allPossibleActions"][ii].get<int>();
    }
    workloadChangeBy = j["workloadChangeBy"].get<int>();
    workloadChangeType = j["workloadChangeType"].get<int>();

    // ----- Other variables -----
    condLength = j["condLength"].get<int>();
    //maxPayoff = j["maxPayoff"].get<double>();
    maxEpochs = j["MaxEpochs"].get<int>();
    epsilon = j["Epsilon"].get<double>();
    testObjective1_AvgRT = j["ARTThreshhold"].get<double>();
    testObjective2_ER = j["ERThreshhold"].get<double>();
    Testing = j["Testing"].get<int>();
    testFrequency = j["testFrequency"].get<int>();
}*/

void readConfig(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open config file: " + filename);
    }

    json j;
    file >> j;

    // ----- Transactions -----
    // Calculate num_transactions from transactions array size
    SUT = j["SUT"].get<std::string>();
    num_transactions = j["transactions"].size();
    if (num_transactions > maxNum_transactions)
        throw std::runtime_error("num_transactions exceeds maxNum_transactions");

    for (int i = 0; i < num_transactions; i++)
    {
        std::string temp = j["transactions"][i].get<std::string>();
        // Append "_thread_group" to each transaction name
        std::string fullName = temp + "_thread_group";
        strncpy(transactions[i].name, fullName.c_str(), sizeof(transactions[i].name) - 1);
        transactions[i].name[sizeof(transactions[i].name) - 1] = '\0'; // ensure null-terminated
        transactions[i].workLoad = 0;                                  // Set workload to 0 as requested
    }

    // ----- Actions -----
    // Calculate numActions from allPossibleActions array size
    numActions = j["allPossibleActions"].size();
    if (numActions > maxNumActions)
        throw std::runtime_error("numActions exceeds MAX_ACTIONS");

    for (int ii = 0; ii < numActions; ii++)
    {
        allPossibleActions[ii] = j["allPossibleActions"][ii].get<int>();
    }

    workloadChangeBy = j["workloadChangeBy"].get<int>();
    workloadChangeType = j["workloadChangeType"].get<int>();

    // ----- Other variables -----
    // Calculate condLength as num_transactions + 1
    condLength = num_transactions + 1;

    maxEpochs = j["MaxEpochs"].get<int>();
    epsilon = j["Epsilon"].get<double>();
    testObjective1_AvgRT = j["ARTThreshhold"].get<double>();
    testObjective2_ER = j["ERThreshhold"].get<double>();
    Testing = j["Testing"].get<int>();
    testFrequency = j["testFrequency"].get<int>();
}

void backupAndCleanResults()
{
    // Create the backup folder name with variables (including SUT string)
    std::string backupFolderName = "SUT=" + SUT + "--ART=" + std::to_string(testObjective1_AvgRT) + "---Population=" + std::to_string(maxPopSize);
    std::string backupPath = "Results_backup/" + backupFolderName;

#ifdef _WIN32
    // Windows commands
    // Create backup directory and copy Results
    std::string cmd1 = "if not exist Results_backup mkdir Results_backup";
    std::string cmd2 = "if not exist \"" + backupPath + "\" mkdir \"" + backupPath + "\"";
    std::string cmd3 = "xcopy /E /I /Y Results \"" + backupPath + "\" >nul 2>&1";

    system(cmd1.c_str());
    system(cmd2.c_str());
    system(cmd3.c_str());

    // Clean Results folder - delete everything and recreate with knowledge folder
    system("rmdir /s /q Results");
    system("mkdir Results");
    system("mkdir Results\\knowledge");

    std::cout << "Results backed up to " << backupPath << " and cleaned\n";
#else
    // Linux/Mac commands
    // Create backup directory and copy Results
    std::string cmd1 = "mkdir -p Results_backup";
    std::string cmd2 = "mkdir -p \"" + backupPath + "\"";
    std::string cmd3 = "cp -r Results/* \"" + backupPath + "/\"";

    system(cmd1.c_str());
    system(cmd2.c_str());
    system(cmd3.c_str());

    // Clean Results folder - delete everything and recreate with knowledge folder
    system("rm -rf Results");
    system("mkdir Results");
    system("mkdir Results/knowledge");

    std::cout << "Results backed up to " << backupPath << " and cleaned\n";
#endif
}

// Function to read all experiment configurations from JSON array
std::vector<json> readAllConfigs(const std::string &configPath)
{
    std::vector<json> configs;

    try
    {
        std::ifstream file(configPath);
        if (!file.is_open())
        {
            std::cerr << "Error: Could not open config file: " << configPath << std::endl;
            return configs;
        }

        json jsonData;
        file >> jsonData;

        for (const auto &item : jsonData)
        {
            configs.push_back(item);
        }

        std::cout << "Loaded " << configs.size() << " experiment configurations" << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error reading config file: " << e.what() << std::endl;
    }

    return configs;
}

// Function to apply configuration from JSON object
void applyConfig(const json &config)
{
    // Apply all configuration settings to your existing global variables
    SUT = config["SUT"].get<std::string>();

    // Set transactions
    num_transactions = config["transactions"].size();
    if (num_transactions > maxNum_transactions)
    {
        throw std::runtime_error("num_transactions exceeds maxNum_transactions");
    }

    for (int i = 0; i < num_transactions; i++)
    {
        std::string temp = config["transactions"][i].get<std::string>();
        std::string fullName = temp + "_thread_group";
        strncpy(transactions[i].name, fullName.c_str(), sizeof(transactions[i].name) - 1);
        transactions[i].name[sizeof(transactions[i].name) - 1] = '\0';
        transactions[i].workLoad = 0;
    }

    // Set actions
    numActions = config["allPossibleActions"].size();
    if (numActions > maxNumActions)
    {
        throw std::runtime_error("numActions exceeds MAX_ACTIONS");
    }

    for (int i = 0; i < numActions; i++)
    {
        allPossibleActions[i] = config["allPossibleActions"][i].get<int>();
    }

    workloadChangeBy = config["workloadChangeBy"].get<int>();
    workloadChangeType = config["workloadChangeType"].get<int>();

    // Set other variables
    condLength = num_transactions + 1;
    maxEpochs = config["MaxEpochs"].get<int>();
    epsilon = config["Epsilon"].get<double>();
    testObjective1_AvgRT = config["ARTThreshhold"].get<double>();
    testObjective2_ER = config["ERThreshhold"].get<double>();
    Testing = config["Testing"].get<int>();
    testFrequency = config["testFrequency"].get<int>();
    repeatTimes = config["repeatTimes"].get<int>();
    maxProblems = config["maxProblems"].get<int>();

    // Calculate derived values
    actionsTriedTh = (numActions * 95 + 50) / 100;
    maxPayoff = 10 + (100 * (testObjective1_AvgRT));

    if (SUT == "RUBIS")
    {
        resetDBFile = "reset-db-rubis.sh";
        jmxFile = "config/RUBiS-testplan/testplan.jmx";

        initDBConnectionRUBIS();
    }
    else if (SUT == "DVD")
    {
        resetDBFile = "reset-db-dvd.sh";
        jmxFile = "config/DVD-testplan/testplan.jmx";

        initDBConnectionDVD();
    }
    else if (SUT == "TEASTORE")
    {
        resetDBFile = "reset-db-teastore.sh";
        jmxFile = "config/teastore-testplan/testplan.jmx";

        initDBConnectionTEASTORE();
    }
}

void flushKnowledge(ClassifierSet **population) {

    // Safely delete classifier population
    if (*population != NULL) {
        ClassifierSet *current = *population;
        ClassifierSet *next;

        // Traverse and free each classifier node
        while (current != NULL) {
            next = current->next;
            // Free the classifier inside if dynamically allocated
            if (current->classifier != NULL)
                free(current->classifier);
            free(current);
            current = next;
        }

        *population = NULL;
        printf("Classifier population flushed.\n");
    }

    // If you don’t yet have previous code fragments or counters, skip them for now
    printf("All knowledge has been successfully flushed. Ready for a fresh start.\n");
}

void startXCS()
{
    std::cout << "XCS starts\n=================\n";
    for (int i = 1; i <= maxEpochs; i++)
    {
        episod = i;
        episodStep = 1;

        // initializePopulation(&population, cfReadingFilePointer);

        CreateFiles(i);

        doOneSingleStepExperiment(&population);

        population = sortClassifierSet(&population, 2); // sort according to 'fitness'
        fprintClassifierSet(fileClassifierPopulation, cfWritingFilePointer, population);
        // freeClassifierSet(&population); // free population for this experiment

        // fclose(filePerformance);
        fclose(cfWritingFilePointer);
        fclose(fileClassifierPopulation);
        if (cfReadingFilePointer != NULL)
            fclose(cfReadingFilePointer);


        backupAndCleanResults();
    }
    std::cout << "\n=================\nXCS ends";
}

int main(int argc, char **argv)
{
    // Read all experiment configurations
    std::vector<json> experiments = readAllConfigs("config/config.json");

    if (experiments.empty())
    {
        std::cerr << "No experiments to run. Exiting." << std::endl;
        return 1;
    }

    int experimentCount = 1;
    for (const auto &config : experiments)
    {
        std::cout << "\n=== Starting Experiment " << experimentCount << "/" << experiments.size()
                  << " - SUT: " << config["SUT"].get<std::string>() << " ===" << std::endl;

        logHeaderWritten = 0;
        fullLogHeaderWritten = 0;

        episodStep = 1;
        solSteps = 1;

        try
        {
            // Apply configuration to global variables
            applyConfig(config);

            //flushKnowledge(&population);

            // exportBlacklistCombinationsToCSV();

            importWorkloadCacheFromCSV();
            //importBlacklistCombinationsFromCSV();

            //clearBlacklistTable();



            // Reset workload for this experiment's transactions
            resetWorkload(0);

            // Start XCS for this configuration
            startXCS();

            std::cout << "=== Completed Experiment " << experimentCount << " - SUT: " << config["SUT"].get<std::string>() << " ===" << std::endl;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error running experiment " << experimentCount << ": " << e.what() << std::endl;
        }

        experimentCount++;
    }

    std::cout << "\nAll experiments completed!" << std::endl;
    getchar();
    exit(0);

    importWorkloadCacheFromCSV();
    importBlacklistCombinationsFromCSV();

    readConfig("config/config.json");

    actionsTriedTh = (numActions * 85 + 50) / 100;

    maxPayoff = 10 + (100 * (testObjective1_AvgRT));

    // Going to start the XCS
    resetWorkload(0);
    // resetWorkloadtoZero(transactions);

    startXCS();

    getchar();
    exit(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void header()
{
    time_t t;
    char hostName[] = "act-PC";
    time(&t);
    printf("%s\n Seed: %ld %s", hostName, getSeed(), ctime(&t));
} // end header

void Exit(FILE *fp)
{
    printf("\nShutting down...\n");
    elapsedTime();
    header();
    printf("Elapsed Time: ");
    elapsed(fp);
    printf(" Secs\n");
} // end exit

/*##########---- Output ----##########*/
/**
 * Writes the performance of the XCS to the specified file.
 * The function writes time performance systemError actualPopulationSizeInMacroClassifiers.
 * Performance and system error are averaged over the last fifty trials.
 *
 * @param performance The performance in the last fifty exploration trials.
 * @param sysError The system error in the last fifty exploration trials.
 * @param exploreProbC The number of exploration trials executed so far.
 */
void writePerformance(ClassifierSet *population, int performance[], double sysError[], int exploreProbC)
{
    filePerformance = fopen(outputFile, "a");
    if (filePerformance == NULL)
    {
        perror("Error opening performance file");
        return;
    }

    int setSize;
    char buf[1000];
    double perf = 0.0, serr = 0.0;
    for (int i = 0; i < testFrequency; i++)
    {
        perf += performance[i];
        serr += sysError[i];
    }
    perf /= testFrequency;
    serr /= testFrequency;
    setSize = getSetSize(population);

    snprintf(buf, strlen(buf), "%d ", exploreProbC);

    sprintf(buf, "%d ", exploreProbC);
    fwrite(buf, strlen(buf), 1, filePerformance);
    sprintf(buf, "%f ", perf);
    fwrite(buf, strlen(buf), 1, filePerformance);
    sprintf(buf, "%f ", serr);
    fwrite(buf, strlen(buf), 1, filePerformance);
    sprintf(buf, "%d ", setSize);
    fwrite(buf, strlen(buf), 1, filePerformance);
    fwrite("\n", strlen("\n"), 1, filePerformance);

    // int numerositySum = getNumerositySum(population); printf("%d %f %f %d %d\n",exploreProbC,perf,serr,setSize,numerositySum);
    // printf("%d %f %f %d\n",exploreProbC,perf,serr,setSize);

    fclose(filePerformance);
}

/*******************************Write Test Performance*************************************/
void writeTestPerformance(ClassifierSet *population, int performance[], double sysError[], int exploreProbC)
{
    char buf[100];
    double perf = 0.0, serr = 0.0;

    for (int i = 0; i < testFrequency; i++)
    {
        perf += performance[i];
        serr += sysError[i];
    }

    perf /= testFrequency;
    serr /= testFrequency;

    int setSize = getSetSize(population);

    sprintf(buf, "%d ", exploreProbC);
    fwrite(buf, strlen(buf), 1, testPerformance);
    sprintf(buf, "%f ", perf);

    fwrite(buf, strlen(buf), 1, testPerformance);
    sprintf(buf, "%d ", setSize);
    fwrite(buf, strlen(buf), 1, testPerformance);
    fwrite("\n", strlen("\n"), 1, testPerformance);

    // int numerositySum = getNumerositySum(population); printf("%d %f %f %d %d\n",exploreProbC,perf,serr,setSize,numerositySum);
    // printf("%d %f %f %d\n",exploreProbC,perf,serr,setSize);
}

void sortAll(distanceInputClassifier arrayToBeSorted[], int size) // Bubble Sort Function for Ascending Order
{
    int i, j;
    bool flag = true;             // set flag to true to start first pass
    distanceInputClassifier temp; // holding variable
    for (i = 1; i <= size && flag == true; i++)
    {
        flag = false;
        for (j = 0; j < size - 1; j++)
        {
            if (arrayToBeSorted[j + 1].distance < arrayToBeSorted[j].distance)
            {
                temp = arrayToBeSorted[j]; // swap elements
                arrayToBeSorted[j] = arrayToBeSorted[j + 1];
                arrayToBeSorted[j + 1] = temp;
                flag = true; // indicates that a swap occurred.
            }
        }
    }
    return; // arrays are passed to functions by address; nothing is returned
}

void sortK(distanceInputClassifier arrayToBeSorted[], int size) // Bubble Sort Function for Ascending Order
{
    int i, j;
    bool flag = true;             // set flag to true to start first pass
    distanceInputClassifier temp; // holding variable
    for (i = 1; i <= size && flag == true; i++)
    {
        flag = false;
        for (j = 0; j < size - 1; j++)
        {
            if (arrayToBeSorted[j + 1].posClassifier < arrayToBeSorted[j].posClassifier)
            {
                temp = arrayToBeSorted[j]; // swap elements
                arrayToBeSorted[j] = arrayToBeSorted[j + 1];
                arrayToBeSorted[j + 1] = temp;
                flag = true; // indicates that a swap occurred.
            }
        }
    }
    return; // arrays are passed to functions by address; nothing is returned
}

std::string NumberToString(int num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

// Original code
/*void CreateFiles(int j)
{
    char path[10] = "";
    char curLevel[2] = "";
    char prevLevel[2] = "";

    sprintf(curLevel, "%d", currentProblemLevel);
    sprintf(prevLevel, "%d", currentProblemLevel-1);

    sprintf(path, "%d", j);
    mkdir(path);
    setSeed(seeds[j]);

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

// Modified by Idrees
// for window
/*void CreateFiles(int j)
{

    strcpy(path, "");

    sprintf(path, "%d", j);

    mkdir(path);

    setSeed(seeds[j]);


    sprintf(outputFile,"%s\\%s.txt", path, outputFileName);
    filePerformance = fopen(outputFile, "a"); //open outputFile in writing mode
    if (filePerformance == NULL)
    {
        printf("Error in opening a file.. %s", outputFile);
        exit(1);
    }
    fclose(filePerformance);


    sprintf(outputFile2,"%s\\%s.txt", path, featureFileName);
    cfWritingFilePointer = fopen(outputFile2, "w"); //open outputFile2 in writing mode

    if(cfWritingFilePointer == NULL)
    {
        printf("Error in opening a file.. %s", outputFile2);
        exit(1);
    }


    sprintf(outputFile3,"%s\\%s.txt", path, ruleFileName);
    fileClassifierPopulation = fopen(outputFile3, "w"); //open outputFile3 in writing mode
    if (fileClassifierPopulation == NULL)
    {
        printf("Error in opening a file.. %s", outputFile3);
        exit(1);
    }

    /*if (currentProblemLevel > 1)
    {
        char inputFile[512];
        sprintf(inputFile,"%s\\%s%s.txt", path, featureFileName, prevLevel );
        cfReadingFilePointer = fopen(inputFile, "r"); //open inputFile in reading mode
        if(cfReadingFilePointer == NULL)
        {
            printf("Error in opening a file.. %s", inputFile);
            exit(1);
        }
    }*/
//}

// for linux
void CreateFiles(int j)
{

    // Step 1: Create directory path
    sprintf(path, "/app/src/results/knowledge/%d", j);

    // Step 2: Create directory (for Linux)
    mkdir(path, 0777);

    // Step 3: Set seed
    setSeed(seeds[j]);

    // Step 4: Create file 1
    sprintf(outputFile, "%s/%s.txt", path, outputFileName);
    filePerformance = fopen(outputFile, "a");
    if (filePerformance == NULL)
    {
        printf("Error in opening a file: %s\n", outputFile);
        exit(1);
    }
    fclose(filePerformance);

    // Step 5: Create file 2
    sprintf(outputFile2, "%s/%s.txt", path, featureFileName);
    cfWritingFilePointer = fopen(outputFile2, "w");
    if (cfWritingFilePointer == NULL)
    {
        printf("Error in opening a file: %s\n", outputFile2);
        exit(1);
    }

    // Step 6: Create file 3
    sprintf(outputFile3, "%s/%s.txt", path, ruleFileName);
    fileClassifierPopulation = fopen(outputFile3, "w");
    if (fileClassifierPopulation == NULL)
    {
        printf("Error in opening a file: %s\n", outputFile3);
        exit(1);
    }

    // Optional: read from previous level file
    /*
    if (currentProblemLevel > 1) {
        char inputFile[512];
        sprintf(inputFile, "%s/%s%s.txt", path, featureFileName, prevLevel);
        cfReadingFilePointer = fopen(inputFile, "r");
        if (cfReadingFilePointer == NULL) {
            printf("Error in opening a file: %s\n", inputFile);
            exit(1);
        }
    }
    */
}
