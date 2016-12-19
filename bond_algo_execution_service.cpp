#include "bond_algo_execution_service.hpp"
#include "execution_service.hpp"
#include <string>
#include "products.hpp"
#include "position_service.hpp"

void BondAlgoExecutionServiceListener::ProcessUpdate(OrderBook<Bond>& data)
{
	cout << "in BondAlgoExecutionServiceListener\n";
	static bool bid = false;
	static int order_id = 1;
	bid = !bid;


	Bond b = data.GetProduct();
	PricingSide _side;
	string _orderId;
	OrderType _orderType;
	double _price, _visibleQuantity;
	double _hiddenQuantity = 0; string _parentOrderId = "";
	bool _isChildOrder = false;

	_orderId = std::to_string(order_id++);
	string cus_id = b.GetProductId();
	long position = all_position[cus_id].GetAggregatePosition();
	// use Market order in default; 
	_orderType = OrderType::MARKET;

	// alternating between BID and OFFER
	if (bid) {
		_side = BID;
		const Order & bid_order = data.GetBidStack()[0];
		_price = bid_order.GetPrice();
		_visibleQuantity = bid_order.GetQuantity();
		if (_visibleQuantity > position)
			_visibleQuantity = position;
	}
	else {
		_side = OFFER;
		const Order & offer_order = data.GetOfferStack()[0];
		_price = offer_order.GetPrice();
		_visibleQuantity = offer_order.GetQuantity();
	}

	ExecutionOrder<Bond> order(b, _side, _orderId, _orderType, _price, _visibleQuantity,
		_hiddenQuantity, _parentOrderId, _isChildOrder);
	exec_service->ExecuteOrder(order, Market::BROKERTEC);
}

BondAlgoExecutionServiceListener::BondAlgoExecutionServiceListener() {
	exec_service = BondExecutionService::get_instance();
}

void BondAlgoExecutionService::OnMessage(OrderBook<Bond>& data) {
	cout << "Executing orderbook in AlgoExecutionService\n";
	for (auto listener : _listeners)
	{
		listener->ProcessUpdate(data);
	}

}

BondAlgoExecutionService::BondAlgoExecutionService() {
	AddListener(BondAlgoExecutionServiceListener::get_instance());
}

