#include <fstream>
#include <string>
#include <sstream>
#include <random>
#include "common_header.h"
#include "products.hpp"
#include "position_service.hpp"
using namespace std;
map<string, Bond> bonds;
map<string, Position<Bond>> all_position;
enum Side { BUY, SELL };

// 2Y, 3Y, 5Y, 7Y, 10Y, and 30Y.
vector<string> CUS_IPs = {
	"912828U40", "912828U73",
	"912828U65", "912828U57",
	"912828U24", "912810RU4" };

void generate_trade_data() {
	ofstream os("data/trades.csv");
	os << "CUSIP,Trade_ID,Book,Price,Quantity,Side\n";

	for (int i = 1; i <= 6; i++) {
		string CUS_IP = CUS_IPs[i - 1];
		for (int j = 1; j <= 10; j++) {
			os << CUS_IP << ",T" << (i - 1) * 10 + j << ",TRSY" << 1 + rand() % 3
				<< ",";
			int  num = rand() % (256 * 2 + 1), num1 = num / 256, num2 = num % 256,
				num3 = num2 / 8, num4 = num2 % 8;
			string str1 = std::to_string(99 + num1) + "-",
				str2 = std::to_string(num3), str3 = std::to_string(num4);
			if (num4 == 4)	str3 = "+";
			if (num3 < 10) str2 = "0" + str2;
			string price = str1 + str2 + str3;
			os << price << "," << (1 + rand() % 9) * 1000000 << ","
				<< (rand() % 2 == 1 ? "BUY" : "SELL") << endl;
		}
	}
}

void generate_prices_data() {
	ofstream os("data/prices.csv");
	os << "CUSIP,mid,bidofferspread\n";

	for (int j = 1; j <= 100; j++) {
		for (int i = 1; i <= 6; i++) {
			string CUS_IP = CUS_IPs[i - 1];
			os << CUS_IP << ",";

			int mid_num = rand() % (256 * 2 - 8) + 4;
			string mid_str = randnum_to_string(mid_num);

			int oscillation = (rand() % 3 + 2);
			string osc_str = "0-00" + (oscillation == 4 ? "+" : std::to_string(oscillation));
			os << mid_str << ',' << osc_str << endl;
		}
	}
}

void generate_market_data() {
	ofstream os("data/marketdata.csv");
	os << "CUSIP,bidprice1,quantity,bidprice2,quantity,bidprice3,quantity,bidprice4,quantity,bidprice5,quantity,";
	os << "offerprice1,quantity,offerprice2,quantity,offerprice3,quantity,offerprice4,quantity,offerprice5,quantity,\n";
	// string price, quantity, side;
	for (int j = 1; j <= 10; j++) {
		for (int i = 1; i <= 6; i++) {
			string cus_ip = CUS_IPs[i - 1];
			os << cus_ip << ',';
			int mid_num = rand() % (256 * 2 + 1);

			// output bid prices in descending order
			int bid_num = mid_num - 1;
			for (int k = 1; k <= 5; k++) {
				string bid_price = randnum_to_string(bid_num);
				int quantity = 1000000 * k;
				os << bid_price << ',' << quantity << ',';
				bid_num--;
			}
			// output all offer prices in ascending order
			int offer_num = mid_num + 1;
			for (int k = 1; k <= 5; k++) {
				string offer_price = randnum_to_string(offer_num);
				int quantity = 1000000 * k;
				os << offer_price << ',' << quantity << ',';
				offer_num++;
			}
			os << endl;
		}
	}
}

void generate_inquiries_data() {
	ofstream os("data/inquiries.csv");
	os << "CUSIP, side, quantity, price, state\n";
	string cus_ip, _side, _quantity, _price, _state;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 6; j++) {
			cus_ip = CUS_IPs[j];
			if (rand() % 2 == 0)
				_side = "BUY";
			else _side = "SELL";
			_quantity = std::to_string(1000 * i);
			_price = "100";
			_state = "RECEIVED";
			os << cus_ip + ',' + _side + ',' + _quantity + ',' + _price + ',' + _state << endl;
		}
	}
}
// when reading csv, split one line with ','
vector<string> split_one_line(string &line) {
	stringstream ss(line);
	std::string item;
	vector<string> elems;
	while (getline(ss, item, ',')) {
		elems.push_back(item);
	}
	return elems;
}

// transform price like 99-31+ to double
double string_to_price(string str) {
	size_t idx = str.find_first_of('-');
	double result = std::stoi(str.substr(0, idx));
	int num1 = std::stoi(str.substr(idx + 1, 2));
	char ch = str[str.size() - 1];
	if (ch == '+') ch = '4';
	int num2 = ch - '0';
	result += (num1 * 8 + num2) / 256.0;
	return result;
}

// transform the generated random number into a price string
string randnum_to_string(int num) {
	int num1 = num / 256, num2 = num % 256,
		num3 = num2 / 8, num4 = num2 % 8;
	string str1 = std::to_string(99 + num1) + "-",
		str2 = std::to_string(num3), str3 = std::to_string(num4);
	if (num3 < 10) str2 = "0" + str2;
	if (num4 == 4)	str3 = "+";
	return str1 + str2 + str3;
}

void init_global_values()
{
	if (bonds.size() == 0 && all_position.size() == 0) {
		vector<float> _coupon = { 0,0,0,0,0,0 };
		vector<Bond> b = {
			Bond(CUS_IPs[0], CUSIP, "T", _coupon[0], date(2018, 11, 30)),
			Bond(CUS_IPs[1], CUSIP, "T", _coupon[1], date(2019, 12, 15)),
			Bond(CUS_IPs[2], CUSIP, "T", _coupon[2], date(2021, 11, 30)),
			Bond(CUS_IPs[3], CUSIP, "T", _coupon[3], date(2023, 11, 30)),
			Bond(CUS_IPs[4], CUSIP, "T", _coupon[4], date(2026, 12, 15)),
			Bond(CUS_IPs[5], CUSIP, "T", _coupon[5], date(2046, 12, 15))
		};
		for (int i = 0; i < 6; i++) {
			bonds[CUS_IPs[i]] = b[i];
			Position<Bond> pos(b[i]);
			pos.init_positions();
			all_position[CUS_IPs[i]] = pos;
		}
	}
}
