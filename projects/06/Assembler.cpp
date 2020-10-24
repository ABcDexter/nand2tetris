#include<bits/stdc++.h>

using namespace std;

//string symbols()


string asm2bin(string &inp){
	string ans;
	int len = inp.size();
	cout <<"INPUT : "<< inp <<endl;
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
		ans += "111"; //012	//3 4  5  6  7  8  9    10 11 12   13 14 15
		ans += "0000000000000"; //a c1 c2 c3 c4 c5 c6 = d1 d2 d3 ; j1 j2 j3
		
		// dest logic
		string dest;
		int iter=0;
		for (; iter<len ; iter++){
			if (inp[iter] == '=')
				break;
			else
				dest += inp[iter];
		}
		cout << "dest is " << dest<<endl;
			if ( dest == "M") ans[12] = '1';

			if ( dest == "D") ans[11] = '1';

			if ( dest == "MD") ans[11] = ans[12] = '1';

			if ( dest == "A") ans[10] = '1';

			if ( dest == "AM") ans[10] = ans[12] = '1';

			if ( dest == "AD") ans[10] = ans[11] = '1';

			if ( dest == "AMD") ans[10] = ans[11] = ans[12] = '1';

			//default : //all are already 000

		cout <<"DEST done!! ans so far : "<<ans<<endl<<" iter : inp[iter] = "<<iter<<":"<<inp[iter] <<"\n\n";
		
		// comp logic
		iter +=1;		
			string comp;
			for (; iter<len ; iter++){
				if ( (inp[iter] == ';') || (inp[iter] == '\n' ))
					break;
				else
					comp += inp[iter];
			}
			cout << "comp is " << comp<<endl;
			//ans += "111"; //012	  //3  4  5  6  7  8  9   10 11 12   13 14 15
			//ans += "0000000000000"; //a c1 c2 c3 c4 c5 c6 = d1 d2 d3 ; j1 j2 j3
		
		
			if ( comp == "0") ans[4] = ans[6] = ans[8] = '1';

			if ( comp == "1") ans[4] = ans[5] = ans[6] = ans[7] = ans[8] = ans[9] = '1';

			if ( comp == "-1") ans[4] = ans[5] = ans[6] = ans[8] = '1';

			if ( comp == "D") ans[6] = ans[7] = '1';

			if ( comp == "A") ans[4] = ans[5] = '1';
			if ( comp == "M") ans[3] = ans[4] = ans[5] = '1';

			if ( comp == "!D") ans[6] = ans[7] = ans[9] = '1';		
			
			if ( comp == "!A") ans[4] = ans[5] = ans[9] = '1';
			if ( comp == "!M") ans[3] = ans[4] = ans[5] = ans[9] = '1';		

			if ( comp == "-D") ans[15] = '1';

			if ( comp == "-A") ans[15] = '1';
			if ( comp == "-M") ans[15] = '1';

			if ( comp == "D+1") ans[15] = '1';

			if ( comp == "A+1") ans[15] = '1';
			if ( comp == "M+1") ans[15] = '1';

			if ( comp == "D-1") ans[15] = '1';

			if ( comp == "A-1") ans[15] = '1';
			if ( comp == "M-1") ans[15] = '1';

			if ( comp == "D+A") ans[15] = '1';
			if ( comp == "D+M") ans[15] = '1';

			if ( comp == "D-A") ans[15] = '1';
			if ( comp == "D-M") ans[15] = '1';

			if ( comp == "A-D") ans[15] = '1';
			if ( comp == "M-D") ans[15] = '1';

			if ( comp == "D&A") ans[15] = '1';
			if ( comp == "D&M") ans[15] = '1';

			if ( comp == "D|A") ans[15] = '1';
			if ( comp == "D|M") ans[15] = '1';
		cout <<"COMP done!! ans so far : "<<ans<<endl <<" iter : inp[iter] = "<<iter<<":"<<inp[iter] <<"\n\n";
				

		if (inp[iter] == ';')
		{		
			// jump logic
			string jump;
			for (; iter<len ; iter++){
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
