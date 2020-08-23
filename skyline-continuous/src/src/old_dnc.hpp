/*
 * old_dnc.hpp
 * Created on: June 27, 2018
 * Author: Oleksii Kulikov
 */

#ifndef OLD_DNC_H
#define OLD_DNC_H

#include <algorithm>

class OldDNC{
private:
	bool dominates(const std::vector<double> &dominator, const std::vector<double> &dominated);
	std::vector<double> median(std::vector<std::vector<double>> tuples, const int &dimension);
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> partition(const std::vector<std::vector<double>> &tuples, const int &dimension, const std::vector<double> &pivot);
	std::vector<std::vector<double>> mergeBasic(std::vector<std::vector<double>> S_1, const std::vector<std::vector<double>> &S_2, const int &dimension);
public:
	std::vector<std::vector<double>> computeSkyline(const std::vector<std::vector<double>> &M, const int &dimension);
};

#endif // OLD_DNC_H

