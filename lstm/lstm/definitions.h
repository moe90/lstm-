#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define USE_BLAS

#define START_SIZE 3000

#define FORWARD 0
#define BACKWARD 1

#define INPUT_LAYER 0
#define LSTM_LAYER 1
#define OUTPUT_LAYER 2

#define MSE 0
#define SOFTMAX 1

#define TIMESTEP 0
#define SEQUENCE 1

#define NONE 0
#define L1 1
#define L2 2

#define SIGMOID 0
#define TANH 1

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define NEWLINE "\r\n"

#define true 1
#define false 0

//#define GRADIENTCHECKING 
/*
#define FLOAT float
#define FORMAT "%f" /**/
//*
#define FLOAT double
#define FORMAT "%lf" /**/

#define EPSILON 1e-4

#define LEAFSIZE 1000

#endif /* DEFINITIONS_H */