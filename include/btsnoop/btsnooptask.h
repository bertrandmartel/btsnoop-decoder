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

#ifdef __ANDROID__
#include "jni.h"
#endif //__ANDROID__

class BtSnoopTask
{

public:

	/**
	 * @brief
	 *      build decoding task
	 */
	BtSnoopTask();

	/**
	 * @brief
	 *      build decoding task with btsnoop file input
	 * @param file_path
	 *       btsnoop file path
	 */
	BtSnoopTask(std::string file_path);

	/**
	 * @brief
	 *      build decoding task with btsnoop file input & packet listener list
	 * @param file_path
	 *       btsnoop file path
	 * @param snoopListenerList
	 *       list of listeners to be notified when a packet is decoded
	 */
	BtSnoopTask(std::string file_path,std::vector<IBtSnoopListener*> *snoopListenerList);

	~BtSnoopTask();

	/**
	 * @brief
	 *      decoding thread task
	 */
	void * decoding_task(void);

	/**
	 * @brief
	 *      decode full snoop file header / packet record data
	 * @param fileStream
	 *      file
	 * @param current_position
	 *      current position of file (initial is 0 / cant be -1)
	 * @return
	 *      new position of file (to match with incoming changes)
	 */
	int decode_streaming_file(std::ifstream *fileStream,int current_position);

	/**
	 * @brief
	 *      decode full snoop file header / packet record data
	 * @return
	 *      success status
	 */
	bool decode_file();

	/**
	 * @brief
	 *      stop decoding : exit control loop
	 */
	void stop();

	/**
	 * @brief
	 *      get file information header object
	 * @return
	 *      file information
	 */
	BtSnoopFileInfo getFileInfo();

	/**
	 * @brief
	 *      get list of decoded packet
	 * @return
	 *      list of btsnoop decoded packets
	 */
	std::vector<BtSnoopPacket> getPacketDataRecords();
	
	static void *decoding_helper(void *context) {
		return ((BtSnoopTask *)context)->decoding_task();
	}

	#ifdef __ANDROID__
	static JavaVM* jvm;
	static jobject jobj;
	static jmethodID mid;
	#endif // __ANDROID__

private:

	/**
	 * btsnoop file path
	 */
	std::string file_path;

	/**
	 * control variable to stop decoding thread if necessary
	 */
	bool task_control;

	/**
	 * decoding state
	 */
	snoop_state state;

	/**
	 * btnsoop file information decoded from the file header
	 */
	BtSnoopFileInfo fileInfo;

	/**
	 * list of listeners waiting for decoded btsnoop packets
	 */
	std::vector<IBtSnoopListener*> *snoopListenerList;

	/**
	 * list of all decoded packets (currently decoded)
	 */
	std::vector<BtSnoopPacket> packetDataRecords;

	/* packet header value (24 o)*/
	char * packet_header;

	/* packet header index count */
	int header_index;

	/* packet data value (dynamic size)*/
	char * packet_data;

	/* packet data index count*/
	int data_index;

	/* current packet*/
	BtSnoopPacket packet;

	/* state for packet record streaming*/
	int packet_record_state;

	#ifdef __ANDROID__
	/*local reference to jni_env attached to JVM*/
	JNIEnv * jni_env;
	#endif // __ANDROID__
};

#endif // BTSNOOPTASK_H
