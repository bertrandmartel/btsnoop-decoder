#ifndef BTSNOOPTASK_H
#define BTSNOOPTASK_H

#include "string"
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
#include "fstream"
#include "btsnoop/btsnoopstate.h"
#include "btsnoop/btsnoopfileinfo.h"
#include "btsnoop/btsnooppacket.h"
#include "ibtsnooplistener.h"

class BtSnoopTask
{

public:

	BtSnoopTask();

	BtSnoopTask(std::string file_path);

	BtSnoopTask(std::string file_path,std::vector<IBtSnoopListener*> *snoopListenerList);

	~BtSnoopTask();

	void * decoding_task(void);

	int decode_file(std::ifstream *fileStream,int current_position);

	bool decode_file();

	void stop();

	BtSnoopFileInfo getFileInfo();

	std::vector<BtSnoopPacket> getPacketDataRecords();
	
	static void *decoding_helper(void *context) {
		return ((BtSnoopTask *)context)->decoding_task();
	}

private:

	std::string file_path;

	bool task_control;

	snoop_state state;

	BtSnoopFileInfo fileInfo;

	bool control;

	std::vector<IBtSnoopListener*> *snoopListenerList;

	std::vector<BtSnoopPacket> packetDataRecords;

};

#endif // BTSNOOPTASK_H
