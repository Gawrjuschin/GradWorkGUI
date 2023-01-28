#include "table_data.h"
#include "models.h"

Table_Data::Table_Data()
  : p_rmodel(new Request_Model)
  , p_emodel(new Event_Model)
{

}

Table_Data::~Table_Data() = default;

Request_Model* Table_Data::request_model()
{
  return p_rmodel.get();
}

Event_Model*   Table_Data::event_model()
{
  return p_emodel.get();
}
