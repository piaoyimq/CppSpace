        #include <iostream>
        #include "StackAr.h"

        using namespace std;

        int main( )
        {
            Stack<int> s;

            for( int i = 0; i < 10; i++ )
                s.push( i );
        
            while( !s.isEmpty( ) )
                cout << s.topAndPop( ) << endl;

            return 0;
        } 
