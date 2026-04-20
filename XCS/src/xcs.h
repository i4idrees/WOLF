#include <fstream>

long seeds[31] = {114665,134296,176806,247157,262025,311756,336922,337104,344465,362234,379332,425485,470006,490758,538402,583115,584068,614795,678991,710953,715062,715911,784943,787483,790558,810292,824057,846845,968381,972820,982820};

struct distanceInputClassifier {
  int posClassifier;
  float distance;
};

void startXCS();
void doOneSingleStepExperiment(ClassifierSet **population);
void doOneSingleStepProblemExplore(ClassifierSet **population, float *state, int counter);
void doOneSingleStepProblemExploit(ClassifierSet **population, float *state, int counter, int correct[], double sysError[]);

void header();
void Exit(FILE *fp);
void CreateFiles(int);
void sortK(distanceInputClassifier[], int);
void sortAll(distanceInputClassifier[], int);
void writePerformance(ClassifierSet*, int[], double[], int);

std::string NumberToString(int);
void resetState(float state[], int inputFileint[][maxCondLength+1]);
FILE *filePerformance=NULL, *testPerformance=NULL, *fileClassifierPopulation=NULL, *cfWritingFilePointer=NULL, *cfReadingFilePointer=NULL;
FILE *runs = NULL;

//void checkConvergance();
