#ifndef FILTER_DEF_H_
#define FILTER_DEF_H_

//const int MASK_N = 2;
const int MASK_X = 3;
const int MASK_Y = 3;

const int gauss_FILTER_R_ADDR = 0x00000000;
const int gauss_FILTER_RESULT_ADDR = 0x00000004;
const int gauss_FILTER_RESULT2_ADDR = 0x00000012;
const int gauss_FILTER_RESULT3_ADDR = 0x00000020;

union word {
  int sint;
  unsigned int uint;
  unsigned char uc[4];
};

#endif
