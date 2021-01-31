#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class Rel{  //My relation class called "Rel".
	private:
		vector<string>         elements  ={ }; 	//Creating a empty vector for holding elements.
		vector<vector<string>> relations ={ }; 	//Creating a empty vector for holding relations.
		vector<int>   how_many_relations ={ };  //Creating a empty vector for holding relation numbers.
		string reason;
	public:
		void read_the_file(string myfile);		//Reads the input.txt file.
		void write_to_file(string myfile);		//Writes to the output.txt file.
		int  relation_number(string inside);	//Finds how many relations are wrote in input.txt file for that set.
		bool reflexive(int i);					//Returns if i'th set is reflexive relation or not.
		bool symmetric(int i);					//Returns if i'th set is symmetric relation or not.
		bool antisymmetric(int i);				//Returns if i'th set is antisymmetric relation or not.
		bool transitive(int i);					//Returns if i'th set is transitive relation or not.
		void setReason(string reason){ this->reason = reason; }	//Sets reason which will write to the output.txt file.
};

int main(){
	Rel relation;
	relation.read_the_file( "input.txt");	//Reading input  file.
	relation.write_to_file("output.txt");	//Writing answer to file.
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

void Rel::write_to_file(string myfile){
	ofstream file(myfile);			//Opening the myfile (output.txt)
	if(file.is_open() == false){	//If output.txt couldn't find, we should prevent infinite loop error.
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
	cout << "Answers wrote to the file.\n";
	file.close();
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

			if(flag == 0)
				setReason(tempReflex + " is not found.\n");
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
		return false;
	}
	else	//flag=2				//That means antisymmetric because (a,b) and (b,a) found and a=b.
		setReason("because in all (x,y) values x=y.\n");
	
	return true;
}

bool Rel::transitive(int i){			//Returns if i'th set is transitive relation or not.
	string tempTr, tempTr2, tempTr3;	//They will hold the relations. For ex tempTr will hold (a,b)
	string a,b,c;						// If (a,b) and (b,c) are in set, and (a,c) is that means transitive.
	int flag;							//flag=0 means not transitive, flag=1 means transitive.

	flag=0;
	tempTr = "EMPTY";					//If still remains empty that means transitive because all values are like (x,x)
	for(int j=0; j<relations[i].size(); j++){
		if(relations[i][j][0] != relations[i][j][2]){	// relations[i][j] = (a,b)
			a = relations[i][j][0],	b = relations[i][j][2]; 	//Char to string conversion.
			for(int k=0; k<relations[i].size(); k++){
				if(j!=k && relations[i][j][2] == relations[i][k][0] ){  // b value.
					if(relations[i][j][0] == relations[i][k][0]) continue; //a==b shouldn't be
					if(relations[i][k][0] == relations[i][k][2]) continue; //b==c shouldn't be
					c = relations[i][k][2];		// c value char to string . (If I don't do that I cannot merge strings like below.)
					tempTr  = "(" + a + "," + b + ")";  // tempTr  will hold the (a,b) value
					tempTr2 = "(" + b + "," + c + ")";	// tempTr2 will hold the (b,c) value	
					tempTr3 = "(" + a + "," + c + ")";	// tempTr3 will hold the (a,c) value
					flag=0;
					for(int m=0; m<relations[i].size(); m++){
						if(tempTr3.compare(1,3, relations[i][m] ,0, 3) == 0)  //Checking if (a,c) will found in relations
							flag=1;					//(a,c) is found.
					}
					if(flag==0)	break;			//That means not transitive.
				}
			}
		}
	}
	if(tempTr.compare(0,5, "EMPTY",0, 5) == 0){		//Empty means all relations are like (x,x) so transitive.
		setReason("because all values are like (x,x).\n");
		return true;
	}
	if(flag==0){			//Not transitive		
		setReason(tempTr3 + " is not found whereas "+ tempTr + " and " + tempTr2 + " is found.\n");
		cout << tempTr3 << endl;
		return false;
	}	
	else{	//if flag==1	//If transitive
		setReason(tempTr3 + " is found since " + tempTr+" and "+tempTr2+" are found.\n");
		return true;
	}

	return false;
}