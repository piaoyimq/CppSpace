#include <iostream>#include "PriorityQueue.h"#include <time.h>#include <cstdlib>using namespace std;struct MyStruct {	int id;	float amount;	bool operator >( const MyStruct & right ) { return amount > right.amount; }};template <class DataType>void heapsort( Array<DataType> & arr, PriorityQueue<DataType> pq );int main( ){	srand( (unsigned int)( time( NULL ) ) );	MyStruct m;	Array<MyStruct> objs( 100 );	for ( int i = 0; i < 100; i++ ) {		m.id = i;		m.amount = float( rand() % 10000 );		objs[ i ] = m;	}		PriorityQueue<MyStruct> pq( objs );	heapsort( objs, pq );	cout << "Records in sorted order:" << endl;	for ( int i = 0; i < objs.length( ); i++ ) {		cout << "record " << i << ":" << endl;		cout << "	id:     " << objs[ i ].id << endl;		cout << "	amount: " << objs[ i ].amount << endl;	}	return 0;}template <class DataType>void heapsort( Array<DataType> & arr, PriorityQueue<DataType> pq ){	for ( int i = arr.length( ) - 1; i >= 0; i-- ) {		pq.dequeue( arr[ i ] );	}}