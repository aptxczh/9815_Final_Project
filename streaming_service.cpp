#include "streaming_service.hpp"
#include "historical_data_service.hpp"

void StreamServiceListener::ProcessAdd(PriceStream<Bond>& data)
{
	cout << "In StreamServiceListener\n";
	string persist_key = "PersistID:" + data.GetProduct().GetProductId();
	service->PersistData(persist_key, data);
}

StreamServiceListener::StreamServiceListener()
{
	service = StreamHistoricalDataService::get_instance();
}
PriceStreamOrder::PriceStreamOrder(double _price, long _visibleQuantity, long _hiddenQuantity, PricingSide _side)
{
	price = _price;
	visibleQuantity = _visibleQuantity;
	hiddenQuantity = _hiddenQuantity;
	side = _side;
}

double PriceStreamOrder::GetPrice() const
{
	return price;
}

long PriceStreamOrder::GetVisibleQuantity() const
{
	return visibleQuantity;
}

long PriceStreamOrder::GetHiddenQuantity() const
{
	return hiddenQuantity;
}

//template<typename T>
//PriceStream<T>::PriceStream(const T &_product, const PriceStreamOrder &_bidOrder, const PriceStreamOrder &_offerOrder) :
//	product(_product), bidOrder(_bidOrder), offerOrder(_offerOrder)
//{
//}

//template<typename T>
//const T& PriceStream<T>::GetProduct() const
//{
//	return product;
//}

//template<typename T>
//const PriceStreamOrder& PriceStream<T>::GetBidOrder() const
//{
//	return bidOrder;
//}
//
//template<typename T>
//const PriceStreamOrder& PriceStream<T>::GetOfferOrder() const
//{
//	return offerOrder;
//}




