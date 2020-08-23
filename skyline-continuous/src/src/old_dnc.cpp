/*
 * old_dnc.cpp
 * Created on: June 27, 2018
 * Author: Oleksii Kulikov
 */

#include "old_dnc.hpp"

/**
 * Checks whether one tuple dominates the other and returns true|false
 * @param dominator the tuple to check for dominating
 * @param dominated the tuple to check for being dominated
 */
bool OldDNC::dominates(const std::vector<double> &dominator, const std::vector<double> &dominated){
	// dominated not worse in all
	bool flag = true;
	for(std::vector<int>::size_type i = 0; i< dominated.size(); i++){
		if(dominator[i] > dominated[i]) return false;
		if(dominated[i] > dominator[i]) flag = false;
	}
	if(flag) return false;
	return true;
//	for(std::vector<double>::size_type i = 0; i< dominated.size(); i++){
//		if(dominator[i] > dominated[i]) return false;
//	}
//	return true;
}

/**
 * Determines and returns the median of a set of tuples for the given dimension.
 */
std::vector<double> OldDNC::median(std::vector<std::vector<double>> tuples, const int &dimension){
	// Sort the tuples asc according to the given dimension
	std::sort(tuples.begin(), tuples.end(), [dimension](const std::vector<double> &a, const std::vector<double> &b){
		  return a[dimension] < b[dimension];
		});
	// Determine appx. pivot aka median for the first dimension
	std::vector<double> pivot = tuples[tuples.size()/2];

	return pivot;
}

/**
 * Partitions a set of tuples into two partitions using the given pivot element for the given dimension.
 * Returns the two partitions in form of a pair with two elements.
 */
std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> OldDNC::partition(const std::vector<std::vector<double>> &tuples, const int &dimension, const std::vector<double> &pivot){
	// Create the two partitions
	std::vector<std::vector<double>> P_1, P_2;
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> partitions;

	for(std::vector<std::vector<double>>::size_type i = 0; i < tuples.size(); i++){
		if(tuples[i][dimension] < pivot[dimension])
			P_1.push_back(tuples[i]);
		else
			P_2.push_back(tuples[i]);
	}

	partitions.first = P_1;
	partitions.second = P_2;

	return partitions;
}

/**
 * Merges two Skylines into one. Returns the new Skyline.
 */
std::vector<std::vector<double>> OldDNC::mergeBasic(std::vector<std::vector<double>> S_1, const std::vector<std::vector<double>> &S_2, const int &dimension){
	std::vector<std::vector<double>> result;

	if(S_2.size() == 0) return result;

	if(S_1.size() == 1){ // trivial case - S_1 has only 1 tuple
		for(std::vector<std::vector<double>>::size_type i = 0; i < S_2.size(); i++){
			if(!dominates(S_1[0], S_2[i]))
				result.push_back(S_2[i]);
		}
	}
	else if(S_2.size() == 1){ // trivial case - S_2 has only 1 tuple
		result.push_back(S_2[0]);
		for(std::vector<std::vector<double>>::size_type i = 0; i < S_1.size(); i++){
			if(dominates(S_1[i], S_2[0])){
				result.erase(result.begin());
				break;
			}
		}
	}
	else if(S_1[0].size() == 2){ // low dimension
		// Min from S_1 according to dimension 1
		std::sort(S_1.begin(), S_1.end(), [](const std::vector<double> &a, const std::vector<double> &b){
			return a[0] < b[0];
		});
		std::vector<double> min = S_1[0];
		// Compare S_2 to Min according to dimension 1; in dimension 2 S_1 is always better
		for(std::vector<std::vector<double>>::size_type i = 0; i < S_2.size(); i++){
			if(S_2[i][0] < min[0]) result.push_back(S_2[i]);
		}
	}
	else{ // general case
		std::vector<double> pivot = median(S_1, dimension-1-1);
		std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> partitions_dim_1 = partition(S_1, dimension-1-1, pivot);
		std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> partitions_dim_2 = partition(S_2, dimension-1-1, pivot);
		std::vector<std::vector<double>> result_1, result_2, result_3;
		result_1 = mergeBasic(partitions_dim_1.first, partitions_dim_2.first, dimension); // compare adjacent parts
		result_2 = mergeBasic(partitions_dim_1.second, partitions_dim_2.second, dimension);
		result_3 = mergeBasic(partitions_dim_1.first, result_2, dimension-1); // compare diagonally
		// Union result_1 and result_3
		for(std::vector<std::vector<double>>::size_type i = 0; i < result_1.size(); i++){
			result.push_back(result_1[i]);
		}
		for(std::vector<std::vector<double>>::size_type i = 0; i < result_3.size(); i++){
			result.push_back(result_3[i]);
		}
	}

	return result;
}

/**
 * Computes the skyline of the given set of tuples using the Basic Divide and Conquer algorithm
 */
std::vector<std::vector<double>> OldDNC::computeSkyline(const std::vector<std::vector<double>> &M, const int &dimension){
	if(M.size() == 1) return M;

	std::vector<double> pivot = median(M, dimension-1); // dimension-1 because we need the last index
	std::pair<std::vector<std::vector<double>>, std::vector<std::vector<double>>> P = partition(M, dimension-1, pivot); // dimension-1 because we need the last index

	std::vector<std::vector<double>> S_1, S_2;
	S_1 = computeSkyline(P.first, dimension);
	S_2 = computeSkyline(P.second, dimension);

	std::vector<std::vector<double>> result;
	std::vector<std::vector<double>> merge_result = mergeBasic(S_1, S_2, dimension);

	// Union S_1 and merge_result
	for(std::vector<std::vector<double>>::size_type i = 0; i < S_1.size(); i++){
		result.push_back(S_1[i]);
	}
	for(std::vector<std::vector<double>>::size_type i = 0; i < merge_result.size(); i++){
		result.push_back(merge_result[i]);
	}

	return result;
}
