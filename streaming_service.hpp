/**
 * streamingservice.hpp
 * Defines the data types and Service for price streams.
 *
 * @author Breman Thuraisingham
 */
#ifndef STREAMING_SERVICE_HPP
#define STREAMING_SERVICE_HPP

#include "soa.hpp"
#include "market_data_service.hpp"
#include "historical_data_service.hpp"

 /**
  * A price stream order with price and quantity (visible and hidden)
  */
class PriceStreamOrder
{

public:
	PriceStreamOrder(){}
	// ctor for an order
	PriceStreamOrder(double _price, long _visibleQuantity, long _hiddenQuantity, PricingSide _side);

	// The side on this order
	PricingSide GetSide() const { return side; }

	// Get the price on this order
	double GetPrice() const;

	// Get the visible quantity on this order
	long GetVisibleQuantity() const;

	// Get the hidden quantity on this order
	long GetHiddenQuantity() const;

private:
	double price;
	long visibleQuantity;
	long hiddenQuantity;
	PricingSide side;

};

/**
* Price Stream with a two-way market.
* Type T is the product type.
*/
template<typename T>
class PriceStream
{

public:
	PriceStream(){}
	// ctor
	PriceStream(const T &_product, const PriceStreamOrder &_bidOrder, const PriceStreamOrder &_offerOrder) :product(_product), bidOrder(_bidOrder), offerOrder(_offerOrder) {}

	// Get the product
	const T& GetProduct() const {
		return product;
	}

	// Get the bid order
	const PriceStreamOrder& GetBidOrder() const {
		return bidOrder;
	}

	// Get the offer order
	const PriceStreamOrder& GetOfferOrder() const {
		return offerOrder;
	}

private:
	T product;
	PriceStreamOrder bidOrder;
	PriceStreamOrder offerOrder;

};
/**
* Streaming service to publish two-way prices.
* Keyed on product identifier.
* Type T is the product type.
*/
class StreamServiceListener : public ServiceListener<PriceStream<Bond>> {
public:
	static StreamServiceListener* get_instance() {
		static StreamServiceListener instance;
		return &instance;
	}

	// pass the data on, to PositionService
	void ProcessAdd(PriceStream<Bond> &data) override;
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(PriceStream<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(PriceStream<Bond> &data) {}
private:
	StreamServiceListener();

	StreamHistoricalDataService * service = nullptr;
};
template<typename T>
class BondStreamingService : public Service<string, PriceStream <T> >
{
public:
	static BondStreamingService<T> *get_instance() {
		static BondStreamingService<T> instance;
		return &instance;
	}
	PriceStream<T>& GetData(string key) { return data[key]; }
	void OnMessage(PriceStream<T> &trade) {};
	void AddListener(ServiceListener<PriceStream<T>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<PriceStream<T>>* >& GetListeners() const { return _listeners; }
	// Publish two-way prices
	void PublishPrice(PriceStream<T> priceStream) {//TODO: Use Listener Instead
		cout << "in StreamingService\n";
		for (auto listener : _listners)
		{
			listener->ProcessAdd(priceStream);
		}
	}
private:
	BondStreamingService() { AddListener(StreamServiceListener::get_instance()); }
	vector<ServiceListener<PriceStream<Bond>>*> _listeners;
};


#endif
