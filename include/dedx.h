#ifndef __DEDX_H
#define __DEDX_H

#include <fstream>
#include <string>
#include <array>
#include <utility>
#include <map>
#include "IrisMaterial.h"

class dedx
{
public:
	dedx();			   //! Create
	virtual ~dedx() {} //!

	void loadIncomingELoss(std::string, std::string, std::string, std::string, double);
	void loadOutgoingELoss(std::string, std::string, std::string, std::string, double);
	void loadELoss(std::string, IrisMaterial, double);
	void makeCatimaTables(int projectileA, int projectileZ, std::string foil, std::string target, double mass);
	std::array<double, 100> GetE(IrisMaterial material) { return tables[material].first; }
	std::array<double, 100> GetDeDx(IrisMaterial material) { return tables[material].second; }
	void Clear(); //!
protected:
private:
	std::map<IrisMaterial, std::pair<std::array<double, 100>, std::array<double, 100>>> tables;
	std::array<double, 100> energy = {0.001000, 0.002000, 0.005000, 0.008000, 0.010000, 0.015000, 0.020000, 0.025000, 0.030000, 0.035000, 0.040000, 0.045000, 0.050000, 0.055000, 0.060000, 0.070000, 0.080000, 0.090000, 0.100000, 0.120000, 0.140000, 0.160000, 0.180000, 0.200000, 0.250000, 0.300000, 0.350000, 0.400000, 0.450000, 0.500000, 0.600000, 0.700000, 0.800000, 0.900000, 1.000000, 1.250000, 1.500000, 1.750000, 2.000000, 2.250000, 2.500000, 3.000000, 3.500000, 4.000000, 4.500000, 5.000000, 5.500000, 6.000000, 6.500000, 7.000000, 7.500000, 8.000000, 8.500000, 9.000000, 9.500000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000, 17.000000, 18.000000, 19.000000, 20.000000, 21.000000, 22.000000, 23.000000, 24.000000, 25.000000, 27.000000, 29.000000, 31.000000, 33.000000, 35.000000, 38.000000, 41.000000, 44.000000, 47.000000, 50.000000, 55.000000, 60.000000, 65.000000, 70.000000, 75.000000, 80.000000, 85.000000, 90.000000, 95.000000, 100.000000, 105.000000, 110.000000, 115.000000, 120.000000, 125.000000, 130.000000, 135.000000, 140.000000, 145.000000};
};

#endif
