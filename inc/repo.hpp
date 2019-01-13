#ifndef __REPO_HPP__
#define __REPO_HPP__

#include "uuid.hpp"
#include "json.hpp"
#include <pqxx/pqxx>

using json = nlohmann::json;

class repo
{
private:
  pqxx::connection _conn;
  std::string get_or_create_x_id(pqxx::work& w, const std::string& entity, const std::string& id);
  std::string get_or_create_sensor__id(pqxx::work& w, const std::string& id);
  std::string get_or_create_equipment__id(pqxx::work& w, const std::string& id);
  std::string get_or_create_parameter_type__id(pqxx::work& w, const std::string& id);

public:
  repo();
  ~repo();
  void store_measurement(json measurement);
};

#endif
