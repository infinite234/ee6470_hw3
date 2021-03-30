#include <cmath>

#include "SobelFilter.h"

SobelFilter::SobelFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

// sobel mask
const int mask[MASK_X][MASK_Y] = {{1,2,1},{2,4,2},{1,2,1}};

void SobelFilter::do_filter() {
  { wait(); }
  while (true) {
      Red = 0; Blue =0; Green = 0; 
      wait();
      double total = 0;
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
          Red += i_r.read()* mask[u][v];
          Green += i_g.read()* mask[u][v];
          Blue += i_b.read()* mask[u][v];
          wait();
        total += mask[u][v];
      }
    }

    int result1 = (int)(Red/total);
    int result2 = (int)(Green/total);
    int result3 = (int)(Blue/total);
    o_result1.write(result1);
    o_result2.write(result2);
    o_result3.write(result3);
  }
}
