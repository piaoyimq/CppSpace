        #ifndef BINARY_HEAP_H_
        #define BINARY_HEAP_H_

        #include "dsexceptions.h"
        #include <vector>

	using namespace std;

        // BinaryHeap class
        //
        // CONSTRUCTION: with an optional capacity (that defaults to 100)
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // deleteMin( minItem )   --> Remove (and optionally return) smallest item
        // Comparable findMin( )  --> Return smallest item
        // bool isEmpty( )        --> Return true if empty; else false
        // bool isFull( )         --> Return true if full; else false
        // void makeEmpty( )      --> Remove all items
        // ******************ERRORS********************************
        // Throws Underflow and Overflow as warranted

        template <class Comparable>
        class BinaryHeap
        {
          public:
            explicit BinaryHeap( int capacity = 100 );

            bool isEmpty( ) const;
            bool isFull( ) const;
            const Comparable & findMin( ) const;

            void insert( const Comparable & x );
            void deleteMin( );
            void deleteMin( Comparable & minItem );
            void makeEmpty( );

          private:
            int                currentSize;  // Number of elements in heap
            vector<Comparable> array;        // The heap array

            void buildHeap( );
            void percolateDown( int hole );
        };

        /**
                * Construct the binary heap.
                * capacity is the capacity of the binary heap.
                */
               template <class Comparable>
               BinaryHeap<Comparable>::BinaryHeap( int capacity )
                 : array( capacity + 1 ), currentSize( 0 )
               {
               }

               /**
                * Insert item x into the priority queue, maintaining heap order.
                * Duplicates are allowed.
                * Throw Overflow if container is full.
                */
               template <class Comparable>
               void BinaryHeap<Comparable>::insert( const Comparable & x )
               {
                   if( isFull( ) )
                       throw Overflow( );

                       // Percolate up
                   int hole = ++currentSize;
                   for( ; hole > 1 && x < array[ hole / 2 ]; hole /= 2 )
                       array[ hole ] = array[ hole / 2 ];
                   array[ hole ] = x;
               }

               /**
                * Find the smallest item in the priority queue.
                * Return the smallest item, or throw Underflow if empty.
                */
               template <class Comparable>
               const Comparable & BinaryHeap<Comparable>::findMin( ) const
               {
                   if( isEmpty( ) )
                       throw Underflow( );
                   return array[ 1 ];
               }

               /**
                * Remove the smallest item from the priority queue.
                * Throw Underflow if empty.
                */
               template <class Comparable>
               void BinaryHeap<Comparable>::deleteMin( )
               {
                   if( isEmpty( ) )
                       throw Underflow( );

                   array[ 1 ] = array[ currentSize-- ];
                   percolateDown( 1 );
               }

               /**
                * Remove the smallest item from the priority queue
                * and place it in minItem. Throw Underflow if empty.
                */
               template <class Comparable>
               void BinaryHeap<Comparable>::deleteMin( Comparable & minItem )
               {
                   if( isEmpty( ) )
                       throw Underflow( );

                   minItem = array[ 1 ];
                   array[ 1 ] = array[ currentSize-- ];
                   percolateDown( 1 );
               }

               /**
                * Establish heap order property from an arbitrary
                * arrangement of items. Runs in linear time.
                */
               template <class Comparable>
               void BinaryHeap<Comparable>::buildHeap( )
               {
                   for( int i = currentSize / 2; i > 0; i-- )
                       percolateDown( i );
               }

               /**
                * Test if the priority queue is logically empty.
                * Return true if empty, false otherwise.
                */
               template <class Comparable>
               bool BinaryHeap<Comparable>::isEmpty( ) const
               {
                   return currentSize == 0;
               }

               /**
                * Test if the priority queue is logically full.
                * Return true if full, false otherwise.
                */
               template <class Comparable>
               bool BinaryHeap<Comparable>::isFull( ) const
               {
                   return currentSize == array.size( ) - 1;
               }

               /**
                * Make the priority queue logically empty.
                */
               template <class Comparable>
               void BinaryHeap<Comparable>::makeEmpty( )
               {
                   currentSize = 0;
               }

               /**
                * Internal method to percolate down in the heap.
                * hole is the index at which the percolate begins.
                */
               template <class Comparable>
               void BinaryHeap<Comparable>::percolateDown( int hole )
               {
       /* 1*/      int child;
       /* 2*/      Comparable tmp = array[ hole ];

       /* 3*/      for( ; hole * 2 <= currentSize; hole = child )
                   {
       /* 4*/          child = hole * 2;
       /* 5*/          if( child != currentSize && array[ child + 1 ] < array[ child ] )
       /* 6*/              child++;
       /* 7*/          if( array[ child ] < tmp )
       /* 8*/              array[ hole ] = array[ child ];
                       else
       /* 9*/              break;
                   }
       /*10*/      array[ hole ] = tmp;
               }
        #endif
