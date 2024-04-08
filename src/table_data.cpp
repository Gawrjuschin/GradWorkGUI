#include "table_data.h"
#include "models.h"

Table_Data::Table_Data()
    : p_rmodel(new RequestModel), p_emodel(new EventModel) {}

Table_Data::~Table_Data() = default;

RequestModel* Table_Data::request_model() { return p_rmodel.get(); }

EventModel* Table_Data::event_model() { return p_emodel.get(); }
