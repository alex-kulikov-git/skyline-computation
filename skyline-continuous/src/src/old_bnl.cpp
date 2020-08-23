/*
 * old_bnl.cpp
 * Created on: June 27, 2018
 * Author: Oleksii Kulikov
 */

#include "old_bnl.hpp"

/**
 * Checks whether one tuple dominates the other and returns true|false
 * @param dominator the tuple to check for dominating
 * @param dominated the tuple to check for being dominated
 */
bool OldBNL::dominates(const std::vector<double> &dominator, const std::vector<double> &dominated){
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
 * Computes the Skyline of the given tuples using the Block-Nested-Loops algorithm
 *
 * WE ONLY HAVE 1 ITERATION BECAUSE EVERYTHING FITS INTO 1 WINDOW (main memory database)
 */
std::vector<std::vector<double>> OldBNL::computeSkyline(const std::vector<std::vector<double>> &tuples, const std::size_t &n){
	std::vector<std::vector<double>> window;

	window.push_back(tuples[0]); // put first element into the window right away
	for(std::size_t i = 1; i < n; i++){
		window.push_back(tuples[i]);
		for(std::size_t j = 0; j < window.size()-1; j++){ // check if new tuple dominates or is dominated
			if(dominates(window.back(), window[j])){
				window.erase(window.begin() + j); // erase dominated tuple
				j--;
			}
			else if (dominates(window[j], window.back())){
				window.erase(window.begin() + window.size()-1); //erase new tuple
				break; // the new tuple was erased, so no need comparing it any longer
			}
		}
	}
	return window;
}
