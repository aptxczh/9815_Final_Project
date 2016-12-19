/**
 * historicaldataservice.hpp
 * @author Breman Thuraisingham
 * Defines the data types and Service for historical data.
 *
 * @author Zhihao Chen
 */
#ifndef HISTORICAL_DATA_SERVICE_HPP
#define HISTORICAL_DATA_SERVICE_HPP
#include "common_header.h"
#include "products.hpp"

class ExecutionDataConnector;
class RiskDataConnector;
class StreamingDataConnector;
class InquiryDataConnector;
template <class T>
class PV01;
template <class T>
class PriceStream;
template <class T>
class Inquiry;

 /**
  * Service for processing and persisting historical data to a persistent store.
  * Keyed on some persistent key.
  * Type T is the data type to persist.
  */

template<typename T>
class HistoricalDataService : Service<string, T>
{

public:

	// Persist data to a store
	virtual void PersistData(string persistKey, const T& data) = 0;

};

//TODO: Split BondHistoricalDataService into 5 parts
class RiskHistoricalDataService : public HistoricalDataService<PV01<Bond>>
{
public:
	static RiskHistoricalDataService* get_instance() {
		static RiskHistoricalDataService instance;
		return &instance;
	}

	// Persist data to a store
	PV01<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(PV01<Bond> &trade) {}
	void AddListener(ServiceListener<PV01<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<PV01<Bond>>* >& GetListeners() const { return _listeners; }
	// Execute an order on a market
	void PersistData(string persistKey, const PV01<Bond>& data);
	//void PersistExecutionData(string msg);
	//void PersistRiskData(PV01<Bond> & pv01);
	//void PersistStreamingData(PriceStream<Bond> &ps);
	//void PersistInquiryData(Inquiry<Bond>& msg);
private:
	RiskHistoricalDataService();
	//ExecutionDataConnector *exe_connector;
	//RiskDataConnector *risk_connector; 
	//StreamingDataConnector *streaming_connector;
	//InquiryDataConnector *inquiry_connector;
	RiskDataConnector* connector;
	vector<ServiceListener<PV01<Bond>>*> _listeners;
};
//TODO: Split BondHistoricalDataService into 5 parts
class ExecutionHistoricalDataService : public HistoricalDataService<ExecutionOrder<Bond>>
{
public:
	static ExecutionHistoricalDataService* get_instance() {
		static ExecutionHistoricalDataService instance;
		return &instance;
	}

	// Persist data to a store
	ExecutionOrder<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(ExecutionOrder<Bond> &trade) {}
	void AddListener(ServiceListener<ExecutionOrder<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<ExecutionOrder<Bond>>* >& GetListeners() const { return _listeners; }
	// Execute an order on a market
	void PersistData(string persistKey, const ExecutionOrder<Bond>& data);
	//void PersistExecutionData(string msg);
	//void PersistRiskData(PV01<Bond> & pv01);
	//void PersistStreamingData(PriceStream<Bond> &ps);
	//void PersistInquiryData(Inquiry<Bond>& msg);
private:
	ExecutionHistoricalDataService();
	//ExecutionDataConnector *exe_connector;
	//RiskDataConnector *risk_connector;
	//StreamingDataConnector *streaming_connector;
	//InquiryDataConnector *inquiry_connector;
	vector<ServiceListener<ExecutionOrder<Bond>>*> _listeners;
	ExecutionDataConnector* connector;
};
class PositionHistoricalDataService : public HistoricalDataService<Position<Bond>>
{
public:
	static PositionHistoricalDataService* get_instance() {
		static PositionHistoricalDataService instance;
		return &instance;
	}

	// Persist data to a store
	Position<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(Position<Bond> &trade) {}
	void AddListener(ServiceListener<Position<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<Position<Bond>>* >& GetListeners() const { return _listeners; }
	// Execute an order on a market
	void PersistData(string persistKey, const Position<Bond>& data);
	//void PersistExecutionData(string msg);
	//void PersistRiskData(PV01<Bond> & pv01);
	//void PersistStreamingData(PriceStream<Bond> &ps);
	//void PersistInquiryData(Inquiry<Bond>& msg);
private:
	PositionHistoricalDataService();
	//ExecutionDataConnector *exe_connector;
	//RiskDataConnector *risk_connector;
	//StreamingDataConnector *streaming_connector;
	//InquiryDataConnector *inquiry_connector;
	vector<ServiceListener<Position<Bond>>*> _listeners;
	PositionDataConnector* connector;
};
class StreamHistoricalDataService : public HistoricalDataService<PriceStream<Bond>>
{
public:
	static StreamHistoricalDataService* get_instance() {
		static StreamHistoricalDataService instance;
		return &instance;
	}

	// Persist data to a store
	PriceStream<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(PriceStream<Bond> &trade) {}
	void AddListener(ServiceListener<PriceStream<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<PriceStream<Bond>>* >& GetListeners() const { return _listeners; }
	// Execute an order on a market
	void PersistData(string persistKey, const PriceStream<Bond>& data);
	//void PersistExecutionData(string msg);
	//void PersistRiskData(PV01<Bond> & pv01);
	//void PersistStreamingData(PriceStream<Bond> &ps);
	//void PersistInquiryData(Inquiry<Bond>& msg);
private:
	StreamHistoricalDataService();
	//ExecutionDataConnector *exe_connector;
	//RiskDataConnector *risk_connector;
	//StreamingDataConnector *streaming_connector;
	//InquiryDataConnector *inquiry_connector;
	vector<ServiceListener<PriceStream<Bond>>*> _listeners;
	StreamingDataConnector* connector;
};
class InquiryHistoricalDataService : public HistoricalDataService<Inquiry<Bond>>
{
public:
	static InquiryHistoricalDataService* get_instance() {
		static InquiryHistoricalDataService instance;
		return &instance;
	}

	// Persist data to a store
	Inquiry<Bond>& GetData(string key) { return data[key]; }
	void OnMessage(Inquiry<Bond> &trade) {}
	void AddListener(ServiceListener<Inquiry<Bond>> *listener) { _listeners.push_back(listener); }
	virtual const vector< ServiceListener<Inquiry<Bond>>* >& GetListeners() const { return _listeners; }
	// Execute an order on a market
	void PersistData(string persistKey, const Inquiry<Bond>& data);
	//void PersistExecutionData(string msg);
	//void PersistRiskData(PV01<Bond> & pv01);
	//void PersistStreamingData(PriceStream<Bond> &ps);
	//void PersistInquiryData(Inquiry<Bond>& msg);
private:
	InquiryHistoricalDataService();
	//ExecutionDataConnector *exe_connector;
	//RiskDataConnector *risk_connector;
	//StreamingDataConnector *streaming_connector;
	//InquiryDataConnector *inquiry_connector;
	vector<ServiceListener<Inquiry<Bond>>*> _listeners;
	InquiryDataConnector* connector;
};
class ExecutionDataConnector : Connector<string>
{
public:
	static ExecutionDataConnector *get_instance() {
		static ExecutionDataConnector instance;
		return &instance;
	}
	virtual void Publish(string &data);
private:
	ExecutionDataConnector() {}
};


class RiskDataConnector : Connector<string>
{
public:
	static RiskDataConnector *get_instance() {
		static RiskDataConnector instance;
		return &instance;
	}
	virtual void Publish(string &data);
private:
	RiskDataConnector() {}
};


class StreamingDataConnector : Connector<string>
{
public:
	static StreamingDataConnector *get_instance() {
		static StreamingDataConnector instance;
		return &instance;
	}
	virtual void Publish(string &data);
private:
	StreamingDataConnector() {}
};


class InquiryDataConnector : Connector<string>
{
public:
	static InquiryDataConnector *get_instance() {
		static InquiryDataConnector instance;
		return &instance;
	}
	virtual void Publish(string &data);
private:
	InquiryDataConnector() {}
};
class PositionDataConnector : Connector<string>
{
public:
	static PositionDataConnector *get_instance() {
		static PositionDataConnector instance;
		return &instance;
	}
	virtual void Publish(string &data);
private:
	PositionDataConnector() {}
};

#endif
