
void print1dArray(FLOAT *a, int n, char *name, FILE *out) {
  int i;
  
  fprintf(out, "%s = np.array([", name);
  for(i = 0; i < n; i++) {
    fprintf(out, "%e, ", a[i]);
    if(i+1 != n) { 
      fprintf(out, " ");
    }
  }
  fprintf(out, "], dtype = np.float64)" NEWLINE);
}

void print2dArray(FLOAT *a, int m, int n, char *name, FILE *out) {
  int i, j;
  
  fprintf(out, "%s = np.array([", name);
  for(i = 0; i < m; i++) {
    fprintf(out, "[");
    for(j = 0; j < n; j++) {
      fprintf(out, "%e, ", a[i*n+j]);
    }
    fprintf(out, "],");
  }  
  fprintf(out, "], dtype = np.float64)" NEWLINE);
}

void print2dArray2(FLOAT ***a, int m, int n, char *name, FILE *out, int b) {
  int t, i;
  
  fprintf(out, "%s = np.array([", name);
  for(t = 0; t < m; t++) {
    fprintf(out, "[");
    for(i = 0; i < n; i++) {
      fprintf(out, "%e, ", a[t][b][i]);
    }
    fprintf(out, "],");
  }
  fprintf(out, "], dtype = np.float64)" NEWLINE);
}

void print3dArray(FLOAT ***a, int l, int m, int n, char *name, FILE *out, int b) {
  int t, i, j;
  
  fprintf(out, "%s = np.array([", name);
  for(t = 0; t < l; t++) {
    fprintf(out, "[");
    for(i = 0; i < m; i++) {
      fprintf(out, "[");
      for(j = 0; j < n; j++) {
        fprintf(out, "%e, ", a[t][b][i*n + j]); //?
      }
      fprintf(out, "],");
    }
    fprintf(out, "],");
  }
  fprintf(out, "], dtype = np.float64)" NEWLINE);
}

void printLineVectorOfMatrix(FLOAT **m, int line, int n, char *name, FILE *out) {
  int i;
  
  fprintf(out, "%s = [", name);
  for(i = 0; i < n; i++) {
    fprintf(out, "%lf ", m[line][i]);
    if(i+1 != n) { 
      fprintf(out, " ");
    }
  }
  fprintf(out, "]" NEWLINE);
}

void printBinaryLineVectorOfMatrix(FLOAT **m, int line, int n, char *name, FILE *out) {
  int i;
  
  fprintf(out, "%s = [", name);
  for(i = 0; i < n; i++) {
    if(1 == (int)m[line][i]) {
      fprintf(out, ANSI_COLOR_RED "%c" ANSI_COLOR_RESET, 219);
    } else {
      fprintf(out, "%c", 219);
    }
  }
  fprintf(out, "]" NEWLINE);
}

void printMatrixI(int *a, int m, int n, char *name, FILE *out) {
  int i, j;
  
  fprintf(out, "%s = [" NEWLINE, name);
  for(i = 0; i < m; i++) {
    for(j = 0; j < n; j++) {
      fprintf(out, "%d ", a[i*n+j]);
    }
    fprintf(out, NEWLINE);
  }  
  fprintf(out, "]" NEWLINE);
}

void printFiles(char **files, int numFiles, FILE *out) {
  int i;

  fprintf(out, "numFiles: %d" NEWLINE, numFiles);
  for(i = 0; i < numFiles; i++) {
    fprintf(out, "%d: %p: %s" NEWLINE, i, files[i], files[i]);
  }
}

void printError(char *errorMessage, FILE *out, int ex) {
  fprintf(out, ANSI_COLOR_RED "%s" NEWLINE ANSI_COLOR_RESET, errorMessage);
  if(ex == true) {
    exit(1);
  }
}
 
FLOAT randGauss(FLOAT mu, FLOAT sigma) {
  FLOAT U1, U2, W, mult;
  static FLOAT X1, X2;
  static int call = 0;
 
  if (call == 1) {
    call = !call;
    return (mu + sigma * (FLOAT) X2);
  }

  do {
    U1 = -1 + ((FLOAT) rand () / RAND_MAX) * 2;
    U2 = -1 + ((FLOAT) rand () / RAND_MAX) * 2;
    W = pow (U1, 2) + pow (U2, 2);
  } while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (FLOAT) X1);
}
