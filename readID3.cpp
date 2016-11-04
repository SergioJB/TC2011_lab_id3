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
	    if(recov == "@data"){
	    	//getline(cin,eater); //eats blank line
        	getline(cin,eater); //eats @data

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
    
    getline(cin,buffer);
	while(buffer.compare("")!=0){
		stringstream extract(buffer);
	    extract >> recov;
	   	//cout << recov << '\n';
	    if(recov == "@data"){
	    	//getline(cin,eater); //eats blank line
        	getline(cin,eater); //eats @data

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
	std::cout << "finish...\n" << std::endl;
}