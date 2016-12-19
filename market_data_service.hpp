/**
 * marketdataservice.hpp
 * Defines the data types and Service for order book market data.
 *
 * @author Breman Thuraisingham
 */
#ifndef MARKET_DATA_SERVICE_HPP
#define MARKET_DATA_SERVICE_HPP

#include <string>
#include <vector>
#include "soa.hpp"
#include "common_header.h"
#include "products.hpp"

using namespace std;
enum PricingSide { BID, OFFER };
// Side for market data


/**
 * A market data order with price, quantity, and side.
 */
class Order
{

public:
	Order(){}
	// ctor for an order
	Order(double _price, long _quantity, PricingSide _side);

	// Get the price on the order
	double GetPrice() const;

	// Get the quantity on the order
	long GetQuantity() const;

	// Get the side on the order
	PricingSide GetSide() const;

private:
	double price;
	long quantity;
	PricingSide side;

};

/**
* Class representing a bid and offer order
*/
class BidOffer
{

public:
	BidOffer(){}
	// ctor for bid/offer
	BidOffer(const Order &_bidOrder, const Order &_offerOrder);

	// Get the bid order
	const Order& GetBidOrder() const;

	// Get the offer order
	const Order& GetOfferOrder() const;

private:
	Order bidOrder;
	Order offerOrder;

};

/**
* Order book with a bid and offer stack.
* Type T is the product type.
*/
template<typename T>
class OrderBook
{

public:
	OrderBook(){}
	// ctor for the order book
	OrderBook(const T &_product, const vector<Order> &_bidStack, const vector<Order> &_offerStack);

	// Get the product
	const T& GetProduct() const {
		return product;
	}

	// Get the bid stack
	const vector<Order>& GetBidStack() const {
		return bidStack;
	}

	// Get the offer stack
	const vector<Order>& GetOfferStack() const {
		return offerStack;
	}

private:
	T product;
	vector<Order> bidStack;
	vector<Order> offerStack;

};


/**
 * Market Data Service which distributes market data
 * Keyed on product identifier.
 * Type T is the product type.
 */
class BondMarketDataServiceListener : public ServiceListener<OrderBook<Bond>> {
public:
	static BondMarketDataServiceListener* get_instance() {
		static BondMarketDataServiceListener instance;
		return &instance;
	}

	// pass the data on, to PositionService
	void ProcessAdd(OrderBook<Bond> &data) {};
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(OrderBook<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(OrderBook<Bond> &data) override;
private:
	BondMarketDataServiceListener();
	BondAlgoExecutionService * nextservice = nullptr;
};
template<typename T>
class MarketDataService : public Service<string, OrderBook <T> >
{

public:

	// Get the best bid/offer order
	virtual const BidOffer& GetBestBidOffer(const string &productId) = 0;

	// Aggregate the order book
	virtual const OrderBook<T>& AggregateDepth(const string &productId) = 0;

};

class BondMarketDataService : public MarketDataService<Bond>
{
public:
	static BondMarketDataService* get_instance() {
		static BondMarketDataService instance;
		return &instance;
	}
	virtual void OnMessage(OrderBook <Bond> &data);
	OrderBook<Bond>& GetData(string key) { return data.at(key); }
	void AddListener(ServiceListener<OrderBook<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<OrderBook<Bond>>* >& GetListeners() const { return _listeners; }
	// Get the best bid/offer order
	BidOffer& GetBestBidOffer(const string &productId) {
		return _bestbidoffer[productId];
	}
	// Aggregate the order book
	OrderBook<Bond>& AggregateDepth(const string &productId) {
	//TODO
		return data.at(productId);
	}
private:
	BondMarketDataService();
	vector<ServiceListener<OrderBook<Bond>>*> _listeners;
	map<string,BidOffer> _bestbidoffer;

};

class MarketDataConnector : public Connector<OrderBook <Bond>> {
public:
	static MarketDataConnector* get_instance() {
		static MarketDataConnector instance;
		return &instance;
	}
	virtual void Publish(OrderBook<Bond> &data) {}
	void GetMarketData();

private:
	MarketDataConnector();
	BondMarketDataService * service;
};

#endif
