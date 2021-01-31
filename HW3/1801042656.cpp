#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Rel{  //My relation class called "Rel".
	private:
		string reason;
		vector<string>            elements = { }; 	//Creating a empty vector for holding elements.
		vector< vector<string> > relations = { }; 	//Creating a empty vector for holding relations.
		vector<int>     how_many_relations = { };  	//Creating a empty vector for holding relation numbers.
		vector< vector<string> >	 hasse = { };	//Creating a empty vector for holding hasse diagram (lefted ones to print) elements.
	public:
		void read_the_file(string myfile);		//Reads the input.txt file.
		void write_to_file(string myfile);		//Writes to the output_hw2.txt file.
		void write_to_file2(string myfile);		//Writes to the output.txt file.
		int  relation_number(string inside);	//Finds how many relations are wrote in input.txt file for that set.
		bool reflexive(int i);					//Returns if i'th set is reflexive relation or not.
		bool symmetric(int i);					//Returns if i'th set is symmetric relation or not.
		bool antisymmetric(int i);				//Returns if i'th set is antisymmetric relation or not.
		bool transitive(int i);					//Returns if i'th set is transitive relation or not.
		void setReason(string reason){ this->reason = reason; }	//Sets reason which will write to the output.txt file.
		void makePoset(int i);					//HW3 PART 		//Check if it is a poset and if is returns true.
		void add_to_hasse(int i);					//Adds the poset elements that are not reflexive, transitive.
};

int main(){
	Rel relation;
	relation.read_the_file("input.txt");			//Reading input  file.
	//relation.write_to_file("output_before.txt");	//Writing old answer to file.
	relation.write_to_file2("output.txt");			//Writing answer to file.
	return 0;
}

void Rel::read_the_file(string myfile){
	string inside;
	
	ifstream file(myfile);			//Reading the file. (input.txt)
	if(file.is_open() == false){	//If input.txt couldn't find, we should prevent infinite loop error.
		cerr << "Couldn't open file.\n";
		return;
	}

	for(int i=0; !file.eof(); i++){ 				//Read until end of file.
		relations.push_back( vector<string>() );	//Allocate memory for first index.

		file >> inside;								//Reading first line. (number of how many relations)
		how_many_relations.push_back( relation_number(inside) );
		if(how_many_relations[i]==-1)	break;		//Garbage value for ex: newline

		file >> inside;					//Reading second line(Elements)
		elements.push_back(inside);		//Adding this string that contains element to elements vector.
		
		for(int j=0; j<how_many_relations[i]; j++){		//Until that relation number of that set.
			file >> inside;								//Reading all relations for ex: (3,1) (4,3) etc.
			relations[i].push_back(inside);				//Adding these relations to relations vector.
		}
	}
	cout << "File readed.\n";
	file.close();		//Reading is done. Closing the file.
}
/*
void Rel::write_to_file(string myfile){	//HW2
	ofstream file(myfile);			//Opening the myfile (output_before.txt)
	if(file.is_open() == false){	//If file couldn't find, we should prevent infinite loop error.
		cerr << "Couldn't open file.\n";
		return;
	}

	for(int i=0; i<relations.size(); i++){		//For all sets, write to output.txt file.
		file << "n\n";
		for(int j=0; j<how_many_relations[i]; j++)
			file << "(" << relations[i][j] << ")";
		file << endl;

		file << "Reflexive: ";
		if(reflexive(i)==true)	file << "Yes, all elements are present.\n";
		else 					file << "No " + reason;

		file << "Symmetric: ";
		if(symmetric(i)==true)		file << "Yes, all relations are symmetric.\n"; // because every (x,y) value has (y,x) too.
		else 						file << "No, " + reason;

		file << "Antisymmetric: ";
		if(antisymmetric(i)==true)	file << "Yes, "+ reason;
		else 						file << "No, " + reason;

		file << "Transitive: ";
		if(transitive(i)==true)		file << "Yes, "+ reason;
		else 						file << "No, " + reason;

		if(how_many_relations[i+1] == -1)  break;	//if input file's last line is newline, prevent error.
	}
	cout << "Old answers wrote to the file.\n";
	file.close();
}

*///HW3	
void Rel::write_to_file2(string myfile){
	ofstream file(myfile);			//Opening the myfile (output.txt)
	if(file.is_open() == false){	//If output.txt couldn't find, we should prevent infinite loop error.
		cerr << "Couldn't open file.\n";
		return;
	}

	for(int i=0; i<relations.size(); i++){		//For all sets, write to output.txt file.
		file << "n\n";
		makePoset(i);		//If it is not an poset it makes the poset(with adding or removing elements)
		file << "POSET: ";

		for(int j=0; j<relations[i].size(); j++)
			file << "(" << relations[i][j] << ")";
		file << endl;

		for(int j=0; j<hasse[i].size(); j++)	//Writing the lefted ones for poset.
			file << hasse[i][j] << endl;

		if(how_many_relations[i+1] == -1)  break;	//if input file's last line is newline, prevent error.
	}
	cout << "Answers wrote to the file.\n";
	file.close();
}

//HW3
void Rel::makePoset(int i){
	//Continue until make it a poset
	for(; reflexive(i) == false || antisymmetric(i) == false || transitive(i) == false ;);
	add_to_hasse(i);	//Adds to hasse diagram the leftovers.
}

int Rel::relation_number(string inside){
	int value=-1;
	if(inside.length() == 1){		//if string length is 1, that means 1 digit integer.
		if( (int)inside[0] == 10)	//10 is ascii of '\n'
			value = -1;				//That means there is no integer number ( maybe that line contains newline character).
		else
			value = int (inside[0]) - 48;
	}
	else if(inside.length() == 2)	//That means string contains 2 digits integer.
		value = (int (inside[0]) - 48)*10 + int (inside[1])-48;

	return value;		/***Return how many relations are wrote in input.txt file for that set.***/
}

bool Rel::reflexive(int i){		//Returns if i'th set is reflexive relation or not.
	int counter, flag;			//If flag==1 reflexive, if 0 not reflexive.
	string temp, tempReflex;	//temp will hold the element's index. | tempReflex is the value will be check if reflexive.

	counter=0;					//Counting if reached to all elements.
	for(int j=0; j<relations[i].size(); j++)
		if(relations[i][j][0] == relations[i][j][2])
			counter++;

	if( counter==(elements[i].length()+1)/2 ) //  dividing to 2 because I don't want to count ',' characters in elements.
		return true;  
	else{
		for(int k=0; k<elements[i].size(); k+=2){
			flag=0;
			temp = elements[i][k];
			tempReflex = "(" +temp+ "," +temp+ ")";		//it will be checked if reflexive or not.
			for(int j=0; j<relations[i].size(); j++)
				if(tempReflex.compare(1,3, relations[i][j] ,0, 3) == 0) //Comparing tempReflex's relation with all other ones.
					flag=1;			//It is matched so yes it is reflexive.

			if(flag == 0){
				setReason(tempReflex + " is not found.\n");
				tempReflex = temp + "," + temp;
				relations[i].push_back(tempReflex);	//If it not reflexive make it reflexive.
				how_many_relations[i]++;
			}
			
		}
		return false; 
	}

	return false;
}

bool Rel::symmetric(int i){		//Returns if i'th set is symmetric relation or not.
	string tempSym, tempSym2;	//tempSym is for holding (a,b) value and tempSym2 for holding (b,a) value.
	string a, b;				//For ex: relations[i][j][0] is a char and to add the reason, I need to make it string with this.
	int flag;					//Flag=1 means symmetric, flag=0 means not symmetric.

	for(int j=0; j<relations[i].size(); j++){
		flag=0;
		if(relations[i][j][0] != relations[i][j][2]){
			a = relations[i][j][2],	b = relations[i][j][0];
			tempSym = "(" +a+ "," +b+ ")";
			for(int k=0; k<relations[i].size(); k++){
				if(tempSym.compare(1,3, relations[i][k] ,0, 3)  == 0){	//Comparing (a,b) value if there is a match in relations.
					flag=1;
					break;
				}
			}
			if(flag==0){
				tempSym2 = "(" +b+ "," +a+ ")";				//(b,a) couldn't find.
				setReason(tempSym2 + " is found whereas " + tempSym + " is not found.\n");
				return false;
			}	
		}
	}

	return true;
}

bool Rel::antisymmetric(int i){		//Returns if i'th set is antisymmetric relation or not.
	string tempSym, tempSym2;		//tempSym is for holding (a,b) value and tempSym2 for holding (b,a) value.
	string a, b;					//For ex: relations[i][j][0] is a char and to add the reason, I need to make it string with this.
	int flag;						//flag=0 means antisymmetric, flag=1 && flag=2 means antisymmetric.

	flag=2;							//If until the end it remains 2 that means all are like (x,x).
	for(int j=0; j<relations[i].size(); j++){
		if(relations[i][j][0] != relations[i][j][2]){	 //means a is not b.
			flag=0;
			a = relations[i][j][2],	b = relations[i][j][0];
			tempSym = "(" + a + "," + b + ")";
			for(int k=0; k<relations[i].size(); k++)		
				if(tempSym.compare(1,3, relations[i][k] ,0, 3) == 0)	//Comparing (a,b) value if there is a match in relations.
					flag=1;												//Match found so not antisymmetric.
			if(flag==1)	break;
		}
	}

	if(flag==0){					//That means antisymmetric because (a,b) not found but (b,a) found. 
		tempSym2 = "(" + b + "," + a + ")";
		setReason(tempSym2 + " is found whereas " + tempSym + " is not found.\n");
		return true;
	}
	else if(flag==1){				//That means not antisymmetric because (a,b) and (b,a) found.
		tempSym2 = "(" + b + "," + a + ")";
		setReason(tempSym2 + " and " + tempSym + " are found.\n");
		for(vector<string>::iterator l=relations[i].begin(); l != relations[i].end(); l++){ //HW3
			if(tempSym.compare(1,3, *l ,0, 3) == 0){
				relations[i].erase(l);	//Erasing symmetric one from the poset
				how_many_relations[i]--;
			}
		}
		return false;
	}
	else	//flag=2				//That means antisymmetric because (a,b) and (b,a) found and a=b.
		setReason("because in all (x,y) values x=y.\n");
	
	return true;
}

bool Rel::transitive(int i){			//Returns if i'th set is transitive relation or not.
	string temp; 						//It will hold the (a,c) value 
	string a,b,c;						// If (a,b) and (b,c) are in set, and (a,c) is that means transitive.
	int flag;							//flag=0 means not transitive, flag=1 means transitive.

	for(int j=0; j<relations[i].size(); j++)	//To find transitive poset elements and delete it from hasse diagram.
		for(int k=0; k<relations[i].size(); k++)
			if( (relations[i][k][0] != relations[i][k][2] && relations[i][j][0] != relations[i][j][2] )  // (a,b) and (b,c) ~checking if it isn't same like (a,a)
				&& relations[i][k][0] == relations[i][j][2] && j != k)  // (a,b) and (b,c) ~checking if b's are same.
			{
				temp = string(1,relations[i][j][0]) + "," + string(1,relations[i][k][2]);  // a,c
				flag=0;
				for(int l=0; l<relations[i].size(); l++)
					if(temp.compare(0,3, relations[i][l] ,0, 3) == 0)  //Checking if (a,c) will found in relations
						flag = 1;
				if(flag == 0){	//HW3. (If not found add to poset.)
					relations[i].push_back(temp);
					how_many_relations[i]++;
					return false;	//Return false for checking transitive later.
				}
			}

	return true;

}

void Rel::add_to_hasse(int i){
	hasse.push_back( vector<string>() );	//Allocate memory for first index of leftover (hasses)	//i times
	string temp;
	for(int j=0; j<relations[i].size(); j++)	//To find non-reflexive poset elements.
		if(relations[i][j][0] != relations[i][j][2])
			hasse[i].push_back(relations[i][j]);	//If not reflexive add to hasse.

	for(int j=0; j<relations[i].size(); j++){	//To find transitive poset elements and delete it from hasse diagram.
		for(int k=0; k<relations[i].size(); k++){
			if( (relations[i][k][0] != relations[i][k][2] && relations[i][j][0] != relations[i][j][2] )  // (a,b) and (b,c) ~checking if it isn't same like (a,a)
				&& relations[i][k][0] == relations[i][j][2])  // (a,b) and (b,c) ~checking if b's are same.
			{
				temp = string(1,relations[i][j][0]) + "," + string(1,relations[i][k][2]); 
				for(int l=0; l<hasse[i].size(); l++)
					if(hasse[i][l] == temp)
						hasse[i].erase( hasse[i].begin() + l);
			}
		}
	}
}