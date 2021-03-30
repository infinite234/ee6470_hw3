#include <cmath>
#include <iomanip>

#include "gaussFilter.h"

gaussFilter::gaussFilter(sc_module_name n)
    : sc_module(n), t_skt("t_skt"), base_offset(0) {
  SC_THREAD(do_filter);

  t_skt.register_b_transport(this, &gaussFilter::blocking_transport);
}

// gauss mask
const int mask[MASK_X][MASK_Y] = {{1,2,1},{2,4,2},{1,2,1}};

void gaussFilter::do_filter() {
  { wait(CLOCK_PERIOD, SC_NS); }
  while (true) {
      Red = 0; Blue =0; Green = 0;
      wait(CLOCK_PERIOD, SC_NS);
    double total =0;
    for (unsigned int v = 0; v < MASK_Y; ++v) {
      for (unsigned int u = 0; u < MASK_X; ++u) {
        wait(CLOCK_PERIOD, SC_NS);
          Red += i_r.read()* mask[u][v];
          Green += i_g.read()* mask[u][v];
          Blue += i_b.read()* mask[u][v];
          wait(CLOCK_PERIOD, SC_NS);
          total+=mask[u][v];
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

void gaussFilter::blocking_transport(tlm::tlm_generic_payload &payload,
                                     sc_core::sc_time &delay) {
  sc_dt::uint64 addr = payload.get_address();
  addr = addr - base_offset;
  unsigned char *mask_ptr = payload.get_byte_enable_ptr();
  unsigned char *data_ptr = payload.get_data_ptr();
  word buffer;
  switch (payload.get_command()) {
  case tlm::TLM_READ_COMMAND:
    switch (addr) {
    case gauss_FILTER_RESULT_ADDR:
          buffer.uint= o_result1.read();
          break;
    case gauss_FILTER_RESULT2_ADDR:
          buffer.uint= o_result2.read();
          break;
    case gauss_FILTER_RESULT3_ADDR:
          buffer.uint= o_result3.read();

          //mask[0] = 0xff;
         // mask[1] = 0xff;
         // mask[2] = 0xff;
          //mask[3] = 0;
    //case SOBEL_FILTER_RESULT_ADDR:
      //if (mask_ptr[0] == 0xff) {
        //o_result1.read(buffer.uc[0]);
      //}
      //if (mask_ptr[1] == 0xff) {
        //o_result2.read(buffer.uc[1]);
      //}
      //if (mask_ptr[2] == 0xff) {
        //o_result3.read(buffer.uc[2]);
      //}
      break;
    default:
      std::cerr << "Error! gaussFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    data_ptr[0] = buffer.uc[0];
    data_ptr[1] = buffer.uc[1];
    data_ptr[2] = buffer.uc[2];
    data_ptr[3] = buffer.uc[3];
    break;

  case tlm::TLM_WRITE_COMMAND:
    switch (addr) {
    case gauss_FILTER_R_ADDR:
      if (mask_ptr[0] == 0xff) {
        i_r.write(data_ptr[0]);
      }
      if (mask_ptr[1] == 0xff) {
        i_g.write(data_ptr[1]);
      }
      if (mask_ptr[2] == 0xff) {
        i_b.write(data_ptr[2]);
      }
      break;
    default:
      std::cerr << "Error! gaussFilter::blocking_transport: address 0x"
                << std::setfill('0') << std::setw(8) << std::hex << addr
                << std::dec << " is not valid" << std::endl;
      break;
    }
    break;

  case tlm::TLM_IGNORE_COMMAND:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  default:
    payload.set_response_status(tlm::TLM_GENERIC_ERROR_RESPONSE);
    return;
  }
  payload.set_response_status(tlm::TLM_OK_RESPONSE); // Always OK
}
