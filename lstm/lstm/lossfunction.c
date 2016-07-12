FLOAT mse(FLOAT *target, FLOAT *y, int n) {
  int i; 
  FLOAT sum = 0;
  
  for(i = 0; i < n; i++) {
    sum += pow(target[i] - y[i], 2);
  }
  sum /= 2;
  
  return sum;
}

FLOAT crossEntropy(FLOAT *target, FLOAT *y, int n) {
  int i;
  FLOAT sum = 0; 
  
  for(i = 0; i < n; i++) {
    if(target[i] == 1) {
      sum += log(y[i]);      
    }
  }  
  return -sum;
}