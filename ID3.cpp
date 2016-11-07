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

struct node
{
	
	struct node* parent;
	std::vector<struct node*> childrens;
	string name;
	int type;
	bool answer;

};

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

double infoGain(attributeInnerMap Set,vector<attributeInnerMap> SubSetV){
	double acum=entropy(Set);
	for(int i = 0; i < SubSetV.size();i++){
		acum -= ((double)SubSetV[i].first / Set.first) * entropy(SubSetV[i]);
	}
	return acum;

}

class TreeID3{

private:
	struct node *root;
	vector<string> samplesT;
	vector<string> index;
	map<string, vector<string> > dictionary;
	char action;
	vector<string> actionValues;

public:

	TreeID3(vector<string> samples, vector<string> indexAttribute, map<string,vector<string> > dictionaryMap): samplesT(samples), index(indexAttribute), dictionary(dictionaryMap) {
		root = new node;
		root->parent=NULL;
		for(int i = 0; i<samplesT.size();i++){
			for(int j=0; j<samplesT[i].length();j++){
				if(samplesT[i][j] == ','){
					samplesT[i][j]=' ';
				}
			}
		}
	}





};

int main(int argc, char *argv[]){
    string buffer, recov, eater,aux;
    char devourer;
    char rmChars[]="{}";

    std::map<string, attributeInnerMap> completeMap;

    std::map<string, vector<string> > dictionaryMap;

    std::vector<string> indexAttribute;

    std::vector<string> samplesTrainings;

    std::vector<string> auxVectorString;
    
    while(cin >> devourer){
        if(devourer=='%'){
            getline(cin,eater);
        }else{
            break;
        }
    }
    
    
    getline(cin,eater); //eats @relation line
    getline(cin,eater); //eats blank line
    
    getline(cin,buffer);
	while(buffer.compare("")!=0){
		stringstream extract(buffer);
	    extract >> recov;
	   	//cout << recov << '\n';
	    if(recov == "@attribute"){
	        extract >> recov;
	        aux = recov;
	        indexAttribute.push_back(aux);
	        std::cout << "Attribute: " << aux << std::endl;
	        buffer = "";
	        while(extract >> recov){
	            buffer += recov;
	        }
	        for (int i = 0; i < strlen(rmChars); i++){
    	      buffer.erase (std::remove(buffer.begin(), buffer.end(), rmChars[i]), buffer.end());
	        }
	        
	        for(int i = 0; i < buffer.length(); i++){
	            if(buffer[i]==','){
	                buffer[i]=' ';
	            }
	        }
	        
	        cout << buffer << std::endl;
	        stringstream extract(buffer);

	        while(extract >> recov){
	            auxVectorString.push_back(recov);
	            cout << "Pushing '" << recov << "' in the vector\n";
	        }

	        cout << "Putting the Attribute " << aux << " in dictionaryMap with all its values\n";

	        dictionaryMap[aux]=auxVectorString;

	        cout << "checking...\n\n";

	        for (int i = 0; i < dictionaryMap[aux].size(); i++)
	        {
	        	cout << dictionaryMap[aux][i] << ' ';
	        }

	        cout << "\n\n";

	        while(!auxVectorString.empty()){
	        	auxVectorString.pop_back();
	        }

	    }
	    
  	    
	    getline(cin,buffer);
	}
    
    getline(cin,buffer);
	while(buffer.compare("")!=0){
		stringstream extract(buffer);
	    extract >> recov;
	   	//cout << recov << '\n';
	    if(recov == "@data"){

		    while(getline(cin,buffer)){

		    	if (buffer.find('%') != std::string::npos){
		    		continue;
		    	}else{
					samplesTrainings.push_back(buffer);
					cout << buffer << std::endl;
				}
		    }

	    }
  	    
	    getline(cin,buffer);
	}

	TreeID3 tree(samplesTrainings,indexAttribute,dictionaryMap);

	std::cout << "finish...\n" << std::endl;

	// cout << "test of the entropy and info gain\n";
	// cout << "theorical Entropy(S) = - (9/14) Log2 (9/14) - (5/14) Log2 (5/14) = 0.940\n";
	// attributeInnerMap test1;
	// test1.first = 14;
	// test1.second["yes"]=9;
	// test1.second["no"]=5;
	// double res = entropy(test1);
	// cout << "H(S) = " << res << std::endl;
	// cout << "theorical Gain(S) = Entropy(S)-(8/14)*Entropy(Sweak)-(6/14)*Entropy(Sstrong) = 0.048\n";
	// attributeInnerMap test2, test3;
	// test2.first = 8;
	// test2.second["yes"]=6;
	// test2.second["no"]=2;
	// test3.first = 6;
	// test3.second["yes"]=3;
	// test3.second["no"]=3;
	// vector<attributeInnerMap> testSubSet;
	// testSubSet.push_back(test2);
	// testSubSet.push_back(test3);
	// res = infoGain(test1,testSubSet);
	// cout << "H(S) = " << res << std::endl;

}