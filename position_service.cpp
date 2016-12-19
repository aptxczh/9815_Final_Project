#include "position_service.hpp"
#include "risk_service.hpp"
#include "trade_booking_service.hpp"
#include "historical_data_service.hpp"
void PositionServiceListener::ProcessAdd(Position<Bond>& data)
{
	cout << "In PositionServiceListener\n";
	string persist_key = "PersistID:" + data.GetProduct().GetProductId();
	service->AddPosition(data);
	hiservice->PersistData(persist_key, data);
}

PositionServiceListener::PositionServiceListener()
{
	service = BondRiskService::get_instance();
	hiservice = PositionHistoricalDataService::get_instance();
}

void BondPositionService::OnMessage(Position<Bond>& data) {

}

// Add a trade to the service
void BondPositionService::AddTrade(const Trade<Bond>& data) {
	cout << "adding trade\n";
	cout << "In PositionService.\n";
	//add the trade data into position and pass it on as Position<Bond> to PositionService
	string cus_id = data.GetProduct().GetProductId();
	int quantity = 0;
	if (data.GetSide() == BUY)	quantity = data.GetQuantity();
	else if (data.GetSide() == SELL)	quantity = -data.GetQuantity();
	all_position[cus_id].AddPosition(data.GetBook(), quantity);
	for (auto listener : _listeners) {
		listener->ProcessAdd(all_position[cus_id]);
	}
}

BondPositionService::BondPositionService() {
	AddListener(PositionServiceListener::get_instance());
}
