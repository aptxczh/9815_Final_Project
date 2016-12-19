/**
 * positionservice.hpp
 * Defines the data types and Service for positions.
 *
 * @author Breman Thuraisingham
 */
#ifndef POSITION_SERVICE_HPP
#define POSITION_SERVICE_HPP
#include <string>
#include <map>
#include "common_header.h"
#include <cassert>
using namespace std;

/**
 * Position class in a particular book.
 * Type T is the product type.
 */
template<typename T>
class Position
{

public:
	Position(){}
	// ctor for a position
	Position(const T &_product);

	// Get the product
	const T& GetProduct() const;

	// Get the position quantity
	long GetPosition(string &book);

	// Get the aggregate position
	long GetAggregatePosition();
	void init_positions() {
		string s = "TRSY";
		for (int i = 1; i <= 3; i++)
			positions[s + std::to_string(i)] = 10000000;
	}

	void AddPosition(string book_id, int amount) {
		assert(positions.find(book_id) != positions.end());
		positions[book_id] += amount;
	}
private:
	T product;
	map<string, long> positions;

};

class PositionServiceListener : public ServiceListener<Position<Bond>> {
public:
	static PositionServiceListener* get_instance() {
		static PositionServiceListener instance;
		return &instance;
	}
	// pass the data on, to RiskService
	void ProcessAdd(Position<Bond> &data) override;
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(Position<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(Position<Bond> &data) {}
private:
	PositionServiceListener();
	BondRiskService *service = nullptr;
	PositionHistoricalDataService * hiservice = nullptr;
};
/**
 * Position Service to manage positions across multiple books and secruties.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PositionService : public Service<string, Position <T> >
{

public:

	// Add a trade to the service
	virtual void AddTrade(const Trade<T> &trade) = 0;

};
class BondPositionService : public PositionService<Bond>
{
public:
	static BondPositionService* get_instance() {
		static BondPositionService instance;
		return &instance;
	}
	Position<Bond>& GetData(string key) { return data[key]; }
	void AddListener(ServiceListener<Position<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<Position<Bond>>* >& GetListeners() const { return _listeners; }
	void OnMessage(Position<Bond> &data) override;

	// Add a trade to the service
	// TODO replace AddTrade with OnMessage
	void AddTrade(const Trade<Bond> &trade);

private:
	BondPositionService();
	vector<ServiceListener<Position<Bond>>*> _listeners;
};


template<typename T>
Position<T>::Position(const T &_product) :
	product(_product)
{
}

template<typename T>
const T& Position<T>::GetProduct() const
{
	return product;
}

template<typename T>
long Position<T>::GetPosition(string &book)
{
	return positions[book];
}

template<typename T>
long Position<T>::GetAggregatePosition()
{
	long result = 0;
	string s = "TRSY";
	for (int i = 1; i <= 3; i++) {
		string key = s + std::to_string(i);
		result += positions.at(key);
	}
	return result;
}
#endif
