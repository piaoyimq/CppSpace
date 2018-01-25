//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
#include <boost/interprocess/detail/config_begin.hpp>
//[doc_anonymous_mutexB
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include "doc_anonymous_mutex_shared_data.hpp"
#include <iostream>
#include <cstdio>

using namespace boost::interprocess;

int main ()
{
   //Remove shared memory on destruction
   struct shm_remove
   {
      ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
   } remover;
   //<-
   (void)remover;
   //->

   //Open the shared memory object.
   shared_memory_object shm
      (open_only                    //only create
      ,"MySharedMemory"              //name
      ,read_write  //read-write mode
      );

   //Map the whole shared memory in this process
   mapped_region region
      (shm                       //What to map
      ,read_write //Map it as read-write
      );

   //Get the address of the mapped region
   void * addr       = region.get_address();

   //Construct the shared structure in memory
   shared_memory_log * data = static_cast<shared_memory_log*>(addr);

   //Write some logs
   for(int i = 0; i < 10; ++i){
       std::cout << "____before lock="<< __LINE__ << std::endl;
      //Lock the mutex
      scoped_lock<interprocess_mutex> lock(data->mutex);
      std::cout << "____lock="<< __LINE__ << std::endl;
      std::sprintf(data->items[(data->current_line++) % shared_memory_log::NumItems]
               ,"%s_%d", "process_a", i);
      if(i == (10-1))
         data->end_b = true;
      //Mutex is released here
   }
   std::cout << "unlock" << std::endl;
   //Wait until the other process ends
   while(1){
       std::cout << "____lock="<< __LINE__ << std::endl;
      scoped_lock<interprocess_mutex> lock(data->mutex);
      if(data->end_a)
         break;
   }
   std::cout << "unlock" << std::endl;
   return 0;
}
//]
#include <boost/interprocess/detail/config_end.hpp>
