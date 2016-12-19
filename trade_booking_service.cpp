#include "trade_booking_service.hpp"

// pass the data on, to PositionService
void BondTradeServiceListener::ProcessAdd(Trade<Bond>& data) {
	cout << "In TradeServiceListener\n";
	service->AddTrade(data); //should not use OnMessage. Use AddTrade Instead

}

BondTradeServiceListener::BondTradeServiceListener() {
	service = BondPositionService::get_instance();
}

// Book the trade in OnMessage

void BondTradeBookingService::OnMessage(Trade<Bond>& trade) {
	cout << "in TradeBookingService\n";
	BookTrade(trade);
}

// Book the trade, by passing the trade data to listener

void BondTradeBookingService::BookTrade(const Trade<Bond>& trade) {
	for (auto listener : _listeners)	{
		listener->ProcessAdd(const_cast<Trade<Bond>&>(trade));
	}
}

BondTradeBookingService::BondTradeBookingService() {
	AddListener(BondTradeServiceListener::get_instance());
}

void BondTradeBookingConnector::GetTradingData() {
	ifstream is("data/trades.csv");
	string line;
	string _cusip, _trade_id, _book, _price, _quantity, _side;
	// skip the header
	getline(is, line);
	while (getline(is, line)) {
		cout << "Got another trade.\n";
		vector<string> elems = split_one_line(line);
		std::tie(_cusip, _trade_id, _book, _price, _quantity, _side) = std::make_tuple(
			elems[0], elems[1], elems[2], elems[3], elems[4], elems[5]);
		// get product from CUSIP
		Bond& b = bonds[_cusip];
		double price = string_to_price(_price);
		Side side = (_side == "BUY" ? BUY : SELL);
		Trade<Bond> trade(b, _trade_id, _book, price, std::stol(_quantity), side);
		//cout << b.GetProductId() << endl;
		//cout << trade.GetProduct().GetProductId() << endl;
		service->OnMessage(trade);
	}
}

BondTradeBookingConnector::BondTradeBookingConnector() {
	service = BondTradeBookingService::get_instance();
}
