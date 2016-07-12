FLOAT identity(FLOAT x) {
  return x;
}

FLOAT dIdentity(FLOAT y) {
  return 1;
}

FLOAT sigmoid(FLOAT x) {
  return 1 / (1 + exp(-x));
}

FLOAT dSigmoid(FLOAT y) {
  return y * (1 - y);
}

//using fast tanh of math.h instead
/*
FLOAT tanh(FLOAT x) {
  return (2 * sigmoid(2 * x) - 1); 
}
*/

FLOAT dTanh(FLOAT y) {
  return 1 - y * y;
}
