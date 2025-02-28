#include<bits/stdc++.h>

using namespace std;

map< string, int > symbolTable = {
	{ "R0", 0 },
	{ "R1", 1 },
	{ "R2", 2 },
	{ "R3", 3 },
	{ "R4", 4 },
	{ "R5", 5 },
	{ "R6", 6 },
	{ "R7", 7 },
	{ "R8", 8 },
	{ "R9", 9 },
	{ "R10", 10 },
	{ "R11", 11 },
	{ "R12", 12 },
	{ "R13", 13 },
	{ "R14", 14 },
	{ "R15", 15 },
	{ "SCREEN", 16384 },
	{ "KBD", 24576 },
	{ "SP", 0 },
	{ "LCL", 1 },
	{ "ARG", 2 },
	{ "THIS", 3 },
	{ "THAT", 4 }
};


void parse(ifstream &asmFile, string const &fileName){
	//vector< pair<string,int> > lineNum;
		ofstream tempFile; // .temp file 
		tempFile.open(fileName); // open the .temp file
		string line;
		int count=0;
		while( getline(asmFile, line))
		{  		
			cout << line << endl;
			int n = line.size(); // cout << "length of this line : " <<n<<endl;	
			string temp; 
			for (int i = 0; i < n ; ++i)	
			{
				if (line[i] == '/' && line[i] == line[i+1])
					break;				
				else
					if (line[i]!= ' ') // whitespace
						temp += line[i];
			}
			if ( temp.size() > 1){  //at least 2 characters
				//cout<< typeid(temp).name()<<endl;
		       		tempFile << temp << "\n";

			}
		}
		tempFile.close();
//parse done
}

void createSymbolTable(ifstream &openTempFile){
	
	cout <<"symbol Table creation. phase 1..............\n";
		string line;
		int lineNo = 0;
		bool flagLabel = false;
		while( getline(openTempFile, line))
		{  	cout << line << endl;
			flagLabel = false;
			int n = line.size();  //cout << "length of this line : " <<n<<endl;	
			string temp; 
			for (int i = 0; i < n ; ++i)	
			{
				if (line[i] == '(')
				{	i++;
					flagLabel = true; //label found
					while( line[i] != ')')
						temp += line[i++];
				}
			}

			if (! flagLabel)// if this line aint a label, increment counter
				lineNo++;		
			else
			{ 	//if (temp[0]!='(')
				//	lineNum[ 
				symbolTable.insert({temp, lineNo}); 
				cout<<"entered " <<temp <<":" <<lineNo  << " into symbol table"<<endl;			
			}
		}
		openTempFile.close();
//createSymbolTable done
}

void printSymbolTable(){
	int len = symbolTable.size();
	for (auto iter:symbolTable) //.begin(); iter<symbolTable.end(); iter++)
	{	cout<<"\""<< iter.first << "\" : " << iter.second<<"|"<<endl;
	}
}

string asm2bin(string &inp){
	string ans;
	int len = inp.size();
	cout <<"INPUT : "<< inp <<endl;
	if (inp[0] == '@') // A ins
	{	// A instructions start with 0 opcode
		ans += '0';
		string addr ; 
		for(int i=1; i<len; i++)
		{ 
				addr += inp[i]; 
		}
		int number = stoi(addr);
		cout << "the address is : " << addr << " the number is : " << number<<endl;
		string binary = std::bitset<15>(number).to_string(); //to binary
		cout<<binary<<"\n";
		ans += binary;
		
	}
	else  // C ins
	{	// C instructions start with 111 opcode
		ans += "111"; //012	//3 4  5  6  7  8  9    10 11 12   13 14 15
		ans += "0000000000000"; //a c1 c2 c3 c4 c5 c6 = d1 d2 d3 ; j1 j2 j3
		
		// dest logic
		string dest;
		int iter=0;
		for (; iter<len ; iter++){
			if ( (inp[iter] == '=') || (inp[iter] == ';'))
				break;
			else
				dest += inp[iter];
		}
		if (inp[iter] == ';')
			cout << "dest is NULL"<<endl;
		else
		{	cout << "dest is " << dest<<endl;
			if ( dest == "M") ans[12] = '1';

			if ( dest == "D") ans[11] = '1';

			if ( dest == "MD") ans[11] = ans[12] = '1';

			if ( dest == "A") ans[10] = '1';

			if ( dest == "AM") ans[10] = ans[12] = '1';

			if ( dest == "AD") ans[10] = ans[11] = '1';

			if ( dest == "AMD") ans[10] = ans[11] = ans[12] = '1';

			//default : //all are already 000
		}
		cout <<"DEST done!! ans so far : "<<ans<<endl<<" iter : inp[iter] = "<<iter<<":"<<inp[iter] <<"\n\n";
		
		// comp logic
		string comp;

		if (inp[iter] == ';')
			iter = 0;
		else
			iter +=1;		
		// for setting right index of comp

			for (; iter<len ; iter++){
				if ( (inp[iter] == ';') || (inp[iter] == 10 ) || (inp[iter] == 13) || (inp[iter] ==32)) //newline or space
					break;
				else
					comp += inp[iter];
			}
			cout << "comp is :" << comp <<", length is :" << comp.size() <<endl;
			// For reference		
			//ans += "111"; //012	  //3  4  5  6  7  8  9   10 11 12   13 14 15
			//ans += "0000000000000"; //a c1 c2 c3 c4 c5 c6 = d1 d2 d3 ; j1 j2 j3
		
			cout << "ans before comp : " << ans << endl;		
		
			if ( comp == "0") ans[4] = ans[6] = ans[8] = '1';

			if ( comp == "1") ans[4] = ans[5] = ans[6] = ans[7] = ans[8] = ans[9] = '1';

			if ( comp == "-1") ans[4] = ans[5] = ans[6] = ans[8] = '1';

			if ( comp == "D") {ans[6] = ans[7] = '1';  }

			if ( comp == "A") {ans[4] = ans[5] = '1'; cout<<"REEEEEEEEEEEEEEEEEEEEEEE"<<endl; }
			if ( comp == "M") ans[3] = ans[4] = ans[5] = '1';

			if ( comp == "!D") ans[6] = ans[7] = ans[9] = '1';		
			
			if ( comp == "!A") ans[4] = ans[5] = ans[9] = '1';
			if ( comp == "!M") ans[3] = ans[4] = ans[5] = ans[9] = '1';		

			if ( comp == "-D") ans[6] = ans[7] = ans[8] = ans[9] = '1';

			if ( comp == "-A") ans[4] = ans[5] = ans[8] = ans[9] = '1';
			if ( comp == "-M") ans[3] = ans[4] = ans[5] = ans[8] = ans[9] = '1';

			if ( comp == "D+1") ans[5] = ans[6] = ans[7] = ans[8] = ans[9] = '1';

			if ( comp == "A+1") ans[4] = ans[5] = ans[7] = ans[8] = ans[9] = '1';
			if ( comp == "M+1") ans[3] = ans[4] = ans[5] = ans[7] = ans[8] = ans[9] = '1';

			if ( comp == "D-1") ans[6] = ans[7] = ans[8] = '1';

			if ( comp == "A-1") ans[4] = ans[5] = ans[8] = '1';
			if ( comp == "M-1") ans[3] = ans[4] = ans[5] = ans[8] = '1';

			if ( comp == "D+A") ans[8] = '1';
			if ( comp == "D+M") ans[3] = ans[8] = '1';

			if ( comp == "D-A") ans[5] = ans[8] = ans[9] = '1';
			if ( comp == "D-M") ans[3] = ans[5] = ans[8] = ans[9] = '1';

			if ( comp == "A-D") ans[7] = ans[8] = ans[9] = '1';
			if ( comp == "M-D") ans[3] = ans[7] = ans[8] = ans[9] = '1';

			//if ( comp == "D&A") //ans[3] = '0'; all are 0
			if ( comp == "D&M") ans[3] = '1';

			if ( comp == "D|A") ans[5] = ans[7] = ans[9] = '1';
			if ( comp == "D|M") ans[3] = ans[5] = ans[7] = ans[9] = '1';
		cout <<"COMP done!! ans so far : "<<ans<<endl <<" iter : inp[iter] = "<<iter<<":"<<inp[iter] <<"\n\n";
		
				
		if (inp[iter] == ';')
		{		
			// jump logic
			string jump;
			for (iter++; iter<len ; iter++){
				if (inp[iter] == '\n')
					break;
				else
					jump += inp[iter];
			}
			cout << "jump is " << jump<<endl;
			if ( jump == "JGT") ans[15] = '1';

			if ( jump == "JEQ") ans[14] = '1';

			if ( jump == "JGE") ans[14] = ans[15] = '1';

			if ( jump == "JLT") ans[13] = '1';

			if ( jump == "JNE") ans[13] = ans[15] = '1';

			if ( jump == "JLE") ans[13] = ans[14] = '1';

			if ( jump == "JMP") ans[13] = ans[14] = ans[15] = '1';
		}
		cout << "FINAL ans: " << ans<< "\n#### #### #### #### #### #### #### ####\n";	
	}
	return ans;
//asm2bin done
}

void tempToHack(string const &tempFileName, string const &hackFileName){
		string sLine;
		int nxt = 16;
		ofstream binFile;
		binFile.open(hackFileName); //open the .hack file given by argv

		ifstream openTempFile;		
		openTempFile.open(tempFileName);

		cout <<"beginning Parser. phase 2..............\n";
		while( getline(openTempFile, sLine))
		{	bool aIns = false, cIns = false;
		  	cout << sLine << endl;
			int n = sLine.size(); cout << "length of this line : " <<n<<endl;	
			string temp; 
			for (int i = 0; i < n ; ++i)	
			{
				if (sLine[i] == '@')
				{	
					aIns = true;
					for(i++; (sLine[i] != 13) && (sLine[i] != 10) && (i<n) ;i++) // \n and \r
						temp += sLine[i];
				}
				else
				if (sLine[i] != '(' && sLine[i] != 32) // space 
				{	
					cIns = true;
					for(; (sLine[i] != 13) && (sLine[i] != 10) && (sLine[i]!=32) && (i<n) ;i++)
						temp += sLine[i];
				}
				else				
					if (sLine[i] == '(') break;
			}
			cout << "temp variable is : \""<<temp <<"\""<<endl;					
			if (aIns) // A-ins
			{	try
				{	int ins = stoi(temp);
					string sTmpAIns; sTmpAIns+='@'; sTmpAIns += temp;
					binFile << asm2bin(sTmpAIns) << "\n"; // a Ins start with '@'
				}
				catch(...){ // we need to add variable
				if (symbolTable.find(temp) == symbolTable.end())
				{	
					symbolTable.insert({temp, nxt++}); 
					cout << "inserted into symbol tabel : "<< symbolTable[temp]<<endl;
				}
				string sTmpAIns; sTmpAIns+='@'; sTmpAIns += to_string(symbolTable[temp]); cout<<"symbol : "<< symbolTable[temp]<<" sTemp :" <<sTmpAIns<<endl;
				binFile << asm2bin(sTmpAIns) << "\n"; // a Ins start with '@'
				}
			}	
			else	if (cIns) // C-ins
					binFile << asm2bin(temp) << "\n"; // c Ins format: "dest = comp; JMP"
		}

		binFile.close();
// tempToHack done
}

int main(int argc, char **argv)
{	
	ios_base::sync_with_stdio(false);
	cin.tie(NULL); // fastIO	

	std::cout<<"Assembler!";
	int i,n,t;
	cout << "You have entered " << argc << " arguments:" << "\n"; 
	for ( i = 0; i < argc; ++i)
		cout << argv[i] << "\n"; 
	if (argc != 4){
		cout<<" USAGE : ./assembler file_name1.asm file_name2.temp file_name3.hack "<<endl;
		return 0;
	}
	//map<string, int> lineNum;

	ifstream asmFile(argv[1]); // .asm file
	if (asmFile.is_open())
	{	parse(asmFile, string(argv[2])); // parse the asm file into .temp file
		asmFile.close();
	}
	else
		cout<< "Couldnt' open the file : "<< argv[1]<<endl;
	
	ifstream openTempFile(argv[2]); // opening .temp file

	if (openTempFile.is_open())
	{	
		createSymbolTable(openTempFile); // create the symbolTable in the first pass and update the temp file by removing (LABELS)
		openTempFile.close();
		tempToHack(string(argv[2]), string(argv[3]));  // convert the .temp file to .hack file using the rules of A ins and C ins

	}
	else
		cout<< "Coulnt' open the temp file : "<< argv[2]<<endl;

	printSymbolTable();
	return 0;
}
