/**
 * inquiryservice.hpp
 * Defines the data types and Service for customer inquiries.
 *
 * @author Breman Thuraisingham
 */
#ifndef INQUIRY_SERVICE_HPP
#define INQUIRY_SERVICE_HPP

#include "soa.hpp"
#include "trade_booking_service.hpp"
#include "historical_data_service.hpp"

 // Various inqyury states
enum InquiryState { RECEIVED, QUOTED, DONE, REJECTED, CUSTOMER_REJECTED };

/**
 * Inquiry object modeling a customer inquiry from a client.
 * Type T is the product type.
 */
template<typename T>
class Inquiry
{

public:
	Inquiry(){}
	// ctor for an inquiry
	Inquiry(string _inquiryId, const T &_product, Side _side, long _quantity, double _price, InquiryState _state);

	// Get the inquiry ID
	const string& GetInquiryId() const;

	// Get the product
	const T& GetProduct() const;

	// Get the side on the inquiry
	Side GetSide() const;

	// Get the quantity that the client is inquiring for
	long GetQuantity() const;

	// Get the price that we have responded back with
	double GetPrice() const;

	// Get the current state on the inquiry
	InquiryState GetState() const;
	void SetState(InquiryState _state) {
		state = _state;
	}
private:
	string inquiryId;
	T product;
	Side side;
	long quantity;
	double price;
	InquiryState state;

};
class InquiryServiceListener : public ServiceListener<Inquiry<Bond>> {
public:
	static InquiryServiceListener* get_instance() {
		static InquiryServiceListener instance;
		return &instance;
	}

	// pass the data on, to PositionService
	void ProcessAdd(Inquiry<Bond> &data) override {
		string persist_key = "PersistID:" + data.GetProduct().GetProductId();
		service->PersistData(persist_key, data);
	}
	// Listener callback to process a remove event to the Service
	virtual void ProcessRemove(Inquiry<Bond> &data) {}

	// Listener callback to process an update event to the Service
	virtual void ProcessUpdate(Inquiry<Bond> &data) {}
private:
	InquiryServiceListener() { service = InquiryHistoricalDataService::get_instance(); }

	InquiryHistoricalDataService * service = nullptr;
};
/**
 * Service for customer inquirry objects.
 * Keyed on inquiry identifier (NOTE: this is NOT a product identifier since each inquiry must be unique).
 * Type T is the product type.
 */
template<typename T>
class InquiryService : public Service<string, Inquiry <T> >
{

public:

	// Send a quote back to the client
	virtual void SendQuote(const string &inquiryId, double price) = 0;

	// Reject an inquiry from the client
	virtual void RejectInquiry(const string &inquiryId) = 0;

};

class BondInquiryService : public InquiryService<Bond>
{
public:
	static BondInquiryService* get_instance() {
		static BondInquiryService instance;
		return &instance;
	}
	void OnMessage (Inquiry<Bond> &data) override {
		cout << "In InquiryService\n";
		data.SetState(InquiryState::DONE);
		for (auto listener : _listeners)
			listener->ProcessAdd(data);
	} 
	Inquiry<Bond>& GetData(string key) { return data[key]; }

	void AddListener(ServiceListener<Inquiry<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<Inquiry <Bond> >* >& GetListeners() const { return _listeners; }
	// Send a quote back to the client
	void SendQuote(const string &inquiryId, double price){}

	// Reject an inquiry from the client
	void RejectInquiry(const string &inquiryId){}
private:
	BondInquiryService() { AddListener(InquiryServiceListener::get_instance()); }
	vector<ServiceListener<Inquiry<Bond>>*> _listeners;
};


class InquiryConnector : public Connector<OrderBook <Bond>> {
public:
	static InquiryConnector* get_instance() {
		static InquiryConnector instance;
		return &instance;
	}
	virtual void Publish(OrderBook<Bond> &data) {}
	void GetInquiries() {
		static int inquiryId = 1;
		inquiryId++;
		ifstream is("data/inquiries.csv");
		string line;
		getline(is, line); 	// skip the header
		string _cusip, _mid, _bidofferspread;

		while (getline(is, line)) {
			cout << "Got another inquiry.\n";
			string _inquiryId = std::to_string(inquiryId);
			string cus_ip, side, quantity, price, state;
			vector<string> elems = split_one_line(line);
			std::tie(cus_ip, side, quantity, price, state) = std::make_tuple(
				elems[0], elems[1], elems[2], elems[3], elems[4]);
			Bond& _product = bonds[cus_ip];

			Side _side;
			long _quantity;
			double _price; InquiryState _state;

			_side = (side == "BUY" ? Side::BUY : Side::SELL);
			_quantity = stod(quantity);
			_price = stod(price);
			if(state == "RECEIVED") _state = InquiryState::QUOTED;
			else assert(false);

			Inquiry<Bond> inq(_inquiryId, _product, _side, _quantity, _price, _state);
			service->OnMessage(inq);
		}
	}

private:
	InquiryConnector() {
		service = BondInquiryService::get_instance();
	}
	BondInquiryService* service;
};


template<typename T>
Inquiry<T>::Inquiry(string _inquiryId, const T &_product, Side _side, long _quantity, double _price, InquiryState _state) :
	product(_product)
{
	inquiryId = _inquiryId;
	side = _side;
	quantity = _quantity;
	price = _price;
	state = _state;
}

template<typename T>
const string& Inquiry<T>::GetInquiryId() const
{
	return inquiryId;
}

template<typename T>
const T& Inquiry<T>::GetProduct() const
{
	return product;
}

template<typename T>
Side Inquiry<T>::GetSide() const
{
	return side;
}

template<typename T>
long Inquiry<T>::GetQuantity() const
{
	return quantity;
}

template<typename T>
double Inquiry<T>::GetPrice() const
{
	return price;
}

template<typename T>
InquiryState Inquiry<T>::GetState() const
{
	return state;
}

#endif
