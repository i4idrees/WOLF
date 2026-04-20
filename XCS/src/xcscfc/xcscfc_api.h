#ifndef XCSCFC_API_H
#define XCSCFC_API_H

#include <stdio.h>

/**
 * Most parameter-names are chosen similar to the 'An Algorithmic Description of XCS' ( Butz&Wilson, IlliGAL report 2000017).
 */
//----------------------------------------------------------------------------------------------------------------------------

const double alpha      = 0.1;   // The fall of rate in the fitness evaluation.
const double beta       = 0.2;   // The learning rate for updating fitness, prediction, prediction error, and action set size estimate in XCS's classifiers.
const double gama       = 0.95;  // The discount rate in multi-step problems.
const double delta      = 0.1;   // The fraction of the mean fitness of the population below which the fitness of a classifier may be considered in its vote for deletion.
const double nu         = 5.0;   // Specifies the exponent in the power function for the fitness evaluation.
const double theta_GA   = 25.0;  // The threshold for the GA application in an action set.
const double epsilon_0  = 10.0;  // The error threshold under which the accuracy of a classifier is set to one.
const int    theta_del  = 20;    // Specified the threshold over which the fitness of a classifier may be considered in its deletion probability.

//crossover
const int    crossoverType = 2;    // 0 uniform, 1 onePoint, and 2 twoPoint crossover.
const double pX            = 0.8;  // The probability of applying crossover in an offspring classifier.

//mutation
const double pM            = 0.04;  // The probability of mutating one allele and the action in an offspring classifier.
const int    mutationType  = 0;     // 0 niche, and 1 general mutation.
const double P_dontcare    = 0.33;   // The probability of using a don't care symbol in an allele when covering.

const double predictionErrorReduction = 1.0;   //The reduction of the prediction error when generating an offspring classifier.
const double fitnessReduction         = 0.1;   //The reduction of the fitness when generating an offspring classifier.
const int    theta_sub                = 20;    //The experience of a classifier required to be a subsumer.
const double predictionIni            = 10.0;  //The initial prediction value when generating a new classifier (e.g in covering).
const double predictionErrorIni       = 0.0;   //The initial prediction error value when generating a new classifier (e.g in covering).
const double fitnessIni               = 0.01;  //The initial fitness value when generating a new classifier (e.g in covering).

const bool   doGASubsumption            = true;  //Specifies if GA subsumption should be executed.
const bool   doActSetSubsumption        = false; //Specifies if action set subsumption should be executed.
const double tournamentSize             = 0.4;   //The fraction of classifiers participating in a tournament from an action set.
const double forceDifferentInTournament = 0.0;
const bool   doGAErrorBasedSelect       = false;
const double selectTolerance            = 0.0;
const char   dontcare                   = '#';   //The don't care symbol (normally '#')
const float  m_0                        = 0.5;   //to be used in mutation

const long   _M = 2147483647; //Constant for the random number generator (modulus of PMMLCG = 2^31 -1).
const long   _A = 16807;      //Constant for the random number generator (default = 16807).

// ======================================================
// FORWARD DECLARATIONS (HIDE INTERNAL STRUCTURES)
// ======================================================

struct Classifier;
struct ClassifierSet;
struct CodeFragment;
struct Leaf;
typedef int opType;

// ======================================================
// CORE SYSTEM INITIALIZATION
// ======================================================

void setInitialVariables(Classifier *clfr, double setSize, int time);

void initializePopulation(ClassifierSet **population, FILE *cfReadingFilePointer);

int getNumPreviousCFs();

// ======================================================
// POPULATION STATISTICS
// ======================================================

int getNumerositySum(ClassifierSet *set);

int getSetSize(ClassifierSet *set);

double getAvgFitness(ClassifierSet *set);

int getNumFitterCFs(ClassifierSet *set, double avgFitness);

// ======================================================
// MATCHING + ACTION SELECTION
// ======================================================

ClassifierSet* getMatchSet(ClassifierSet **population,
                            ClassifierSet **killset,
                            float state[],
                            int itTime);

int nrActionsInSet(ClassifierSet *set, bool coveredActions[]);

bool isConditionMatched(CodeFragment clfrCond[], float state[]);

Classifier* matchingCondAndSpecifiedAct(float state[], int act, int setSize, int time);

void createMatchingCondition(CodeFragment cond[], float state[]);

// ======================================================
// PREDICTION / ACTION STRATEGY
// ======================================================

void getPredictionArray(ClassifierSet *ms);

double getBestValue();

int randomActionWinner();

int bestActionWinner();

int rouletteActionWinner();

ClassifierSet* getActionSet(int action, ClassifierSet *ms);

// ======================================================
// LEARNING / UPDATE PHASE
// ======================================================

void updateActionSet(ClassifierSet **aset,
                     double maxPrediction,
                     double reward,
                     ClassifierSet **pop,
                     ClassifierSet **killset);

void updateFitness(ClassifierSet *aset);

void discoveryComponent(ClassifierSet **set,
                        ClassifierSet **pop,
                        ClassifierSet **killset,
                        int itTime,
                        float situation[]);

void getDiscoversSums(ClassifierSet *set,
                      double *fitsum,
                      int *setsum,
                      int *gaitsum);

void setTimeStamps(ClassifierSet *set, int itTime);

// ======================================================
// SELECTION OPERATORS
// ======================================================

void selectTwoClassifiers(Classifier **cl,
                          Classifier **parents,
                          ClassifierSet *set,
                          double fitsum,
                          int setsum);

Classifier* selectClassifierUsingTournamentSelection(ClassifierSet *set,
                                                     int setsum,
                                                     Classifier *notMe);

Classifier* selectClassifierUsingRWS(ClassifierSet *set,
                                     double fitsum);

// ======================================================
// GENETIC OPERATORS
// ======================================================

void subsumeCFs(Classifier *clfr, float state[]);

bool crossover(Classifier **cl, int crossoverType);

void uniformCrossover(Classifier **cl);

void onePointCrossover(Classifier **cl);

void twoPointCrossover(Classifier **cl);

bool mutation(Classifier *clfr, float state[]);

bool applyNicheMutation(Classifier *clfr, float state[]);

bool applyGeneralMutation(Classifier *clfr, float state[]);

bool mutateAction(Classifier *clfr);

// ======================================================
// CLASSIFIER MANAGEMENT
// ======================================================

void insertDiscoveredClassifier(Classifier **cl,
                                Classifier **parents,
                                ClassifierSet **set,
                                ClassifierSet **pop,
                                ClassifierSet **killset,
                                int len,
                                float state[]);

void doActionSetSubsumption(ClassifierSet **aset,
                            ClassifierSet **pop,
                            ClassifierSet **killset);

void subsumeClassifier(Classifier *cl,
                       Classifier **parents,
                       ClassifierSet *locset,
                       ClassifierSet **pop,
                       float state[]);

bool subsumeClassifierToSet(Classifier *cl, ClassifierSet *set);

bool subsumes(Classifier *cl1, Classifier *cl2);

bool isSubsumer(Classifier *cl);

bool isMoreGeneral(Classifier *clfr1, Classifier *clfr2);

bool compareDontcares(Classifier *clfr1, Classifier *clfr2);

bool checkNonDontcares(CodeFragment cond1[], CodeFragment cond2[]);

// ======================================================
// SET OPERATIONS
// ======================================================

bool addClassifierToPointerSet(Classifier *cl, ClassifierSet **pointerset);

bool addClassifierToSet(Classifier *cl, ClassifierSet **clSet);

void addNewClassifierToSet(Classifier *cl, ClassifierSet **clSet);

bool equals(Classifier *clfr1, Classifier *clfr2);

// ======================================================
// DELETION / CLEANUP
// ======================================================

Classifier* deleteStochClassifier(ClassifierSet **pop);

double getDelProp(Classifier *clfr, double meanFitness);

Classifier* deleteTypeOfClassifier(ClassifierSet *setp,
                                   ClassifierSet *setpl,
                                   ClassifierSet **pop);

bool updateSet(ClassifierSet **uset, ClassifierSet *killset);

bool deleteClassifierPointerFromSet(ClassifierSet **set, Classifier *clp);

// ======================================================
// MEMORY MANAGEMENT
// ======================================================

void freeSet(ClassifierSet **cls);

void freeClassifierSet(ClassifierSet **cls);

void freeClassifier(Classifier *cl);

// ======================================================
// OUTPUT / DEBUG
// ======================================================

void printClassifierSet(ClassifierSet *head);

void fprintClassifierSet(FILE *fpClfr, FILE *fpCF, ClassifierSet *head);

void printClassifier(Classifier *clfr);

void fprintClassifier(FILE *fp, Classifier *classifier);

// ======================================================
// UTILITIES
// ======================================================

ClassifierSet* sortClassifierSet(ClassifierSet **cls, int type);

void simplifyPopulation(ClassifierSet **population);

double absoluteValue(double value);

float computeDistance(CodeFragment clfrCond[], float cond[]);

// ======================================================
// CF / UTILITY FUNCTIONS
// ======================================================

CodeFragment addLeafCF(CodeFragment cf);

bool equalTwoLeaf(Leaf lf1, Leaf lf2);

void initializeCFPopulation(FILE *cfReadingFilePointer);

void getPreviousCFPopulation(FILE *cfReadingFilePointer);

opType getOpType(char str[]);

bool isExists(CodeFragment newCF, CodeFragment cfPopulation[], int numCFs);

bool equalTwoCFs(CodeFragment cf1, CodeFragment cf2);

bool isDontcareCF(CodeFragment cf);

int numberOfNonDontcares(CodeFragment cond[]);

void printCF(CodeFragment cf);

void validateDepth(opType* cf, opType* end);

CodeFragment createNewCF(int id);

void storeCFs(ClassifierSet* population, FILE *cfWritingFilePointer);

bool isGeneralCF(CodeFragment cf1, CodeFragment cf2);

bool isMoreGeneralLeaf(Leaf lf1, Leaf lf2);

int evaluateCF(CodeFragment cf, float state[]);

bool isPreviousLevelsCode(const opType code);

int getNumberOfArguments(const opType opcode);

opType leafOpCode(const int r);

opType randomLeaf();

int validLeaf(const opType opcode);

opType randomFunction();

opType* randomProgram(opType* prog,
                      const int isfull,
                      const int maxDepth,
                      const int minDepth);

char* leafname(const opType code);

void DepthMax(const opType* const end,
              opType** prog,
              int& argstogo,
              int& depth);

// ======================================================
// MISC
// ======================================================

Leaf leafNode(std::string str);

char* leafInterval(const Leaf leaf);

char* opchar(opType code);

void outprog_bin(const opType* prog, int size);

void outprog(CodeFragment prog, int size, FILE *fp);

float roundRealValueLib(float val, int num);

void setSeed(long newSeed);
long getSeed();
double drand();

int irand(int n);

#endif