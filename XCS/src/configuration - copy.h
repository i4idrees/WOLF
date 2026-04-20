typedef struct {
    char name[256];
    int workLoad;
} Transaction;

inline int workloadChangeBy = 0;
inline int workloadChangeType = 0;
inline int transactionNo = 0;

// =================== CONFIG START ===================

//inline char* jmxFile = "config/RUBiS-testplan.jmx";
inline char* jmxFile;
inline int num_transactions;
inline const int maxNum_transactions = 25;
inline Transaction transactions[maxNum_transactions];
/*inline Transaction transactions[maxNum_transactions] = {
    {"HomePage_thread_group", 0},
    {"RegisterPage_thread_group", 0},
    {"RegisterUser_thread_group", 0},
    {"BrowsePage_thread_group", 0},
    {"BrowseInCategories_thread_group", 0},
    {"BrowseInRegions_thread_group", 0},
    {"SellPage_thread_group", 0},DVD-testplan
    {"SellItem_thread_group", 0},
    {"AboutMePage_thread_group", 0},
    {"AboutMeUser_thread_group", 0},
    {"BidOnItem_thread_group", 0},
    {"BuyItem_thread_group", 0},
};*/

/*inline const int num_transactions = 12;
inline std::vector<Transaction> transactions = {
    {"HomePage_thread_group", 0},
    {"RegisterPage_thread_group", 0},
    {"RegisterUser_thread_group", 0},
    {"BrowsePage_thread_group", 0},
    {"BrowseInCategories_thread_group", 0},
    {"BrowseInRegions_thread_group", 0},
    {"SellPage_thread_group", 0},
    {"SellItem_thread_group", 0},
    {"AboutMePage_thread_group", 0},
    {"AboutMeUser_thread_group", 0},
    {"BidOnItem_thread_group", 0},
    {"BuyItem_thread_group", 0},
};*/

inline const unsigned int noOfObservations = 13;
inline float observations[noOfObservations];

inline const int maxNumActions = 25;
inline int numActions;

inline int actionsTriedTh;

inline int allPossibleActions[maxNumActions];

const int maxPopSize = 2000;
//const int condLength = 13;
//inline int condLength = 13;
inline double maxPayoff;

// =================== CONFIG END ===================
inline int repeatTimes;

const int totalNumInstances = 1;
const int testNumInstances  = 10000;

//const int condLength        = 13;     //256; //4626; //for review analysis 300, 500, 1000, 1500, 2000, 2257
//const int maxPopSize        = 10000;    //.5 * totalNumInstances; //Specifies the maximal number of micro-classifiers in the population. [ (0.5, 1, 2, 5, 10/20, 50)*1000 for 6-, 11-, 20-, 37-, 70-, 135-bits MUX respectively]
inline int maxProblems;  //15 * totalNumInstances; //1*100*1000; //training set = [ (1, 1.5, 2, 2.5, 3,)*100*1000 for 6-, 11-, 20-, 37-, 70-, 135-bits MUX respectively]
//const double maxPayoff      = 1000.0;
//const int run               = 32;

//const int maxEpochs         = 10;

inline bool Testing = false;
const int precisionDigits   = 2;
//const int testFrequency     = 1034;

const char outputFileName[]   = "output_training";
const char featureFileName[]  = "feature_codefragments";
const char ruleFileName[]     = "rule_with_codefragements";
const char resultFile[]       = "result_testing.txt";

const int cfMaxDepth  = 2;
const int cfMinDepth  = 0;
const int cfMaxLength = 8;// pow(2,adfMaxDepth+1); //allow for endstop OPNOP
const int cfMaxArity  = 2;
const int cfMaxStack  = (cfMaxArity-1)*(cfMaxDepth-1)+2;

typedef int opType;
const int opSize = sizeof(opType);

const opType OPNOP  = -100; //to be used as ending symbol
const opType OPAND  = -101;
const opType OPOR   = -102;
const opType OPNAND = -103;
const opType OPNOR  = -104;
const opType OPNOT  = -105;
//const opType OPUNITY = -106;

const int totalFunctions = 5;
const opType functionCodes[] = {OPAND, OPOR, OPNAND, OPNOR, OPNOT};
//const opType functionCodes[] = {OPAND, OPOR, OPNOT};

const int numLeaf = 4;

struct Leaf
{
    opType featureNumber;
    float lowerBound;
    float upperBound;
};

struct CodeFragment
{
    opType codeFragment[cfMaxLength];
    Leaf leaf[numLeaf];
    int cfID;
};

inline int condLength=13;
inline const int maxCondLength = 25;

struct Classifier
{
    CodeFragment condition[maxCondLength];
    int action;
    double prediction;
    double predictionError;
    double accuracy;
    double fitness;
    int experience;
    int numerosity;
    double actionSetSize;
    int timeStamp;
    int specificness; //number of specific CFs
};


struct ClassifierSet
{
    Classifier *classifier;
    ClassifierSet *next;
};

struct DataSource
{
    float *state;
    int action;

    ~DataSource(){
        delete []state;
    }
};
