#include "execution_service.hpp"
#include "historical_data_service.hpp"
string toString(Market market) {
	if (market == Market::BROKERTEC)
		return "BROKERTEC";
	if (market == Market::CME)
		return "CME";
	else
		return "ESPEED";
}
void ExecutionServiceListener::ProcessAdd(ExecutionOrder<Bond> &data) {

	string persist_key = "PersistID:" + data.GetOrderId();
	service->PersistData(persist_key, data);
}
ExecutionServiceListener::ExecutionServiceListener() {
	service = ExecutionHistoricalDataService::get_instance();
}
// Execute an order on a market
void BondExecutionService::ExecuteOrder(const ExecutionOrder<Bond>& order, Market market) {
	cout << "executing order in ExecutionService\n";
	for (auto listner : _listeners) {
		listner->ProcessAdd(const_cast<ExecutionOrder<Bond>&>(order));
	}
}

