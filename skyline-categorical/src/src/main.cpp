/**
 * main.cpp
 * Author: Oleksii Kulikov
 * Created: August 2018
*/

#include <iostream>
#include <chrono>

#include "art.hpp"
#include "bnl.hpp"
#include "ntree.hpp"
#include "cat_generator.hpp"
#include "cat_output.hpp"
#include "nnl.hpp"
#include "sts.hpp"
#include "s_art_s.hpp"
#include "parallel_sarts.hpp"
#include "parallel_sts.hpp"

static std::size_t n = 0, dimension = 0;

void init(){
	std::cout << "Number of tuples: \nn = ";
	std::cin >> n;
	std::cout << "Number of dimensions: \ndimensions = ";
	std::cin >> dimension;
}

double test_algorithm(CatOutput &output, int cp){
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

bool check_results(const std::vector<std::vector<std::vector<int>>> &results){
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

int main(int argc,char** argv){
	init();

	STS *sts_pointer;
	CatOutput *output_pointer;
	CatGenerator g(sts_pointer, n, dimension);
	std::vector<int> attributes = g.getAttributes();

	NTree ntree(attributes);
	STS sts(output_pointer, &g, ntree);
	CatOutput o(&sts);
	g.set_parent(&sts);
	sts.set_parent(&o);
//	g.print();
	std::cout << "Computing Skyline with STS...\n";
	double elapsed_1 = test_algorithm(o,1);
//	ntree.print();

	NTree ntree2(attributes);
	ParallelSTS parallel_sts(output_pointer, &g, ntree2);
	CatOutput o2(&parallel_sts);
	g.set_parent(&parallel_sts);
	parallel_sts.set_parent(&o2);
	std::cout << "Computing Skyline with Parallel STS...\n";
	double elapsed_2 = test_algorithm(o2,1);
//	ntree2.print();

	ART art(attributes);
	SARTS sarts(output_pointer, &g, art);
	CatOutput o3(&sarts);
	g.set_parent(&sarts);
	sarts.set_parent(&o3);
	std::cout << "Computing Skyline with SARTS...\n";
	double elapsed_3 = test_algorithm(o3,1);
//	art.print();

	ART art2(attributes);
	ParallelSARTS parallel_sarts(output_pointer, &g, art2);
	CatOutput o4(&parallel_sarts);
	g.set_parent(&parallel_sarts);
	parallel_sarts.set_parent(&o4);
	std::cout << "Computing Skyline with Parallel SARTS...\n";
	double elapsed_4 = test_algorithm(o4,1);
//	art2.print();

	// BNL iterator model
	BNL bnl_im(output_pointer, &g);
	CatOutput o5(&bnl_im);
	g.set_parent(&bnl_im);
	bnl_im.set_parent(&o5);
	std::cout << "Computing Skyline with BNL it mod ...\n";
	double elapsed_5 = test_algorithm(o5,0);

	// BNL produce/cconsume
	BNL bnl_pc(output_pointer, &g);
	CatOutput o6(&bnl_pc);
	g.set_parent(&bnl_pc);
	bnl_pc.set_parent(&o6);
	std::cout << "Computing Skyline with new BNL p/c ...\n";
	double elapsed_6 = test_algorithm(o6,1);

	// NNL produce/consume
	NNL nnl_pc(output_pointer, &g);
	CatOutput o7(&nnl_pc);
	g.set_parent(&nnl_pc);
	nnl_pc.set_parent(&o7);
	std::cout << "Computing Skyline with NNL p/c ...\n";
	double elapsed_7 = test_algorithm(o7,1);

	// NNL parallel produce/consume
	NNL parallel_nnl_pc(output_pointer, &g);
	CatOutput o8(&parallel_nnl_pc);
	g.set_parent(&parallel_nnl_pc);
	parallel_nnl_pc.set_parent(&o8);
	std::cout << "Computing Skyline with NNL parallel p/c ...\n";
	double elapsed_8 = test_algorithm(o8,2);


	// Results
	std::cout << "Input was: n = " << n << " dimension = " << dimension << "\n\n";
	std::cout << "Results are: ";
	std::vector<std::vector<std::vector<int>>> to_check;
	to_check.push_back(o.getStorage());
	to_check.push_back(o2.getStorage());
	to_check.push_back(o3.getStorage());
	to_check.push_back(o4.getStorage());
	to_check.push_back(o5.getStorage());
	to_check.push_back(o6.getStorage());
	to_check.push_back(o7.getStorage());
	to_check.push_back(o8.getStorage());
	if(check_results(to_check))
		std::cout << "OK\n";
	else std::cout << "NOT OK\n";
	std::cout << "\nExecution time\n";
	std::cout << "STS: " << elapsed_1 << "s\n";
	std::cout << "Parallel STS: " << elapsed_2 << "s\n";
	std::cout << "SARTS: " << elapsed_3 << "s\n";
	std::cout << "Parallel SARTS: " << elapsed_4 << "s\n";
	std::cout << "BNL it mod: " << elapsed_5 << "s\n";
	std::cout << "BNL p/c: " << elapsed_6 << "s\n";
	std::cout << "NNL p/c: " << elapsed_7 << "s\n";
	std::cout << "NNL parallel p/c: " << elapsed_8 << "s\n";

	return 0;
}





