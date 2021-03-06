#pragma warning (disable: 4786)

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <string>
#include <vector>

using namespace std;

ostream & operator<<( ostream & out, const pair<string, vector<int> > & rhs )
{
    out << rhs.first << ": " << '\t' << rhs.second[ 0 ];
    for( int i = 1; i < rhs.second.size( ); i++ )
        out << ", " << rhs.second[ i ];
    return out;
}

int main( int argc, char *argv[ ] )
{
    if( argc != 2 )
    {
        cerr << "Usage: " << argv[ 0 ] << " filename" << endl;
        return 1;
    }

    ifstream inFile( argv[ 1 ] );
    if( !inFile )
    {
        cerr << "Cannot open " << argv[ 1 ] << endl;
        return 1;
    }

    string                              oneLine;
    map<string, vector<int> >           concordance;

        // Read the words; add them to wordMap
    for( int lineNum = 1; getline( inFile, oneLine ); lineNum++ )
    {
        istringstream st( oneLine );

        string word;

        while( st >> word )
            concordance[ word ].push_back( lineNum );
    }

    map<string, vector<int> >::iterator itr;
    for( itr = concordance.begin( ); itr != concordance.end( ); itr++ )
        cout << *itr << endl;

    return 0;
}

