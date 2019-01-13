#include "uuid.hpp"

std::string uuid_to_hex(uuid_t uuid);

std::string get_uuid() {
  uuid_t uuid;
  char out[37];

  uuid_generate_random(uuid);
  uuid_unparse_upper(uuid, out);
  uuid_clear(uuid);

  return std::string(out);
}
