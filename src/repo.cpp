#include "repo.hpp"

repo::repo() : _conn("")
{
  this->_conn.prepare("sensor_by_id", "SELECT _id, id, info FROM sensor WHERE id = $1");
  this->_conn.prepare("equipment_by_id", "SELECT _id, id, info FROM equipment WHERE id = $1");
  this->_conn.prepare("parameter_type_by_id", "SELECT _id, id, unit, info FROM parameter_type WHERE id = $1");
  this->_conn.prepare("new_equipment", "INSERT INTO equipment (_id, id) VALUES ($1, $2)");
  this->_conn.prepare("new_sensor", "INSERT INTO sensor (_id, id) VALUES ($1, $2)");
  this->_conn.prepare("new_parameter_type", "INSERT INTO parameter_type (_id, id) VALUES ($1, $2)");
  this->_conn.prepare("new_measurement", "INSERT INTO measurement (_id, ts, _equipment_id, index, rssi) VALUES ($1, $2, $3, $4, $5)");
  this->_conn.prepare("new_parameter", "INSERT INTO parameter (_id, _measurement_id, _parameter_type_id, _sensor_id, val) VALUES ($1, $2, $3, $4, $5)");
}

repo::~repo() {
}

void repo::store_measurement(json measurement) {
  pqxx::work w(this->_conn);

  try {
    std::string sensor__id = get_or_create_sensor__id(w, measurement["message"]["sensor_id"]);
    std::string equipment__id = get_or_create_equipment__id(w, measurement["message"]["mcu_id"]);
    std::string pt_temperature__id = get_or_create_parameter_type__id(w, "temperature");
    std::string pt_humidity__id = get_or_create_parameter_type__id(w, "humidity");
    std::string measurement__id = get_uuid();

    w.prepared("new_measurement")
      (measurement__id)
      (std::string(measurement["timestamp"]))
      (equipment__id)
      ((uint)measurement["message"]["message_index"])
      ((float)measurement["rssi"]).exec();
    w.prepared("new_parameter")
      (std::string(get_uuid()))
      (measurement__id)
      (pt_temperature__id)
      (sensor__id)
      ((float)measurement["message"]["temperature"]).exec();
    w.prepared("new_parameter")
      (std::string(get_uuid()))
      (measurement__id)
      (pt_humidity__id)
      (sensor__id)
      ((float)measurement["message"]["humidity"]).exec();
    w.commit();
  } catch (...) {
    w.abort();
    throw;
  }
}

std::string repo::get_or_create_sensor__id(pqxx::work& w, const std::string& id) {
  return this->get_or_create_x_id(w, "sensor", id);
}

std::string repo::get_or_create_equipment__id(pqxx::work& w, const std::string& id) {
  return this->get_or_create_x_id(w, "equipment", id);
}

std::string repo::get_or_create_parameter_type__id(pqxx::work& w, const std::string& id) {
  return this->get_or_create_x_id(w, "parameter_type", id);
}

std::string repo::get_or_create_x_id(pqxx::work& w, const std::string& entity, const std::string& id) {
  std::string _id;
  pqxx::result r = w.prepared(entity + "_by_id")(id).exec();

  if (r.size() > 0) {
    _id = r[0]["_id"].c_str();
  } else {
    _id = get_uuid();
    r = w.prepared("new_" + entity)(_id)(id).exec();
  }

  return _id;
}
