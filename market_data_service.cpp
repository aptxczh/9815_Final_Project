#include "market_data_service.hpp"
#include "historical_data_service.hpp"
#include "bond_algo_execution_service.hpp"
#include "products.hpp"
#include <fstream>

void MarketDataConnector::GetMarketData()
{   // pass an order book to MarketDataService;
	cout << "in MarketDataConnector.\n";

	ifstream is("data/marketdata.csv");
	string line;
	// skip the header
	getline(is, line);
	for (int i = 0; i < 12; i++) {
		getline(is, line);
		vector<string> elems = split_one_line(line);
		string _cusip = elems[0];

		PricingSide side;
		vector<Order> bid_stack, offer_stack;
		double price;	long quantity;
		int idx = 1;
		for (int k = 1; k <= 5; k++) {
			price = string_to_price(elems[idx++]);
			quantity = std::stol(elems[idx++]);
			side = BID;
			Order bid_order(price, quantity, side);
			bid_stack.push_back(bid_order);
		}

		for (int k = 1; k <= 5; k++) {
			price = string_to_price(elems[idx++]);
			quantity = std::stol(elems[idx++]);
			side = OFFER;
			Order offer_order(price, quantity, side);
			offer_stack.push_back(offer_order);
		}
		Bond &b = bonds[_cusip];
		OrderBook<Bond> order_book(b, bid_stack, offer_stack);
		service->OnMessage(order_book);
		BidOffer best = { bid_stack[0],offer_stack[0] };
		(service->GetBestBidOffer(_cusip)) = best;
	}
}

MarketDataConnector::MarketDataConnector() {
	service = BondMarketDataService::get_instance();
}

BondMarketDataService::BondMarketDataService() {
	//AddListener(HistoricalDataServiceListener::get_instance());
	AddListener(BondAlgoExecutionServiceListener::get_instance());
}

void BondMarketDataService::OnMessage(OrderBook <Bond> &data) {
	for (auto listener : _listeners) {
		listener->ProcessUpdate(data);
	}
}

void BondMarketDataServiceListener::ProcessUpdate(OrderBook<Bond> &data) {
	nextservice->OnMessage(data);
}


Order::Order(double _price, long _quantity, PricingSide _side)
{
	price = _price;
	quantity = _quantity;
	side = _side;
}

double Order::GetPrice() const
{
	return price;
}

long Order::GetQuantity() const
{
	return quantity;
}

PricingSide Order::GetSide() const
{
	return side;
}

BidOffer::BidOffer(const Order &_bidOrder, const Order &_offerOrder) :
	bidOrder(_bidOrder), offerOrder(_offerOrder)
{
}

const Order& BidOffer::GetBidOrder() const
{
	return bidOrder;
}

const Order& BidOffer::GetOfferOrder() const
{
	return offerOrder;
}

template<typename T>
OrderBook<T>::OrderBook(const T &_product, const vector<Order> &_bidStack, const vector<Order> &_offerStack) :
	product(_product), bidStack(_bidStack), offerStack(_offerStack)
{
}

//template<typename T>
//const T& OrderBook<T>::GetProduct() const
//{
//	return product;
//}

//template<typename T>
//const vector<Order>& OrderBook<T>::GetBidStack() const
//{
//	return bidStack;
//}
//
//template<typename T>
//const vector<Order>& OrderBook<T>::GetOfferStack() const
//{
//	return offerStack;
//}