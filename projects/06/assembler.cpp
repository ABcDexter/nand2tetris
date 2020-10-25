#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <bitset>
#include <boost/algorithm/string.hpp>

#include <cctype>

using namespace std;

vector<string> arrLines;

map<string, int> dctSym;
map<string, string> dctDest, dctComp, dctJump;


vector<int> arrLineMap;

bool parseLine(const string &sLineIn)
{
  string sLine = sLineIn;
  boost::trim(sLine);

  // Strip trailing comments
  string sComment = "//";
  auto it = search(sLine.begin(), sLine.end(), sComment.begin(), sComment.end());
  if(it != sLine.end())
  {
    sLine = string(sLine.begin(), it);
  }
  
  // ignore blank lines
  if(sLine.length())
  {
    arrLines.push_back(sLine);
    return true;
  }
  
  return false;
}

// Fill default symbols and other lookup tables
void initSyms()
{
  dctSym["SP"] = 0x0000;
  dctSym["LCL"] = 0x0001;
  dctSym["ARG"] = 0x0002;
  dctSym["THIS"] = 0x0003;
  dctSym["THAT"] = 0x0004;
  dctSym["SCREEN"] = 0x4000;
  dctSym["KBD"] = 0x6000;

  for(int i = 0; i < 16; ++i)
  {
    dctSym["R" + to_string(i)] = i;
  }  
  
  dctDest["0"]   = "000";
  dctDest["M"]   = "001";
  dctDest["D"]   = "010";
  dctDest["MD"]  = "011";
  dctDest["A"]   = "100";
  dctDest["AM"]  = "101";
  dctDest["AD"]  = "110";
  dctDest["AMD"] = "111";
  
  dctComp["0"]   = "0101010";
  dctComp["1"]   = "0111111";
  dctComp["-1"]  = "0111010";
  dctComp["D"]   = "0001100";
  dctComp["A"]   = "0110000";
  dctComp["!D"]  = "0001101";
  dctComp["!A"]  = "0110001";
  dctComp["-D"]  = "0001111";
  dctComp["-A"]  = "0110011";
  dctComp["D+1"] = "0011111";
  dctComp["A+1"] = "0110111";
  dctComp["D-1"] = "0001110";
  dctComp["A-1"] = "0110010";
  dctComp["D+A"] = "0000010";
  dctComp["D-A"] = "0010011";
  dctComp["A-D"] = "0000111";
  dctComp["D&A"] = "0000000";
  dctComp["D|A"] = "0010101";
  dctComp["M"]   = "1110000";
  dctComp["!M"]  = "1110001";
  dctComp["-M"]  = "1110011";
  dctComp["M+1"] = "1110111";
  dctComp["M-1"] = "1110010";
  dctComp["D+M"] = "1000010";
  dctComp["D-M"] = "1010011";
  dctComp["M-D"] = "1000111";
  dctComp["D&M"] = "1000000";
  dctComp["D|M"] = "1010101";
  
  
  dctJump["0"  ]="000";
  dctJump["JGT"]="001";
  dctJump["JEQ"]="010";
  dctJump["JGE"]="011";
  dctJump["JLT"]="100";
  dctJump["JNE"]="101";
  dctJump["JLE"]="110";
  dctJump["JMP"]="111";
}


// Throws a syntax error
void syntaxErrIf(bool bCond, int iLine, const string &sErr)
{
  if(bCond)
  {
    throw string("Line " + to_string(arrLineMap[iLine]) + ": " + sErr);
  }
}


// Go through the symbol table and collect the address of the label symbols
void fetchAddrSyms()
{
  int iAddr = 0;
  int iLine = 0;
  for(const auto &sLine: arrLines)
  {
    if(sLine[0] == '(')
    {
      syntaxErrIf(sLine.back() != ')', iLine, "Missing ')'");
      syntaxErrIf(sLine.length() < 3, iLine, "Empty label");
      
      string sSym(sLine.begin() + 1, sLine.end() - 1);
      boost::trim(sSym);
      syntaxErrIf(sSym.length() < 1, iLine, "Empty label");

      dctSym[sSym] = iAddr;
      
      ++iLine;
    }
    else
    {
      ++iAddr;
    }
  }
}

string processA(int iLine, int &iNextSym, const string &sSym)
{
  int iAddr = 0;
  
  try
  {
    // If address starts with a digit assume its a number
    if(sSym[0] >= '0' && sSym[0] <= '9')
    {
      iAddr = stoi(sSym);
      syntaxErrIf(iAddr < 0 || iAddr > 32767, iLine, "Address " + to_string(iAddr) + " out of range");
    }
    else // assume its a symbol
    {
      // Not found, so add it
      if(!dctSym.count(sSym))
      {
        dctSym[sSym] = iNextSym++;
      }
      
      // Get the address we know
      iAddr = dctSym[sSym];

      // Have we run out of variables in RAM?
      syntaxErrIf(iNextSym == 0x4000, iLine, "Memory exhausted");
    }
  }
  catch(invalid_argument &e)
  {
    syntaxErrIf(true, iLine, "Invalid address");
  }
  catch(out_of_range &e)
  {
    syntaxErrIf(true, iLine, "Invalid address");
  }

  // Output the instruction
  bitset<16> bits(iAddr);
  return bits.to_string();
}

//C instruction
string processC(int iLine, const string &sLine)
{
  string sDestBits = "000", sCompBits="0000000", sJumpBits = "000";

  //C instruction format
  // dest=comp;jmp
  // if dest is empty, = is omitted
  // if jmp is empty ; is omitted
  
  // Find the "=" symbol
  auto itDestEnd = find(sLine.begin(), sLine.end(), '=');
  if(itDestEnd != sLine.end())
  {
    // Look up the bits for the dest field based on the part upto the =
    string sDest(sLine.begin(), itDestEnd);
    boost::trim(sDest);
    syntaxErrIf(!dctDest.count(sDest), iLine, "Invalid destination " + sDest);
    sDestBits = dctDest[sDest];
  }
  
  // if dest is empty, take the string start, else take the char after =
  auto itCompStart = itDestEnd != sLine.end() ? itDestEnd + 1 : sLine.begin();
  
  if(itCompStart != sLine.end())
  {
    // Find the ";" if any
    auto itCompEnd = find(itCompStart, sLine.end(), ';');
    string sComp(itCompStart, itCompEnd);
    
    boost::trim(sComp);
    syntaxErrIf(!dctComp.count(sComp), iLine, "Invalid computation " + sComp);
    sCompBits = dctComp[sComp];
    
    //Check for any JMP 
    if(itCompEnd != sLine.end())
    {
      auto itJMPStart = itCompEnd + 1;
      string sJmp(itJMPStart, sLine.end());

      boost::trim(sJmp);
      syntaxErrIf(!dctJump.count(sJmp), iLine, "Invalid jump " + sJmp);
      sJumpBits = dctJump[sJmp];
    }
  }
  
  return "111" + sCompBits + sDestBits + sJumpBits;
}


vector<string> assemble()
{
  vector<string> arrOutput;
  
  int iLine = 0, iAddr = 0;
  int iNextSym = 16;
  for(const auto &sLine: arrLines)
  {
    cerr << "Assembling..." << iLine << "     \r";
    
    if(sLine[0] != '(')
    {
      // A instruction
      if(sLine[0] == '@')
      {
        string sSym = sLine.substr(1);
        string sInstruction = processA(iLine, iNextSym, sSym);
        arrOutput.push_back(sInstruction);
      }
      else // C instruction
      {
        string sInstruction = processC(iLine, sLine);
        arrOutput.push_back(sInstruction);
      }
      
      ++iAddr;
    }
    
    ++iLine;
  }
  
  cerr << endl;
  
  return arrOutput;
}

int main(int argc, char **argv)
{
  
  try
  {
    if(argc < 2)
    {
      throw string("Usage: ") + argv[0] + " <hackfile>";
    }
    
    // Open file
    ifstream ifs(argv[1]);
    if(!ifs)
    {
      throw string("Can't open file: ") + argv[1];
    }
    
    // Parse - trim lines, remove comments
    string sLine;
    int iFileLine = 1;
    while(getline(ifs, sLine))
    {
      cerr << "Parsing line ..." << iFileLine << "    \r";
      
      if(parseLine(sLine))
      {
        // Maintain a map of code lines to file lines
        arrLineMap.push_back(iFileLine);
      }
      ++iFileLine;
    }
    
    cerr << endl;
    
    
    // Adds default symbols
    initSyms();
    
    // fetch all the code label symbols
    fetchAddrSyms();
    
    // Create the code
    const auto &arrBin = assemble();
    
    string sBaseName = argv[1];
    auto iDot = sBaseName.find_last_of('.');
    if(iDot != string::npos)
    {
      sBaseName = sBaseName.substr(0, iDot);
    }

    cerr << "Saving... " << sBaseName + ".hack" << endl;
    
    ofstream ofs(sBaseName + ".hack");
    for(const auto &s: arrBin)
    {
      ofs << s << endl;
    }
    
  }
  catch(string s)
  {
    cerr << endl << s << endl;
    return -1;
  }
  
  
  return 0;
}
