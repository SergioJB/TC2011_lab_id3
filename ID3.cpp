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
	int type; // 0:attr ; 1:value; 2:answer;
	bool answer;

};

double log2( double number ) {
	return log( number ) / log( 2 ) ;
}

double entropy(attributeInnerMap Set){
	double sum = 0.0;
	for(std::map<string, int>::iterator ite = Set.second.begin();ite != Set.second.end(); ite++){
		if(ite->second > 0){
			sum -= (double) ite->second / Set.first * log2((double) ite->second / Set.first);
		}
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

bool doNeedSplit(vector<string> index, map<string, vector<string> > dictionary, vector<vector<string> > samples){
	attributeInnerMap Set;
	int answerIndex = index.size()-1;
	Set.first=samples.size();
	for (int i = 0; i < dictionary[index[answerIndex]].size(); i++)
	{
		int count=0;
		for(int j = 0; j < samples.size(); j++){
			if(samples[j][answerIndex] == dictionary[index[answerIndex]][i]){
				count++;
			}
		}
		Set.second[dictionary[index[answerIndex]][i]] = count;
	}

	if(entropy(Set)!=0.0){
		return true;
	}else{
		return false;
	}
}

int toSplit(vector<string> index, map<string, vector<string> > dictionary, vector<vector<string> > samples){
	int instances = samples.size();
	vector<attributeInnerMap> SubSetV;
	attributeInnerMap Set, aux;
	Set.first=instances;
	int answerIndex = index.size()-1;
	double maxGain = -1.0;
	int indexInt = -1;
	for (int i = 0; i < dictionary[index[answerIndex]].size(); i++)
	{
		int count=0;
		for(int j = 0; j < samples.size(); j++){
			if(samples[j][answerIndex] == dictionary[index[answerIndex]][i]){
				count++;
			}
		}
		Set.second[dictionary[index[answerIndex]][i]] = count;
		// cout << dictionary[index[answerIndex]][i] << " " << count << endl;
	}

	// cout << "Set: " << instances << endl;
	// cout << "entropy: " << entropy(Set) << endl;


	for(int i = 0; i < index.size()-1; i++){
		for (int j = 0; j < dictionary[index[i]].size(); j++)
		{
			int inst = 0;
			for (int k = 0; k < dictionary[index[answerIndex]].size(); k++)
			{
				int count = 0;
				for (int n = 0; n < samples.size(); n++)
				{
					if(samples[n][answerIndex] == dictionary[index[answerIndex]][k] && samples[n][i] == dictionary[index[i]][j]){
						count++;
					}
				}
				aux.second[dictionary[index[answerIndex]][k]] = count;
				// cout << dictionary[index[answerIndex]][k] << " " << count << endl;
				inst+=count;
			}
			// cout << dictionary[index[i]][j] << ": " << inst <<endl;
			aux.first = inst;
			SubSetV.push_back(aux);
			aux.second.clear();
		}
		double gainN = infoGain(Set,SubSetV);
		SubSetV.clear();
		// cout << gainN << endl;
		if(gainN > maxGain){
			maxGain = gainN;
			indexInt = i;
		}
	}

	// cout << "MaxGain: " << maxGain << endl;
	// cout << index[indexInt] << endl;
	return indexInt;
}

void splitting(struct node* Node,vector<string> index, map<string, vector<string> > dictionary, vector<vector<string> > samples, string level){
	if(doNeedSplit(index,dictionary,samples)){
		int indexSplit = toSplit(index,dictionary,samples);
		vector<string> copyIndex;
		map<string, vector<string> > copyDictionary;
		vector<vector<string> > branchSamples;
		Node->name = index[indexSplit];
		Node->type = 0; //attribute
		Node->answer = false;
		for (int i = 0; i < dictionary[index[indexSplit]].size(); i++){
			struct node* child = new node;
			child->type=1; //att val
			child->parent=Node;
			child->name=dictionary[index[indexSplit]][i];
			child->answer=false;
			cout << level << Node->name << ": " << child->name << endl;
			Node->childrens.push_back(child);

			copyIndex = index;

			copyIndex.erase(copyIndex.begin()+(indexSplit));
			for(int j=0;j<samples.size();j++){
				if(samples[j][indexSplit]==dictionary[index[indexSplit]][i]){
					branchSamples.push_back(samples[j]);
				}
			}
			for(int j=0;j<branchSamples.size();j++){
				branchSamples[j].erase(branchSamples[j].begin()+indexSplit);
			}

			splitting(child,copyIndex,dictionary,branchSamples,"  ");
			copyIndex.clear();
			branchSamples.clear();
		}
	}else{
		int indexAnswer = index.size() - 1;
		struct node* child = new node;
		child->type=2; //answer val
		child->parent=Node;
		child->name=samples[0][indexAnswer];
		child->answer=true;
		cout << level << "  ANSWER: " << child->name << endl;
		Node->childrens.push_back(child);
	}
}


class TreeID3{

private:
	struct node* root;
	vector<string> samplesT;
	vector<string> index;
	map<string, vector<string> > dictionary;
	string action;
	vector<string> actionValues;
	vector<vector<string> > Samples;

public:

	TreeID3(vector<string> samples, vector<string> indexAttribute, map<string,vector<string> > dictionaryMap): samplesT(samples), index(indexAttribute), dictionary(dictionaryMap) {
		root = new node;
		root->parent=NULL;
		root->answer=false;
		root->type = 0;

		for(int i = 0; i<samplesT.size();i++){
			for(int j=0; j<samplesT[i].length();j++){
				if(samplesT[i][j] == ','){
					samplesT[i][j]=' ';
				}
			}
			stringstream extract(samplesT[i]);
			string aux;
			vector<string> auxiliar;
			while(extract >> aux){
				auxiliar.push_back(aux);
			}
			Samples.push_back(auxiliar);
			auxiliar.clear();
		}
		action = index[index.size()-1];
		for(int i = 0; i<dictionary[action].size();i++){
			actionValues.push_back(dictionary[action][i]);
		}
	}

	void createTree(){
		// toSplit(index,dictionary,Samples);;
		splitting(root,index,dictionary,Samples,"");
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
	        // std::cout << "Attribute: " << aux << std::endl;
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
	        
	        // cout << buffer << std::endl;
	        stringstream extract(buffer);

	        while(extract >> recov){
	            auxVectorString.push_back(recov);
	            // cout << "Pushing '" << recov << "' in the vector\n";
	        }

	        // cout << "Putting the Attribute " << aux << " in dictionaryMap with all its values\n";

	        dictionaryMap[aux]=auxVectorString;

	        // cout << "checking...\n\n";

	        // for (int i = 0; i < dictionaryMap[aux].size(); i++)
	        // {
	        // 	cout << dictionaryMap[aux][i] << ' ';
	        // }

	        // cout << "\n\n";

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
					//cout << buffer << std::endl;
				}
		    }

	    }
  	    
	    getline(cin,buffer);
	}

	TreeID3 tree(samplesTrainings,indexAttribute,dictionaryMap);

	tree.createTree();

	//std::cout << "finish...\n" << std::endl;

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