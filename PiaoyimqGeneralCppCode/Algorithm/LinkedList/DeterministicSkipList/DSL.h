        #ifndef DSL_H_
        #define DSL_H_

        #include "dsexceptions.h"
        #include <iostream>        // For NULL

        using namespace std;

        // Deterministic skip list class class
        //
        // CONSTRUCTION: with INFINITY object that is
        //               also used to signal failed finds
        //
        // ******************PUBLIC OPERATIONS*********************
        // void insert( x )       --> Insert x
        // void remove( x )       --> Remove x (unimplemented)
        // Comparable find( x )   --> Return item that matches x
        // Comparable findMin( )  --> Return smallest item
        // Comparable findMax( )  --> Return largest item
        // boolean isEmpty( )     --> Return true if empty; else false
        // void makeEmpty( )      --> Remove all items
        // void printList( )      --> Print items in sorted order


          // Node and forward declaration because g++ does
          // not understand nested classes.
        template <class Comparable>
        class DSL;

        template <class Comparable>
        class SkipNode
        {
            Comparable element;
            SkipNode  *right;
            SkipNode  *down;

            SkipNode( const Comparable & theElement = Comparable( ),
                               SkipNode *rt = NULL, SkipNode *dt = NULL )
              : element( theElement ), right( rt ), down( dt ) { }

            friend class DSL<Comparable>;
        };


        template <class Comparable>
        class DSL
        {
          public:
            explicit DSL( const Comparable & inf );
            DSL( const DSL & rhs );
            ~DSL( );

            const Comparable & findMin( ) const;
            const Comparable & findMax( ) const;
            const Comparable & find( const Comparable & x ) const;
            bool isEmpty( ) const;
            void printList( ) const;

            void makeEmpty( );
            void insert( const Comparable & x );
            void remove( const Comparable & x );

            const DSL & operator=( const DSL & rhs );
          private:
            const Comparable INFINITY;
            SkipNode<Comparable> *header;  // The list
            SkipNode<Comparable> *bottom;
            SkipNode<Comparable> *tail;

            const Comparable & elementAt( SkipNode<Comparable> * t ) const;

                // Usual recursive stuff
            void reclaimMemory( SkipNode<Comparable> *t ) const;
        };

        /**
                 * Construct the tree.
                 * inf is the largest Comparable
                 * and is used to signal failed finds.
                 */
                template <class Comparable>
                DSL<Comparable>::DSL( const Comparable & inf ) : INFINITY( inf )
                {
                    bottom = new SkipNode<Comparable>( );
                    bottom->right = bottom->down = bottom;
                    tail   = new SkipNode<Comparable>( INFINITY );
                    tail->right = tail;
                    header = new SkipNode<Comparable>( INFINITY, tail, bottom );
                }

                /**
                 * Copy constructor.
                 * Left as an exercise.
                 */
                template <class Comparable>
                DSL<Comparable>::DSL( const DSL<Comparable> & rhs ) : INFINITY( rhs.INFINITY)
                {
                    cout << "Copy constructor is unimplemented" << endl;
                }

                /**
                 * Destructor.
                 */
                template <class Comparable>
                DSL<Comparable>::~DSL( )
                {
                    makeEmpty( );
                    delete header;
                    delete tail;
                    delete bottom;
                }

                /**
                 * Insert item x into the DSL.
                 */
                template <class Comparable>
                void DSL<Comparable>::insert( const Comparable & x )
                {
                    SkipNode<Comparable> *current = header;

                    bottom->element = x;
                    while( current != bottom )
                    {
                        while( current->element < x )
                            current = current->right;

                        // If gap size is 3 or at bottom level and
                        // must insert, then promote middle element
                        if( current->down->right->right->element < current->element )
                        {
                            current->right = new SkipNode<Comparable>( current->element,
                                             current->right, current->down->right->right );
                            current->element = current->down->right->element;
                        }
                        else
                            current = current->down;
                    }

                    // Raise height of DSL if necessary
                    if( header->right != tail )
                        header = new SkipNode<Comparable>( INFINITY, tail, header );
                }

                /**
                 * Remove item x from the DSL. Unimplemented.
                 */
                template <class Comparable>
                void DSL<Comparable>::remove( const Comparable & x )
                {
                    cout << "Sorry, remove unimplemented; " << x <<
                         " still present" << endl;
                }

                /**
                 * Find the smallest item in the tree.
                 * Return smallest item or INFINITY if empty.
                 */
                template <class Comparable>
                const Comparable & DSL<Comparable>::findMin( ) const
                {
                    if( isEmpty( ) )
                        return INFINITY;

                    SkipNode<Comparable> *current = header;
                    while( current->down != bottom )
                        current = current->down;

                    return elementAt( current );
                }

                /**
                 * Find the largest item in the tree.
                 * Return the largest item or INFINITY if empty.
                 */
                template <class Comparable>
                const Comparable & DSL<Comparable>::findMax( ) const
                {
                    if( isEmpty( ) )
                        return INFINITY;

                    SkipNode<Comparable> *current = header;
                    for( ; ; )
                         if( current->right->right != tail )
                             current = current->right;
                         else if( current->down != bottom )
                             current = current->down;
                         else
                             return elementAt( current );
                }

                /**
                 * Find item x in the tree.
                 * Return the matching item or INFINITY if not found.
                 */
                template <class Comparable>
                const Comparable & DSL<Comparable>::find( const Comparable & x ) const
                {
                    SkipNode<Comparable> *current = header;

                    bottom->element = x;
                    for( ; ; )
                        if( x < current->element )
                            current = current->down;
                        else if( current->element < x )
                            current = current->right;
                        else
                            return elementAt( current );
                }

                /**
                 * Make the tree logically empty.
                 */
                template <class Comparable>
                void DSL<Comparable>::makeEmpty( )
                {
                    reclaimMemory( header );
                    header->right = tail;
                    header->down = bottom;
                }

                /**
                 * Test if the tree is logically empty.
                 * Return true if empty, false otherwise.
                 */
                template <class Comparable>
                bool DSL<Comparable>::isEmpty( ) const
                {
                    return header->right == tail && header->down == bottom;
                }

                /**
                 * Internal method to get element field from node t.
                 * Return the element field or INFINITY if t is at the bottom.
                 */
                template <class Comparable>
                const Comparable & DSL<Comparable>::
                elementAt( SkipNode<Comparable> *t ) const
                {
                    if( t == bottom )
                       return INFINITY;
                    else
                       return t->element;
                }

                /**
                 * Print the DSL.
                 */
                template <class Comparable>
                void DSL<Comparable>::printList( ) const
                {
                    SkipNode<Comparable> *current = header;

                    while( current->down != bottom )
                         current = current->down;

                    while( current->right != tail )
                    {
                        cout << current->element << endl;
                        current = current->right;
                    }
                }

                /**
                 * Deep copy. Left as an exercise
                 */
                template <class Comparable>
                const DSL<Comparable> &
                DSL<Comparable>::operator=( const DSL<Comparable> & rhs )
                {
                    if( this != &rhs )
                        cout << "Sorry, operator= is unimplemented" << endl;

                    return *this;
                }

                /**
                 * reclaimMemory is left as an exercise.
                 * Hint: delete from top level to bottom level.
                 */
                template <class Comparable>
                void DSL<Comparable>::reclaimMemory( SkipNode<Comparable> *t ) const
                {
                    if( t != bottom )
                        cout << "reclaimMemory is unimplemented -- leaking!" << endl;
                }
        #endif
