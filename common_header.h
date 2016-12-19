/**
 * Created by Zengye (Arthur) Wang, on 2015-12-19
 * Include necessary declarations.
 */
#ifndef _COMMON_HEADER_H_
#define _COMMON_HEADER_H_
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include "soa.hpp"

using namespace std;

template<typename K, typename V>
class Service;

// template<typename T>
class BondTradeBookingService;
class BondTradeServiceListener;
class BondTradeBookingConnector;


class BondPositionService;
class BondPositionServiceListener;
class BondPositionService;

class BondRiskService;
class HistoricalDataServiceListener;
class BondHistoricalDataService;
class BondAlgoExecutionService;
class BondAlgoExecutionServiceListener;
class BondExecutionService;

template<typename T>
class OrderBook;

template<typename T>
class Trade;

template<typename T>
class Position;

enum Side;
enum PricingSide;
class Bond;

void generate_trade_data();
void generate_prices_data();
void generate_market_data();
void generate_inquiries_data();

// when reading csv, split one line with ',' then return a vector of strings
vector<string> split_one_line(string &line);

// transform price like 99-31+ to double
double string_to_price(string str);

// transform the generated random number into a price string
string randnum_to_string(int num);

// global variables; Use CUS_IP to access bonds and positions
extern map<string, Bond> bonds;
extern map<string, Position<Bond>> all_position;

// init global variables at the first 
void init_global_values();
void generate_prices_data();


#endif

