#include "parser.hpp"

Data mydata[MAX_DATA];
int mdi = 0;	// Index in mydata array


int main()
{
	TokenParser parser;
    	string line;
	parser.SetStartCallback( ShowCounters );
    	while(std::getline(cin,line)) 
	{
        	parser.Parse(line);
    	}
	parser.SetEndCallback( ShowValues );
	parser.SetDigitTokenCallback( ShowOdd );
	parser.SetDigitTokenCallback( ShowEven );

	return(0);
}
