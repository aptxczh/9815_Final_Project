/**
 * riskservice.hpp
 * Defines the data types and Service for fixed income risk.
 *
 * @author Breman Thuraisingham
 */
#ifndef RISK_SERVICE_HPP
#define RISK_SERVICE_HPP

#include "soa.hpp"
#include "position_service.hpp"
#include "products.hpp"

 /**
  * PV01 risk.
  * Type T is the product type.
  */
template<typename T>
class PV01
{

public:
	PV01(){}
	// ctor for a PV01 value
	PV01(const vector<T> &_products, double _pv01, long _quantity);

	// Get the products associated with this PV01 value
	const vector<T>& GetProducts() const;

	// Get the PV01 value
	double GetPV01() const;

	// Get the quantity that this risk value is associated with
	long GetQuantity() const;

private:
	vector<T> products;
	double pv01;
	long quantity;

};

/**
* A bucket sector to bucket a group of securities.
* We can then aggregate bucketed risk to this bucket.
* Type T is the product type.
*/
template<typename T>
class BucketedSector
{

public:

	// ctor for a bucket sector
	BucketedSector(const vector<T> &_products, string _name);

	// Get the products associated with this bucket
	const vector<T>& GetProducts() const;

	// Get the name of the bucket
	const string& GetName() const;

private:
	vector<T> products;
	string name;

};

class RiskServiceListener : public ServiceListener<PV01<Bond>> {
public:
	static RiskServiceListener* get_instance() {
		static RiskServiceListener instance;
		return &instance;
	}

	// pass the data on, to PositionService
	void ProcessAdd(PV01<Bond> &data) override;
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(PV01<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(PV01<Bond> &data) {}
private:
	RiskServiceListener();

	RiskHistoricalDataService * service = nullptr;
};
/**
 * Risk Service to vend out risk for a particular security and across a risk bucketed sector.
 * Keyed on product identifier.
 * Type T is the product type.
 */
template<typename T>
class RiskService : public Service<string, PV01 <T> >
{

public:

	// Add a position that the service will risk
	virtual void AddPosition(Position<T> &position) = 0;

	// Get the bucketed risk for the bucket sector
	virtual const PV01<T>& GetBucketedRisk(const BucketedSector<T> &sector) const = 0;

};
class BondRiskService : public RiskService<Bond>
{
public:
	static BondRiskService* get_instance() {
		static BondRiskService instance;
		return &instance;
	}

	// Add a position that the service will risk
	void AddPosition(Position<Bond> &position);
	PV01<Bond>& GetData(string key) {
		return data[key];
	}
	void OnMessage(PV01<Bond> &trade) {}
	void AddListener(ServiceListener<PV01<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<PV01<Bond>>* >& GetListeners() const { return _listeners; }
	// Get the bucketed risk for the bucket sector
	const PV01<Bond>& GetBucketedRisk(const BucketedSector<Bond> &sector) const ;
private:
	BondRiskService() {}
	vector<ServiceListener<PV01<Bond>>*> _listeners;
};

template<typename T>
PV01<T>::PV01(const vector<T> &_products, double _pv01, long _quantity) :
	products(_products)
{
	pv01 = _pv01;
	quantity = _quantity;
}

template<typename T>
const vector<T>& PV01<T>::GetProducts() const
{
	return products;
}

template<typename T>
double PV01<T>::GetPV01() const
{
	return pv01;
}

template<typename T>
long PV01<T>::GetQuantity() const
{
	return quantity;
}

template<typename T>
BucketedSector<T>::BucketedSector(const vector<T>& _products, string _name) :
	products(_products)
{
	name = _name;
}

template<typename T>
const vector<T>& BucketedSector<T>::GetProducts() const
{
	return products;
}

template<typename T>
const string& BucketedSector<T>::GetName() const
{
	return name;
}
#endif
