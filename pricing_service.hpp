/**
 * pricingservice.hpp
 * Defines the data types and Service for internal prices.
 *
 * @author Breman Thuraisingham
 */
#ifndef PRICING_SERVICE_HPP
#define PRICING_SERVICE_HPP

#include <string>
#include "soa.hpp"
#include "common_header.h"

 /**
  * A price object consisting of mid and bid/offer spread.
  * Type T is the product type.
  */
template<typename T>
class Price
{

public:
	Price():product(T()){}
	// ctor for a price
	Price(const T &_product, double _mid, double _bidOfferSpread);

	// Get the product
	const T& GetProduct() const;

	// Get the mid price
	double GetMid() const;

	// Get the bid/offer spread around the mid
	double GetBidOfferSpread() const;

private:
	const T& product;
	double mid;
	double bidOfferSpread;

};

/**
 * Pricing Service managing mid prices and bid/offers.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class PricingService : public Service<string, Price <T> >
{
};

class BondPricingService : public PricingService<Bond>
{
public:
	static BondPricingService* get_instance() {
		static BondPricingService instance;
		return &instance;
	}
	Price<Bond>& GetData(string key) { return data[key]; }
	void AddListener(ServiceListener<Price<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<Price<Bond>>* >& GetListeners() const { return _listeners; }
	void OnMessage(Price<Bond> &trade) override;
private:
	BondPricingService() {}
	vector<ServiceListener<Price<Bond>>*> _listeners;
};

class PricingServiceConnector : public Connector<Price<Bond>> {
public:
	static PricingServiceConnector* get_instance() {
		static PricingServiceConnector instance;
		return &instance;
	}
	virtual void Publish(Price<Bond> &data) {}
	void GetPriceData();

private:
	PricingServiceConnector();
	BondPricingService *service;
};

template<typename T>
Price<T>::Price(const T &_product, double _mid, double _bidOfferSpread) :
	product(_product)
{
	mid = _mid;
	bidOfferSpread = _bidOfferSpread;
}

template<typename T>
const T& Price<T>::GetProduct() const
{
	return product;
}

template<typename T>
double Price<T>::GetMid() const
{
	return mid;
}

template<typename T>
double Price<T>::GetBidOfferSpread() const
{
	return bidOfferSpread;
}
#endif
