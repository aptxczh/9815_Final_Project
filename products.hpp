/**
 * products.hpp
 * Defines Bond and Interest Rate Swap products.
 *
 * @author Breman Thuraisingham
 */
#ifndef PRODUCTS_HPP
#define PRODUCTS_HPP

#include <iostream>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::gregorian;

enum ProductType { IRSWAP, BOND };

/**
 * Base class for a product.
 */
class Product
{

public:

	// ctor for a prduct
	Product(string _productId, ProductType _productType);

	// Get the product identifier
	const string& GetProductId() const;

	// Ge the product type
	ProductType GetProductType() const;

private:
	string productId;
	ProductType productType;

};

enum BondIdType { CUSIP, ISIN };

/**
 * Bond product class
 */
class Bond : public Product
{
public:
	// ctor for a bond
	Bond(string _productId, BondIdType _bondIdType, string _ticker, float _coupon, date _maturityDate);
	Bond();

	// Get the ticker
	const string& GetTicker() const;

	// Get the coupon
	float GetCoupon() const;

	// Get the maturity date
	const date& GetMaturityDate() const;

	// Get the bond identifier type
	BondIdType GetBondIdType() const;

	// Print the bond
	friend ostream& operator<<(ostream &output, const Bond &bond);

private:
	string productId;
	BondIdType bondIdType;
	string ticker;
	float coupon;
	date maturityDate;
};

/**
 * Interest Rate Swap enums
 */
enum DayCountConvention { THIRTY_THREE_SIXTY, ACT_THREE_SIXTY };
enum PaymentFrequency { QUARTERLY, SEMI_ANNUAL, ANNUAL };
enum FloatingIndex { LIBOR, EURIBOR };
enum FloatingIndexTenor { TENOR_1M, TENOR_3M, TENOR_6M, TENOR_12M };
enum Currency { USD, EUR, GBP };
enum SwapType { STANDARD, FORWARD, IMM, MAC, BASIS };
enum SwapLegType { OUTRIGHT, CURVE, FLY };


/**
 * Interest Rate Swap product
 */
class IRSwap : public Product
{
public:

	// ctor for a swap
	IRSwap(string productId, DayCountConvention _fixedLegDayCountConvention, DayCountConvention _floatingLegDayCountConvention, PaymentFrequency _fixedLegPaymentFrequency, FloatingIndex _floatingIndex, FloatingIndexTenor _floatingIndexTenor, date _effectiveDate, date _terminationDate, Currency _currency, int termYears, SwapType _swapType, SwapLegType _swapLegType);
	IRSwap();

	// Get the fixed leg daycount convention
	DayCountConvention GetFixedLegDayCountConvention() const;

	// Get the floating leg daycount convention
	DayCountConvention GetFloatingLegDayCountConvention() const;

	// Get the payment frequency on the fixed leg
	PaymentFrequency GetFixedLegPaymentFrequency() const;

	// Get the flaotig leg index
	FloatingIndex GetFloatingIndex() const;

	// Get the floating leg index tenor
	FloatingIndexTenor GetFloatingIndexTenor() const;

	// Get the effective date
	const date& GetEffectiveDate() const;

	// Get the termination date
	const date& GetTerminationDate() const;

	// Get the currency
	Currency GetCurrency() const;

	// Get the term in years
	int GetTermYears() const;

	// Get the swap type
	SwapType GetSwapType() const;

	// Get the swap leg type
	SwapLegType GetSwapLegType() const;

	// Print the swap
	friend ostream& operator<<(ostream &output, const IRSwap &swap);

private:
	DayCountConvention fixedLegDayCountConvention;
	DayCountConvention floatingLegDayCountConvention;
	PaymentFrequency fixedLegPaymentFrequency;
	FloatingIndex floatingIndex;
	FloatingIndexTenor floatingIndexTenor;
	date effectiveDate;
	date terminationDate;
	Currency currency;
	int termYears;
	SwapType swapType;
	SwapLegType swapLegType;
	string ToString(DayCountConvention dayCountConvention) const;
	string ToString(PaymentFrequency paymentFrequency) const;
	string ToString(FloatingIndex floatingIndex) const;
	string ToString(FloatingIndexTenor floatingIndexTenor) const;
	string ToString(Currency currency) const;
	string ToString(SwapType swapType) const;
	string ToString(SwapLegType swapLegType) const;
};


#endif
