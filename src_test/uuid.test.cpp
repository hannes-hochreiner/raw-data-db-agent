#include <gtest/gtest.h>
#include "uuid.hpp"

namespace {
  TEST(global, round_trip) {
    std::string res = get_uuid();
    const std::string hex_chars("0123456789ABCDEF");

    for (uint cntr = 0; cntr < 36; cntr++) {
      if ((cntr == 8) || (cntr == 13) || (cntr == 18) || (cntr == 23)) {
        EXPECT_EQ('-', res[cntr]);
      } else {
        EXPECT_NE(hex_chars.find(res[cntr]), std::string::npos);
      }
    }
  }
}
