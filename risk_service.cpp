#include "risk_service.hpp"
#include "historical_data_service.hpp"

map<string, double> risk_map;
void RiskServiceListener::ProcessAdd(PV01<Bond> &data) {
	string persist_key = "PersistID:" + (data.GetProducts()[0]).GetProductId();
	service->PersistData(persist_key,data);
}
RiskServiceListener::RiskServiceListener() {
	service = RiskHistoricalDataService::get_instance();
}
// Add a position that the service will risk
void BondRiskService::AddPosition(Position<Bond>& position) {
	if (risk_map.size() == 0) {
		risk_map["912828U40"] = 0.018574732;
		risk_map["912828U73"] = 0.022954258;
		risk_map["912828U65"] = 0.045681529;
		risk_map["912828U57"] = 0.075925714;
		risk_map["912828U24"] = 0.109254107;
		risk_map["912810RU4"] = 0.182341252;
	}
	cout << "in RiskService.\n";
	string cus_id = position.GetProduct().GetProductId();
	long _quantity = position.GetAggregatePosition();
	double _risk = risk_map[cus_id] * position.GetAggregatePosition() / 100.0;
	PV01<Bond> pv01({ position.GetProduct() }, _risk, _quantity);
	for (auto listener : _listeners) {
		listener->ProcessAdd(pv01);
	}
}

// Get the bucketed risk for the bucket sector
const PV01<Bond>& BondRiskService::GetBucketedRisk(const BucketedSector<Bond> &sector) const {
	return data.at(sector.GetName());
}
