// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *
 *
 * Copyright (C) 2025 Daniel Wegkamp
 */

 #include <threader/threader.h>

#include "debug.h"

#include <iostream>
#include <chrono>
#include <sys/syscall.h>
#include <unistd.h>


threader::threader()
{
	DEBUG_MSG("threader::threader()");
	//called before child class constructor

	idleTime = std::chrono::milliseconds(delay_us);
	lastTime = chrono::high_resolution_clock::now();
}

threader::~threader()
{
	DEBUG_MSG( "threader::~threader(), id: " << id << " delay: " << delay_us << " us");
	delete theThread;
}

void threader::setDelay( int ms )
{
	int us = 1000*ms;
	DEBUG_MSG( "threader::setDelay(), delay_ms: " << ms );
	if ( (us != 0 && delay_us !=0) || delay_us == us || !running ) 	//no need to restart thread
	{
			delay_us = us;
			idleTime = std::chrono::microseconds(delay_us);

	}
	else		//need to switch thread because it is already running and the delay changes from zero to nonzero
	{
			delay_us = us;
			idleTime = std::chrono::microseconds(delay_us);

			running = false;	//stop thread
			theThread->join();
			delete theThread;
			start( id );		//restart thread
	}
}

void threader::setDelay_us( int us )
{
	DEBUG_MSG( "threader::setDelay(), delay_us: " << us );
	if ( (us != 0 && delay_us !=0) || delay_us == us || !running ) 	//no need to restart thread
	{
			delay_us = us;
			idleTime = std::chrono::microseconds(delay_us);
	}
	else		//need to switch thread because it is already running and the delay changes from zero to nonzero
	{
			delay_us = us;
			idleTime = std::chrono::microseconds(delay_us);

			running = false;	//stop thread
			theThread->join();
			delete theThread;
			start( id );		//restart thread
	}
}

void threader::start( int myid )
{
	id = myid;
	running = true;

	if( delay_us == 0 )
	{
			DEBUG_MSG( "threader::start() - no delay, id: " << id << ", class: " << className );
			theThread = new thread( &threader::loop, this );		//the thread will be associated with threader::loop() below
	}
	else
	{
			DEBUG_MSG( "threader::start() - with delay, id: " << id << ", class: " << className );
			theThread = new thread( &threader::loop_delay, this );		//the thread will be associated with threader::loop() below
	}
}


void threader::loop()
{
	DEBUG_MSG( "threader::loop(), id: " << id );
	while( running )
	{
		counter++;
		this->looped();										//this looped is usually overwritten by child class and executes specific code!
	}
	//exit thread:
	DEBUG_MSG( "exit loop() PID: " << syscall(SYS_gettid) );
}

void threader::loop_delay()
{
	DEBUG_MSG( "threader::loop(), id: " << id );
	while( running )
	{
		thisTime = high_resolution_clock::now();
	    microseconds duration = duration_cast<microseconds>(thisTime - lastTime);
		if( duration >= idleTime )								//seems to be the most reliable method for "waiting"
		{
			counter++;
			this->looped();										//this looped is usually overwritten by child class and executes specific code!
			lastTime = high_resolution_clock::now();
		}

		//sleeping returns control to cpu and is unpredictable
		//std::this_thread::sleep_for( idleTime ); 250k us->250ms
		//nanosleep(&idleTimespec, NULL); 250k us->250ms
		//cout << "delay";		//1000us, 1ms
	}

	//exit thread:
	DEBUG_MSG( "exit loop_delay() PID: " << syscall(SYS_gettid) );
}

void threader::stop()	//stops loop and joins thread. Doing this in the destructor did not work well... probably because child class was destructed first.
{
	running = false;

	if ( theThread != nullptr )
	{
		DEBUG_MSG( "threader::stop(), id: " << id << ", class name: " << className << " - "<< "counter: " << counter );
		theThread->join();			//avoid segmentation fault
	}
	else
	{
		DEBUG_MSG( "threader::stop(), id: " << id << ", class name: " << className << " - "<< "counter: " << counter << " - was never started!");
	}
}

void threader::sleep( int ms )
{
	std::this_thread::sleep_for( std::chrono::milliseconds(ms) );
}

// this is the part that threader::loop() loops over. loop() is executed in a separate thread.					IS THIS CALLED IF NOT IMPLEMENTED IN SUBCLASS
void threader::looped()
{
	running = false;
	cout << "threader::looped(), id: " << id << " abort!" << endl;
}
