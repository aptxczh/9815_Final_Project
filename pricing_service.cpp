#include "pricing_service.hpp"
#include "products.hpp"
#include "streaming_service.hpp"
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

void PricingServiceConnector::GetPriceData()
{
	ifstream is("data/prices.csv");
	string line;
	getline(is, line); 	// skip the header
	string _cusip, _mid, _bidofferspread;

	while (getline(is, line)) {
		cout << "Got another price data.\n";
		vector<string> elems = split_one_line(line);
		std::tie(_cusip, _mid, _bidofferspread) = std::make_tuple(
			elems[0], elems[1], elems[2]);
		// get product from CUSIP
		Bond & b = bonds[_cusip];
		double mid_price = string_to_price(_mid);
		double spread = string_to_price(_bidofferspread);
		Price<Bond> price(b, mid_price, spread);
		service->OnMessage(price);
	};
}

PricingServiceConnector::PricingServiceConnector()
{
	service = BondPricingService::get_instance();
}

void BondPricingService::OnMessage(Price<Bond>& trade)
{
	cout << "in Pricing Service.\n";
	double mid = trade.GetMid();
	double spread = trade.GetBidOfferSpread();
	double offer = mid + spread / 2, bid = mid - spread / 2;
	// no size provided, therefore use 0...
	PriceStreamOrder bid_order(bid, 0, 0, PricingSide::BID);
	PriceStreamOrder offer_order(offer, 0, 0, PricingSide::OFFER);

	PriceStream<Bond> ps(trade.GetProduct(), bid_order, offer_order);
	BondStreamingService<Bond>::get_instance()->PublishPrice(ps);
}
