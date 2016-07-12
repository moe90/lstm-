#ifndef DATA_H
#define DATA_H

#include <dirent.h>

typedef struct {  
  int length; 
  FLOAT **x;
  FLOAT **y;
  
  char *fileName;
} sequence_t;

typedef struct {  
  int numSequences; 
  int numFeatures;
  int numOutputs;
    
  sequence_t *sequences; 
} data_t;

void fileToSequenceLength(char *file, int *len);
void folderToFiles(char *folder, char ***files, int *numFiles, int **seqLengths);
void allocateDataMemory(data_t *data, int numSequences, int numFeatures, int numOutputs, int *seqLengths);
void allocateSequenceMemory(sequence_t *sequence, int length, int numFeatures, int numOutputs);
void freeSequenceMemory(sequence_t *sequence, int length, int numFeatures, int numOutputs);
void readDataFromFile(char *file, sequence_t *sequence, int numFeatures, int numOutputs);
void readDataFromDirectory(char *Path, int numFeatures, int numOutputs, data_t *data);
void printData(data_t data, FILE *out, int verbose);
void printSequence(sequence_t sequence, int numFeatures, int numOutputs, FILE *out);
void printElement(sequence_t sequence, int idx, int numFeatures, int numOutputs, FILE *out);

#endif /* DATA_H */