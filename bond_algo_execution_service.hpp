#ifndef _BOND_ALGO_EXECUTION_SERVICE_HPP_
#define _BOND_ALGO_EXECUTION_SERVICE_HPP_

#include "soa.hpp"
#include "common_header.h"
//#include "market_data_service.hpp"
//#include "execution_service.hpp"

class BondAlgoExecutionServiceListener : public ServiceListener<OrderBook<Bond>> {
public:
	static BondAlgoExecutionServiceListener* get_instance() {
		static BondAlgoExecutionServiceListener instance;
		return &instance;
	}

	// update the orderbook
	void ProcessUpdate(OrderBook<Bond> &data) override;
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(OrderBook<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessAdd(OrderBook<Bond> &data) {}

private:
	BondAlgoExecutionServiceListener();
	BondExecutionService *exec_service;
};

class BondAlgoExecutionService : public Service<string, OrderBook<Bond>>
{
public:
	static BondAlgoExecutionService* get_instance() {
		static BondAlgoExecutionService instance;
		return &instance;
	}
	OrderBook<Bond>& GetData(string key) { return data[key]; }
	void AddListener(ServiceListener<OrderBook<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<OrderBook<Bond>>* >& GetListeners() const { return _listeners; }
	virtual void OnMessage(OrderBook<Bond> &data) override;


private:
	BondAlgoExecutionService();
	vector<ServiceListener<OrderBook<Bond>>*> _listeners;
	// PositionService *pos_service;
};


#endif
