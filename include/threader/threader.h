// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *
 *
 * Copyright (C) 2025 Daniel Wegkamp
 */

#ifndef threader_h
#define threader_h

#include <thread>
#include <string>
#include <chrono>

using namespace std;
using namespace std::chrono;

class threader
{
public:
	threader();
	virtual ~threader();

	void start( int myid );
	void loop();
	void loop_delay();
	void stop();
	void sleep( int ms );
	void setDelay( int ms );
	void setDelay_us( int us );

	//autonomous looper for child classes (make sure this doesnt get stuck!!):
	virtual void looped();


protected:		//can be seen from child class
	bool		running = false;

	int			id = 0;
	string		className = "threader";

	long		counter = 0;
	int 		delay_us = 0;



private:
	thread  *theThread = nullptr;

	microseconds idleTime;
	time_point<std::chrono::high_resolution_clock> lastTime, thisTime;

};



#endif
