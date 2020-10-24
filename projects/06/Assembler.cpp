#include<bits/stdc++.h>

using namespace std;

//string symbols()


string asm2bin(string &inp){
	string ans;
	int len = inp.size();
	if (inp[0] == '@') // A ins
	{	// A instructions start with 1
		ans += '1';
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
	{	// C instructions start with 111
		ans += "111";
		string addr ; 
		

		
	}
	return ans;
}

int main(int argc, char **argv)
{
	std::cout<<"Assembler!";
	int i,n,t;
	cout << "You have entered " << argc << " arguments:" << "\n"; 
	for (i = 0; i < argc; ++i)
		cout << argv[i] << "\n"; 

	ifstream asmFile(argv[1]);
	if (asmFile.is_open())
	{
		string line;
		ofstream binFile;
		binFile.open(argv[2]); //open the file given by argv

		while( getline(asmFile, line))
		{ 
			cout << line << endl;
			n = line.size(); cout << "length of this line : " <<n<<endl;	
			string temp; 
			for (i = 0; i < n ; ++i)	
			{
				if (line[i] == '/' && line[i] == line[i+1])
					break;				
				else
					temp += line[i];
			}
			if ( temp.size() > 1)  //at least 2 characters
				binFile << asm2bin(temp) << "\n";
		}
		binFile.close();
	
	asmFile.close();
	}
	else
		cout<< "Coulnt' open the file : "<< argv[1]<<endl;
	
	return 0;
}
