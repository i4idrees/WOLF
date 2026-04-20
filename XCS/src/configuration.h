#pragma once

/* ============================================================
 *  DATA STRUCTURES
 * ============================================================ */

typedef struct
{
    char name[256];
    int  workLoad;
} Transaction;

inline int transactionNo      = 0;

/* ============================================================
 *  CONFIGURATION START
 * ============================================================ */

inline int workloadChangeBy   = 0;
inline int workloadChangeType = 0;

inline int repeatTimes;

inline const char* jmxFile;

inline int  num_transactions;

inline const int maxNum_transactions = 6;
inline Transaction transactions[maxNum_transactions];

inline const unsigned int noOfObservations = 7;
inline float observations[noOfObservations];

inline const int maxNumActions = 6;
inline int numActions;

inline int actionsTriedTh;
inline int allPossibleActions[maxNumActions];

const int maxPopSize = 2000;

inline double maxPayoff;

inline int maxProblems; 

inline bool Testing = false;

/* =================== CONFIG END =================== */

const int totalNumInstances = 1;
const int testNumInstances  = 10000;
const int precisionDigits = 2;


/* ============================================================
 *  OUTPUT FILES
 * ============================================================ */

const char outputFileName[]  = "output_training";
const char featureFileName[] = "feature_codefragments";
const char ruleFileName[]    = "rule_with_codefragements";
const char resultFile[]      = "result_testing.txt";


/* =======================OUTPUT FILES END======================*/

const int cfMaxDepth  = 2;
const int cfMinDepth  = 0;
const int cfMaxLength = 8;   // pow(2,adfMaxDepth+1);
const int cfMaxArity  = 2;
const int cfMaxStack  = (cfMaxArity - 1) * (cfMaxDepth - 1) + 2;


typedef int opType;
const int opSize = sizeof(opType);

const opType OPNOP  = -100;   // ending symbol
const opType OPAND  = -101;
const opType OPOR   = -102;
const opType OPNAND = -103;
const opType OPNOR  = -104;
const opType OPNOT  = -105;
//const opType OPUNITY = -106;

const int totalFunctions = 5;
const opType functionCodes[] = {
    OPAND,
    OPOR,
    OPNAND,
    OPNOR,
    OPNOT
};

const int numLeaf = 4;


/* ============================================================
 *  CODE FRAGMENT STRUCTURES
 * ============================================================ */

struct Leaf
{
    opType featureNumber;
    float  lowerBound;
    float  upperBound;
};

struct CodeFragment
{
    opType codeFragment[cfMaxLength];
    Leaf   leaf[numLeaf];
    int    cfID;
};


/* ============================================================
 *  CLASSIFIER & DATASOURCE STRUCTURES
 * ============================================================ */

inline int condLength = 7;
inline const int maxCondLength = 7;

struct Classifier
{
    CodeFragment condition[maxCondLength];

    int    action;
    double prediction;
    double predictionError;
    double accuracy;
    double fitness;
    int    experience;
    int    numerosity;
    double actionSetSize;
    int    timeStamp;
    int    specificness;   // number of specific CFs
};

struct ClassifierSet
{
    Classifier    *classifier;
    ClassifierSet *next;
};


struct DataSource
{
    float *state;
    int    action;

    ~DataSource()
    {
        delete[] state;
    }
};
