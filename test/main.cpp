// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *
 *
 * Copyright (C) 2025 Daniel Wegkamp
 */

 #include "threader.h"

int main( void )
{
	threader *myThreader;
	myThreader = new threader();


	delete myThreader;
	return 0;
}
