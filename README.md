# raw-data-db-agent
An agent moving data from zeromq to the database.

## DB installation
I chose postgresql as the database back-end.
On raspian, it can be installed by running:
```
sudo apt install postgresql-9.6
sudo su postgres
createuser sensor-net -P --interactive
createdb -O sensor-net sensor-net
```
It is necessary to add a line allowing password authentication (before peer authentication) and to restart the service.
```
sudo vim.tiny /etc/postgresql/9.6/main/pg_hba.conf
sudo systemctl restart postgresql
```
Then the tables can be created.
```
psql --username=sensor-net --password --file=db_structure.sql
```
The postgres-client library for C++ as well as the uuid development packet also need to be installed.
```
sudo apt install libpqxx-dev uuid-dev
```

## Run
To run the program, the database credentials need to be provided via the environment.
```
PGUSER=<user> PGPASSWORD=<password> prog/raw-data-db-agent 2>/dev/null 1</dev/null 3>/dev/null &
```

## example message
{"message":{"humidity":54.12139892578125,"mcu_id":"00610000-33373938-17473634","message_index":10988,"sensor_id":"0847","temperature":20.08026123046875,"type":1},"rssi":106.0,"timestamp":"2019-01-10T19:54:31Z"}
