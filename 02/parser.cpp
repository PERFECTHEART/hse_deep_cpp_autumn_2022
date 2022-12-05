#include "parser.hpp"

extern int mdi;
extern Data mydata[];
std::string MAX_UINT64 = std::to_string((uint64_t) -1);
// 18446744073709551615

void TokenParser::Parse(const std::string &line)
{
	for( unsigned int i = 0; i < line.length(); ) 
	{
		i = mydata[mdi].Store( line, i );
	}
}

int TokenParser::SetStartCallback( void (*f)() )
{
	f();
	return( 0 );
}

int TokenParser::SetEndCallback( void (*f)() )
{
	f();
	return( 0 );
}

void Data::ShowValue( char s1[], char s2[] )
{
	if( len == 0 )
	{
		cout << s2[0] << num << s2[1];
	}
	else
	{
		cout << s1[0] << txt << s1[1];
	}
	cout << " ";
}

void ShowCounters()
{
	cout << mdi << " strings/numbers buffered. Limit: " << MAX_DATA << "\n";
}

void ShowValues()
{
	for( int i = 0; i < mdi; i++ )
		mydata[i].ShowValue( (char *) "()", (char *) "##" );
	if ( mdi > 0 )
		cout << "\n";
}

unsigned int Data::Store( std::string str, unsigned int pos)
{																																								
	std::locale	loc;
	bool isnumber = true;
	unsigned int fin;

	while( std::isspace( str[pos], loc ) && str.length() > pos )
	{
		pos++;
	}
	fin = pos;
	while( !std::isspace(str[fin], loc ) && fin < str.length() )
	{
		if( ! std::isdigit( str[fin], loc ) )
		{
			isnumber = false;
		}
		fin++;
	}

	// Check whether number fits limit
	if( fin != pos )
	{
		if( isnumber )
		{
			// Truncate leading zeros
			while(str[pos] == '0' && str.length() > 1)
				pos++;
			if( fin-pos > MAX_UINT64.length() || 
				( fin-pos == MAX_UINT64.length() && (MAX_UINT64.compare( str.substr(pos,fin-pos) ) < 0) ) )
			{
				isnumber = false;
			}
			
		}
		if( isnumber )
		{
			std::istringstream iss( str.substr(pos,fin-pos) );

			iss >> num;
			len = 0;
			mdi++;
		}
		else
		{
			if( fin > str.length() )
			{
				fin = str.length();
			}
			txt = str.substr( pos, fin - pos);
			len = fin - pos;
			if( len > 0 )
			{
				mdi++;
			}
		}
		// Rewind to next nonblanc symbol
		while( std::isspace(str[fin]) && fin < str.length() )
		{
			fin++;
		}
	}
	return( fin );
}

typedef int (*MyFuncPtr)(uint64_t);
int TokenParser::SetDigitTokenCallback( MyFuncPtr f )
{
	int sum = 0;
	for( int i = 0; i < mdi; i++ )
		if( mydata[i].len == 0 )
			sum += f( mydata[i].num );
	if( sum != 0 )
		cout << "\nTotal count: " << sum << "\n";
	return( 0 );
}

int ShowOdd( uint64_t val )
{
	if( val % 2 > 0 )
	{
		cout << val << " ";
		return( 1 );
	}
	else
		return( 0 );
}

int ShowEven( uint64_t val )
{
	if( val % 2 == 0 )
	{
		cout << val << " ";
		return( 1 );
	}
	else
		return( 0 );
}
