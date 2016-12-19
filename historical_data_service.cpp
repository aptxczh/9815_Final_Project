#include "historical_data_service.hpp"
#include <fstream>
#include "risk_service.hpp"
#include "streaming_service.hpp"
#include "inquiry_service.hpp"
#include "execution_service.hpp"
using namespace std;

void RiskHistoricalDataService::PersistData(string persistKey, const PV01<Bond> & data) {
	string msg = "PV01 is " + std::to_string(data.GetPV01());
	connector->Publish(persistKey+msg);
}

void ExecutionHistoricalDataService::PersistData(string persistKey, const ExecutionOrder<Bond> & data)
{
	string cus_id = data.GetProduct().GetProductId();
	string msg = " CUSID: " + cus_id;
	connector->Publish(persistKey + msg);
}
void PositionHistoricalDataService::PersistData(string persistKey, const Position<Bond> & data)
{
	string msg = "Position of TRSY1:" + to_string(const_cast<Position<Bond>&>(data).GetPosition(string("TRSY1")))+
		"Position of TRSY2:" + to_string(const_cast<Position<Bond>&>(data).GetPosition(string("TRSY2"))) +
		"Position of TRSY3:" + to_string(const_cast<Position<Bond>&>(data).GetPosition(string("TRSY3"))) +
		"Aggregate Position:"+to_string(const_cast<Position<Bond>&>(data).GetAggregatePosition());
	connector->Publish(persistKey+msg);
}
void StreamHistoricalDataService::PersistData(string persistKey, const PriceStream<Bond> & ps)
{
	cout << "PersistStreamingData.\n";
	string msg;
	double bid = ps.GetBidOrder().GetPrice();
	double offer = ps.GetOfferOrder().GetPrice();
	string cus_id = ps.GetProduct().GetProductId();
	msg = "CUSID: " + cus_id + "; Bid Price: " + to_string(bid) +
		"; Offer Price: " + to_string(offer);
	connector->Publish(persistKey + msg);
}

void InquiryHistoricalDataService::PersistData(string persistKey, const Inquiry<Bond> & inq)
{
	cout << "Persisting InquiryData\n";
	string msg;
	msg += " Inquiry Id: " + inq.GetInquiryId();
	if (inq.GetSide() == Side::BUY)
		msg += "; BUY ";
	else msg += "; SELL ";
	msg += inq.GetProduct().GetProductId() + " for quantity of ";
	msg += std::to_string(inq.GetQuantity()) + ", at the price of";
	msg += std::to_string(inq.GetPrice());
	connector->Publish(persistKey+msg);
}

RiskHistoricalDataService::RiskHistoricalDataService() {
	connector = RiskDataConnector::get_instance();
}
PositionHistoricalDataService::PositionHistoricalDataService() {
	connector = PositionDataConnector::get_instance();
}
StreamHistoricalDataService::StreamHistoricalDataService() {
	connector = StreamingDataConnector::get_instance();
}
ExecutionHistoricalDataService::ExecutionHistoricalDataService() {
	connector = ExecutionDataConnector::get_instance();
}
InquiryHistoricalDataService::InquiryHistoricalDataService() {
	connector = InquiryDataConnector::get_instance();
}
void ExecutionDataConnector::Publish(string & data) {
	ofstream os("data/executions.txt", ios_base::app);
	os << data << endl;
}

void RiskDataConnector::Publish(string & data)
{
	ofstream os("data/risk.txt", ios_base::app);
	os << data << endl;
}

void StreamingDataConnector::Publish(string & data)
{
	ofstream os("data/streaming.txt", ios_base::app);
	os << data << endl;
}

void InquiryDataConnector::Publish(string & data)
{
	ofstream os("data/allinquiries.txt", ios_base::app);
	os << data << endl;
}
void PositionDataConnector::Publish(string & data) {
	ofstream os("data/positions.txt", ios_base::app);
	os << data << endl;
}