/**
 * tradebookingservice.hpp
 * Defines the data types and Service for trade booking.
 *
 * @author Breman Thuraisingham
 */
#ifndef TRADE_BOOKING_SERVICE_HPP
#define TRADE_BOOKING_SERVICE_HPP


#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include "soa.hpp"
#include "common_header.h"
#include "products.hpp"
#include "position_service.hpp"

using namespace std;

// Trade sides
enum Side { BUY, SELL };

/**
 * Trade object with a price, side, and quantity on a particular book.
 * Type T is the product type.
 */
template<typename T>
class Trade
{
public:
	Trade() {}
	// ctor for a trade
	Trade(const T &_product, string _tradeId, string _book, double _price, long _quantity, Side _side) :
		product(_product)
	{
		trade_id = _tradeId;
		book = _book;
		price = _price;
		quantity = _quantity;
		side = _side;
	}

	// Get the product
	const T& GetProduct() const {
		return product;
	}

	// Get the trade ID
	const string& GetTradeId() const {
		return trade_id;
	}

	// Get the book
	const string& GetBook() const {

		return book;
	}

	double GetPrice() const {
		return price;
	}

	// Get the quantity
	long GetQuantity() const {
		return quantity;
	}

	// Get the side
	Side GetSide() const {
		return side;
	}

private:
	T product;
	string trade_id;
	string book;
	double price;
	long quantity;
	Side side;
};

class BondTradeServiceListener : public ServiceListener<Trade<Bond>> {
public:
	static BondTradeServiceListener* get_instance() {
		static BondTradeServiceListener instance;
		return &instance;
	}

	// pass the data on, to PositionService
	void ProcessAdd(Trade<Bond> &data) override;
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(Trade<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(Trade<Bond> &data) {}
private:
	BondTradeServiceListener();

	BondPositionService * service = nullptr;
};

/**
 * Trade Booking Service to book trades to a particular book.
 * Keyed on product identifier.
 * Type T is the product type.
 */


template<typename T>
class TradeBookingService : public Service<string, Trade <T> >
{

public:

	// Book the trade
	virtual void BookTrade(const Trade<T> &trade) = 0;

};

class BondTradeBookingService : public TradeBookingService<Bond>
{//Singleton
public:
	static BondTradeBookingService* get_instance() {
		static BondTradeBookingService instance;
		return &instance;
	}

	// Called by connector, and call the private method booktrade
	Trade<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(Trade<Bond> &trade) override;
	void AddListener(ServiceListener<Trade<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<Trade<Bond>>* >& GetListeners() const { return _listeners; }

private:
	// Book the trade, by passing the trade data to listener
	void BookTrade(const Trade<Bond> &trade);
	vector<ServiceListener<Trade<Bond>>*> _listeners;
	BondTradeBookingService();
};
//template<typename T>
class BondTradeBookingConnector : public Connector<Trade<Bond>> {
public:
	static BondTradeBookingConnector* get_instance() {
		static BondTradeBookingConnector instance;
		return &instance;
	}
	void Publish(Trade<Bond> &data) {};
	void GetTradingData();

private:
	BondTradeBookingConnector();
	BondTradeBookingService *service;
};




#endif
