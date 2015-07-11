        #include "Random.h"
        #include <iostream>

        using namespace std;

            // Test program
        int main( )
        {
            Random r( 1 );

            for( int i = 0; i < 20; i++ )
                cout << r.random0_1( ) << endl;

            return 0;
        }
