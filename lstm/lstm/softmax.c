void softmax(FLOAT *src, int n, FLOAT *dst) {
  int i; 
  FLOAT sum = 0;
    
  for(i = 0; i < n; i++) {
    dst[i] = exp(src[i]);    
    sum += dst[i];
  }
    
  for(i = 0; i < n; i++) {
    dst[i] /= sum;
  }
} 

void dSoftmax(FLOAT *y, int n, FLOAT *dst) {
  int i;
  
  for(i = 0; i < n; i++) {
    dst[i] = y[i] * (1 - y[i]);
  }
}