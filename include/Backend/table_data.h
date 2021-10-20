#ifndef TABLE_DATA_H
#define TABLE_DATA_H

#include <memory>

class Request_Model;
class Event_Model;

class Table_Data
{

public:
  Table_Data();
  ~Table_Data();

  Request_Model* request_model();
  Event_Model*   event_model();

private:
  std::unique_ptr<Request_Model> p_rmodel;
  std::unique_ptr<Event_Model>   p_emodel;

};

#endif // TABLE_DATA_H
