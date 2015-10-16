/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Bertrand Martel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
/**
	btsnoopparsert.h

	Parse and manage task for streaming bt snoop file

	@author Bertrand Martel
	@version 0.1
*/

#ifndef BTSNOOPPARSER_H
#define BTSNOOPPARSER_H

#include "btsnoopstate.h"
#include "fstream"
#include "vector"
#include "ibtsnooplistener.h"
#include "btsnoopfileinfo.h"
#include "pthread.h"
#include "btsnoop/btsnooptask.h"

class BtSnoopParser
{

public:

	/**
	 * @brief BtSnoopParser
	 *      initialize bt snoop file parser
	 */
	BtSnoopParser();

	/**
	 * stop and join thread
	 **/
	~BtSnoopParser();

	/**
	 * @brief addSnoopListener
	 *      add a listener to monitor streamed packet record
	 * @param listener
	 */
	void addSnoopListener(IBtSnoopListener* listener);

	/**
	 * @brief decode_streaming_file
	 *      decode streaming file (non blocking method)
	 * @param file_path
	 * @return
	 *      success status
	 */
	bool decode_streaming_file(std::string file_path);

	/**
	 * @brief join
	 *      wait for thread to finish (blocking method)
	 */
	void join();

private:

	/**
	 * @brief decode_task
	 *      decode thread task
	 */
	pthread_t decode_task;

	/**
	 * @brief snoop_task
	 *      parser object manager
	 */
	BtSnoopTask snoop_task;

	/**
	 * @brief snoopListenerList
	 *      list of listener registered
	 */
	std::vector<IBtSnoopListener*> snoopListenerList;

	/**
	 * @brief thread_started
	 *      define if a thread has already been created before
	 */
	bool thread_started;
};

#endif // BTSNOOPPARSER_H
