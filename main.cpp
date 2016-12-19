#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include "trade_booking_service.hpp"
#include "pricing_service.hpp"
#include "common_header.h"
#include "products.hpp"
#include "market_data_service.hpp"
#include "inquiry_service.hpp"

using namespace std;

int main()
{
	generate_prices_data();
	generate_trade_data();
	generate_market_data();
	generate_inquiries_data();

	init_global_values();
	InquiryConnector::get_instance()->GetInquiries();
	PricingServiceConnector::get_instance()->GetPriceData();
	BondTradeBookingConnector::get_instance()->GetTradingData();
	MarketDataConnector::get_instance()->GetMarketData();

}