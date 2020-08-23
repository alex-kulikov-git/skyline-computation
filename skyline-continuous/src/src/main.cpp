/*
 * main.cpp
 * Created on: July 5, 2018
 * Author: Oleksii Kulikov
 */

#include <iostream>
#include <chrono>

#include "bnl.hpp"
#include "output.hpp"
#include "generator.hpp"
#include "old_bnl.hpp"
#include "old_dnc.hpp"
#include "dnc.hpp"
#include "nnl.hpp"
#include "parallel_dnc.hpp"

static std::size_t n = 0, dimension = 0;

void init(){
	std::cout << "Number of tuples: \nn = ";
	std::cin >> n;
	std::cout << "Number of dimensions: \ndimensions = ";
	std::cin >> dimension;
}

// @param cp gives the variation of the algorithm ( 0: getNext | 1: consume/produce | 2: parallel consume/produce )
double test_algorithm(Output &output, int cp){
	std::chrono::_V2::system_clock::time_point start = std::chrono::high_resolution_clock::now();
	switch(cp){
	case 0:
		output.getTuples();
		break;
	case 1:
		output.getTuplesCP();
		break;
	case 2:
		output.getTuplesParallelCP();
		break;
	default:
		break;
	}
	std::chrono::_V2::system_clock::time_point finish = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = finish - start;
	output.print();
	std::cout << "Elapsed time: " << elapsed.count() << "s\n\n";
	return elapsed.count();
}

bool check_results(const std::vector<std::vector<std::vector<double>>> &results){
	static bool output = true;
	// check if sizes are equal
	for(auto r:results){
		if(results[0].size() != r.size()){
			std::cout << "Result sizes do not match.\n";
			output = false;
		}
	}
	// check if results match
	for(std::size_t i = 1; i < results.size(); i++){
		for(std::size_t j = 0; j < results[0].size(); j++){
			if(std::find(results[i].begin(), results[i].end(), results[0][j]) == results[i].end()) {
				std::cout << "Tuple[" << j << "] in result 0 does not exist in result " << i << ".\n";
				output = false;
			}
		}
	}
	return output;
}


int main()
{
	init();

	BNL *bnl_pointer;
	Output *output_pointer;

	// BNL iterator model
	Generator g(bnl_pointer, n, dimension);
	BNL bnl_im(output_pointer, &g);
	Output o1(&bnl_im);
	g.set_parent(&bnl_im);
	bnl_im.set_parent(&o1);
	std::cout << "Computing Skyline with iterator model BNL...\n";
	double elapsed_1 = test_algorithm(o1, 0);

	// BNL produce/consume
	BNL bnl_pc(output_pointer, &g);
	Output o2(&bnl_pc);
	g.set_parent(&bnl_pc);
	bnl_pc.set_parent(&o2);
	std::cout << "Computing Skyline with produce/consume BNL...\n";
	double elapsed_2 = test_algorithm(o2, 1);

	// NNL produce/consume
	NNL nnl_pc(output_pointer, &g);
	Output o3(&nnl_pc);
	g.set_parent(&nnl_pc);
	nnl_pc.set_parent(&o3);
	std::cout << "Computing Skyline with produce/consume NNL...\n";
	double elapsed_3 = test_algorithm(o3, 1);

	// NNL produce/consume parallelized
	NNL nnl_parallel_pc(output_pointer, &g);
	Output o4(&nnl_parallel_pc);
	g.set_parent(&nnl_parallel_pc);
	nnl_parallel_pc.set_parent(&o4);
	std::cout << "Computing Skyline with produce/consume parallelized NNL...\n";
	double elapsed_4 = test_algorithm(o4, 2);

	// DNC produce/consume
	DNC dnc(output_pointer, &g);
	Output o5(&dnc);
	g.set_parent(&dnc);
	dnc.set_parent(&o5);
	std::cout << "Computing Skyline with produce/consume DNC...\n";
	double elapsed_5 = test_algorithm(o5, 1);

	// DNC produce/consume parallelized
	Parallel_DNC parallel_dnc(output_pointer, &g);
	Output o6(&parallel_dnc);
	g.set_parent(&parallel_dnc);
	parallel_dnc.set_parent(&o6);
	std::cout << "Computing Skyline with parallel produce/consume DNC...\n";
	double elapsed_6 = test_algorithm(o6, 1);


	// OLD VERSIONS FOR TESTING

	// Old version of BNL
//	OldBNL old_bnl;
//	auto start = std::chrono::high_resolution_clock::now();
//	std::cout << "Computing Skyline with normal BNL...\n";
//	std::vector<std::vector<double>> result_bnl = old_bnl.computeSkyline(g.getTuples(), n);
//	auto finish = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<double> elapsed_7 = finish - start;
//	std::cout << "Resulting Skyline: \n";
//	for(std::vector<std::vector<double>>::size_type i = 0; i < result_bnl.size(); i++){
//		std::cout << "Tuple[" << i << "] is (";
//		for(std::vector<double>::size_type j = 0; j < result_bnl[i].size(); j++){
//			std::cout << result_bnl[i][j] << ' ';
//		}
//		std::cout << ")\n";
//	}
//	std::cout << "\nElapsed time: " << elapsed_7.count() << "s\n\n";

	// Old version of DNC
//	OldDNC old_dnc;
//	start = std::chrono::high_resolution_clock::now();
//	std::cout << "Computing Skyline with normal DNC...\n";
//	std::vector<std::vector<double>> result_old_dnc = old_dnc.computeSkyline(g.getTuples(), dimension);
//	finish = std::chrono::high_resolution_clock::now();
//	std::chrono::duration<double> elapsed_8 = finish - start;
//	std::cout << "Resulting Skyline: \n";
//	for(std::vector<std::vector<double>>::size_type i = 0; i < result_old_dnc.size(); i++){
//		std::cout << "Tuple[" << i << "] is (";
//		for(std::vector<double>::size_type j = 0; j < result_old_dnc[i].size(); j++){
//			std::cout << result_old_dnc[i][j] << ' ';
//		}
//		std::cout << ")\n";
//	}
//	std::cout << "\nElapsed time: " << elapsed_8.count() << "s\n\n";


	// Total
	std::cout << "Input was: n = " << n << " dimension = " << dimension << "\n\n";
	std::cout << "Results are: ";
	std::vector<std::vector<std::vector<double>>> to_check;
	to_check.push_back(o1.getStorage());
	to_check.push_back(o2.getStorage());
	to_check.push_back(o3.getStorage());
	to_check.push_back(o4.getStorage());
	to_check.push_back(o5.getStorage());
	to_check.push_back(o6.getStorage());
//	to_check.push_back(result_bnl);
//	to_check.push_back(result_old_dnc);
	if(check_results(to_check))
		std::cout << "OK\n";
	else std::cout << "NOT OK\n";
	std::cout << "\nExecution time\n";
	std::cout << "Iterator Model BNL: " << elapsed_1 << "s\n";
	std::cout << "Produce/Consume BNL " << elapsed_2 << "s\n";
	std::cout << "Produce/Consume NNL: " << elapsed_3 << "s\n";
	std::cout << "Produce/Consume NNL Parallelized: " << elapsed_4 << "s\n";
	std::cout << "Produce/Consume DNC: " << elapsed_5 << "s\n";
	std::cout << "Produce/Consume DNC Parallelized: " << elapsed_6 << "s\n";
//	std::cout << "Normal BNL: " << elapsed_7.count() << "s\n";
//	std::cout << "Normal DNC: " << elapsed_8.count() << "s\n";

	return 0;
}
