#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <sstream>

#include <algorithm>
#include <map>
#include <vector>
#include <utility>

using namespace std;

typedef pair<int,map<string, int> > attributeInnerMap;

double log2( double number ) {
	return log( number ) / log( 2 ) ;
}

double entropy(attributeInnerMap Set){
	double sum = 0.0;
	for(std::map<string, int>::iterator ite = Set.second.begin();ite != Set.second.end(); ite++){
		sum -= (double) ite->second / Set.first * log2((double) ite->second / Set.first);
	}
	return sum;
}