#include "data.h"

void fileToSequenceLength(char *file, int *len) {                             
  FILE *fp;
  size_t l;
  ssize_t ret;
  char *line;
  
  if((fp = fopen(file,"r")) == NULL) {
    printError(strcat("couldn't open file ", file), stderr, true);
  }
    
  line = NULL;
  *len = 0;
  while((ret = getline(&line, &l, fp)) != -1) {
    (*len)++;
  }
  free(line);

  fclose(fp);
}

void folderToFiles(char *folder, char ***files, int *numFiles, int **seqLengths) {
  DIR *d;
  struct dirent *dir;
  int i;
  char **fptr;
  int *sptr;
  
  *numFiles = 0;
  
  d = opendir(folder);
  if(d) {
    while((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) {
        *numFiles = *numFiles + 1;
      }
    }
    closedir(d);
  }
  
  if(((*files) = (char **) malloc((*numFiles) * sizeof(char *))) == NULL) {
    printError("out of memory", stderr, true);
  }
  fptr = *files;
  
  if(((*seqLengths) = (int *) malloc((*numFiles) * sizeof(int))) == NULL) {
    printError("out of memory", stderr, true);
  }
  sptr = *seqLengths;
  
  d = opendir(folder);
  if(d) {
    i = 0;
    while((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) {
        if((fptr[i] = (char *) malloc((strlen(dir->d_name) + strlen(folder) + 2) * sizeof(char))) == NULL) {
          printError("out of memory", stderr, true);
        }
        sprintf(fptr[i], "%s/%s", folder, dir->d_name);
        fileToSequenceLength(fptr[i], &sptr[i]);
        
        i++; 
      }           
    }
    closedir(d);
  }
  
  return;
}

void allocateDataMemory(data_t *data, int numSequences, int numFeatures, int numOutputs, int *seqLengths) {
  int i;
  sequence_t *sequence;
    
  data->numSequences = numSequences; 
  data->numFeatures = numFeatures;
  data->numOutputs = numOutputs;

  if((data->sequences = (sequence_t *) malloc(numSequences * sizeof(sequence_t))) == NULL) {
    printError("out of memory", stderr, true);
  }
  sequence = data->sequences;
  
  for(i = 0; i < numSequences; i++) {
    allocateSequenceMemory(&(sequence[i]), seqLengths[i], numFeatures, numOutputs);    
  }
}

void allocateSequenceMemory(sequence_t *sequence, int length, int numFeatures, int numOutputs) {
  int j;
  
  sequence->length = length;
  
  if((sequence->x = (FLOAT **) malloc(sequence->length * numFeatures * sizeof(FLOAT *))) == NULL) {
    printError("out of memory", stderr, true);
  }
  if((sequence->y = (FLOAT **) malloc(sequence->length * numOutputs * sizeof(FLOAT *))) == NULL) {
    printError("out of memory", stderr, true);
  }
  
  for(j = 0; j < sequence->length; j++) {
    if((sequence->x[j] = (FLOAT *) malloc(numFeatures * sizeof(FLOAT))) == NULL) {
      printError("out of memory", stderr, true);
    }
    if((sequence->y[j] = (FLOAT *) calloc(numOutputs, sizeof(FLOAT))) == NULL) {
      printError("out of memory", stderr, true);
    }
  }
}

void freeSequenceMemory(sequence_t *sequence, int length, int numFeatures, int numOutputs) {
  int j;
  
  for(j = 0; j < sequence->length; j++) {
    free(sequence->x[j]);
    free(sequence->y[j]);
  }  
  free(sequence->x);
  free(sequence->y);
}

void readDataFromFile(char *file, sequence_t *sequence, int numFeatures, int numOutputs) {
  int i, j, yTemp;  
  FILE *fp;
  char *line;
  size_t len;
  ssize_t ret;
  char *token;
  const char delim[2] = " ";
        
  if((fp = fopen(file, "r")) == NULL) {
    printError(strcat("Couldn't open file ", file), stderr, true);
  }
  
  if((sequence->fileName = (char *)malloc((strlen(file)+1) * sizeof(char))) == NULL) {
    printError("out of memory", stderr, true);
  }
  strcpy(sequence->fileName, file);
  
  line = NULL;
  len = 0;
  i = 0;
  while ((ret = getline(&line, &len, fp)) != -1) {
    token = strtok(line, delim);

    j = 0;
    while(token != NULL) {
      if(j < numFeatures) {
        sscanf(token, FORMAT, &(sequence->x[i][j]));
      } else if(j == numFeatures) {
        sscanf(token, "%d", &yTemp);
        sequence->y[i][yTemp] = 1;
      } else {
        printError("numFeatures passed was not correct", stderr, true);
      }
      token = strtok(NULL, delim);
      j++;
    }
    i++;
  }

  free(line);
  fclose(fp);
}

void readDataFromDirectory(char *path, int numFeatures, int numOutputs, data_t *data) {
  int i, numFiles, *seqLengths;
  char **files;
  
  folderToFiles(path, &files, &numFiles, &seqLengths);
  
  //TODO: REMOVE THIS
  numFiles = 100;
  
  //printf("reading %s\n", path);
  allocateDataMemory(data, numFiles, numFeatures, numOutputs, seqLengths);
    
  for(i = 0; i < numFiles; i++) {
    readDataFromFile(files[i], &(data->sequences[i]), numFeatures, numOutputs);
    if(i % 100 == 0) {
      //printf("%5.2lf%%\n", 100*(double)i/(double)numFiles);
    }
  }  
}

void printData(data_t data, FILE *out, int verbose) {
  int i;
  
  fprintf(out, "numSequences: %d \n", data.numSequences);
  fprintf(out, "numFeatures: %d \n", data.numFeatures);
  fprintf(out, "numOutputs: %d \n", data.numOutputs);
  if(verbose == true) {
    for(i = 0; i < data.numSequences; i++) {
      printSequence(data.sequences[i], data.numFeatures, data.numOutputs, out);
    }
  }
}

void printSequence(sequence_t sequence, int numFeatures, int numOutputs, FILE *out) {
  int i;
  
  fprintf(out, "length: %d \n", sequence.length);
  for(i = 0; i < sequence.length; i++) {
    printElement(sequence, i, numFeatures, numOutputs, out);
  }
}

void printElement(sequence_t sequence, int idx, int numFeatures, int numOutputs, FILE *out) {
  printLineVectorOfMatrix(sequence.x, idx, numFeatures, "x", out);
  printBinaryLineVectorOfMatrix(sequence.y, idx, numOutputs, "y", out);  
}