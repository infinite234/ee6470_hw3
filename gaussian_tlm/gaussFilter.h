#ifndef gauss_FILTER_H_
#define gauss_FILTER_H_
#include <systemc>
using namespace sc_core;

#include "tlm"
#include "tlm_utils/simple_target_socket.h"

#include "filter_def.h"

class gaussFilter : public sc_module {
public:
  tlm_utils::simple_target_socket<gaussFilter> t_skt;

  sc_fifo<unsigned char> i_r;
  sc_fifo<unsigned char> i_g;
  sc_fifo<unsigned char> i_b;
  sc_fifo<int> o_result1;
  sc_fifo<int> o_result2;
  sc_fifo<int> o_result3;

  SC_HAS_PROCESS(gaussFilter);
  gaussFilter(sc_module_name n);
  ~gaussFilter() = default;

private:
  void do_filter();
  int Red;
  int Blue;
  int Green;

  unsigned int base_offset;
  void blocking_transport(tlm::tlm_generic_payload &payload,
                          sc_core::sc_time &delay);
};
#endif
