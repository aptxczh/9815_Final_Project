/**
 * executionservice.hpp
 * Defines the data types and Service for executions.
 *
 * @author Zhihao Chen
 */
#ifndef EXECUTION_SERVICE_HPP
#define EXECUTION_SERVICE_HPP

#include <string>
#include "soa.hpp"
#include "market_data_service.hpp"
#include "common_header.h"
#include "products.hpp"
enum OrderType { FOK, IOC, MARKET, LIMIT, STOP };

enum Market { BROKERTEC, ESPEED, CME };

/**
 * An execution order that can be placed on an exchange.
 * Type T is the product type.
 */
template<typename T>
class ExecutionOrder
{
public:
	ExecutionOrder(){}
	// ctor for an order
	ExecutionOrder(const T &_product, PricingSide _side, string _orderId,
		OrderType _orderType, double _price, double _visibleQuantity,
		double _hiddenQuantity, string _parentOrderId, bool _isChildOrder)
		:product(_product)
	{
		side = _side;
		orderId = _orderId;
		orderType = _orderType;
		price = _price;
		visibleQuantity = _visibleQuantity;
		hiddenQuantity = _hiddenQuantity;
		parentOrderId = _parentOrderId;
		isChildOrder = _isChildOrder;
	}

	// Get the product
	const T& GetProduct() const {
		return product;
	}

	// Get the order ID
	const string& GetOrderId() const {
		return orderId;
	}

	// Get the order type on this order
	OrderType GetOrderType() const {
		return orderType;
	}

	// Get the price on this order
	double GetPrice() const {
		return price;
	}

	// Get the visible quantity on this order
	long GetVisibleQuantity() const {
		return visibleQuantity;
	}

	// Get the hidden quantity
	long GetHiddenQuantity() const {
		return hiddenQuantity;
	}

	// Get the parent order ID
	const string& GetParentOrderId() const {
		return parentOrderId;
	}

	// Is child order?
	bool IsChildOrder() {
		return IsChildOrder;
	}

private:
	T product;
	PricingSide side;
	string orderId;
	OrderType orderType;
	double price;
	double visibleQuantity;
	double hiddenQuantity;
	string parentOrderId;
	bool isChildOrder;
};
class ExecutionServiceListener : public ServiceListener<ExecutionOrder<Bond>> {
public:
	static ExecutionServiceListener* get_instance() {
		static ExecutionServiceListener instance;
		return &instance;
	}

	// pass the data on, to PositionService
	void ProcessAdd(ExecutionOrder<Bond> &data) override;
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(ExecutionOrder<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(ExecutionOrder<Bond> &data) {}
private:
	ExecutionServiceListener();
	ExecutionHistoricalDataService * service = nullptr;
};
/**
 * Service for executing orders on an exchange.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class ExecutionService : public Service<string, ExecutionOrder <T> >
{

public:

	// Execute an order on a market
	virtual void ExecuteOrder(const ExecutionOrder<T>& order, Market market) = 0;

};
class BondExecutionService : public ExecutionService<Bond>
{
public:
	static BondExecutionService* get_instance() {
		static BondExecutionService instance;
		return &instance;
	}
	ExecutionOrder<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(ExecutionOrder<Bond> &trade) {}
	void AddListener(ServiceListener<ExecutionOrder<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<ExecutionOrder<Bond>>* >& GetListeners() const { return _listeners; }
	// Execute an order on a market
	void ExecuteOrder(const ExecutionOrder<Bond>& order, Market market);
private:
	BondExecutionService() {}
	vector<ServiceListener<ExecutionOrder<Bond>>*> _listeners;
};


#endif
