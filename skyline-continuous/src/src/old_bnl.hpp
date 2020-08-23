/*
 * old_bnl.hpp
 * Created on: June 27, 2018
 * Author: Oleksii Kulikov
 */

#ifndef OLD_BNL_H
#define OLD_BNL_H

#include <vector>

class OldBNL{
private:
	bool dominates(const std::vector<double> &dominator, const std::vector<double> &dominated);
public:
	std::vector<std::vector<double>> computeSkyline(const std::vector<std::vector<double>> &tuples, const std::size_t &n);
};

#endif // OLD_BNL_H

