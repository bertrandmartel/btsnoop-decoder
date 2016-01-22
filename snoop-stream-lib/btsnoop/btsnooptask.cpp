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
	btsnooptask.cpp

	Monitoring implementation of ibtsnooplistener

	@author Bertrand Martel
	@version 0.1
*/

#include "btsnooptask.h"
#include <fstream>
#include "btsnoopfileinfo.h"
#include "btsnooppacket.h"
#include "iostream"

#ifdef __ANDROID__

#include "android/log.h"

#endif

using namespace std;

/**
 * @brief BtSnoopTask::BtSnoopTask
 *
 */
BtSnoopTask::BtSnoopTask(){
}

/**
 * @brief BtSnoopTask::BtSnoopTask
 *
 */
BtSnoopTask::BtSnoopTask(std::string file_path){
	this->snoopListenerList=0;
	this->file_path=file_path;
	task_control=false;
	state = FILE_HEADER;

}

/**
 * @brief BtSnoopTask::BtSnoopTask
 *          build a BtSnoopTask object with file path and a list of listener (for standalone streaming task)
 * @param file_path
 *      snoop file path
 * @param snoopListenerList
 *      list of listeners
 */
BtSnoopTask::BtSnoopTask(std::string file_path,std::vector<IBtSnoopListener*> *snoopListenerList){

	this->file_path=file_path;
	this->snoopListenerList=snoopListenerList;
	task_control=false;
	state = FILE_HEADER;

}

/**
 * @brief BtSnoopTask::~BtSnoopTask
 *      exit control loop
 */
BtSnoopTask::~BtSnoopTask(){
	 task_control=false;
}

/**
 * @brief BtSnoopTask::stop
 *      stop decoding : exit control loop
 */
void BtSnoopTask::stop(){
	task_control=false;
}

/**
 * @brief BtSnoopTask::decoding_task
 *      streaming decoding / monitoring snoop file for changes
 * @return
 */
void * BtSnoopTask::decoding_task(void) {

	packetDataRecords.clear();
	task_control=true;

	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = 1000000L * 200;

	int index = 0;

	while (task_control) {

		ifstream fileStream(file_path.c_str());

		if (fileStream.is_open()) {

			fileStream.seekg (0, fileStream.end);
			int length = fileStream.tellg();

			fileStream.seekg(index,ios::beg);

			if (!fileStream.eof() && fileStream.tellg()!=-1 && length!=index){

				index = decode_streaming_file(&fileStream,index);
			}

		}
		else{

			#ifdef __ANDROID__

			__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","file could not be opened");

			#else

			cout << "file could not be opened" << endl;
			
			#endif // __ANDROID__

			return 0;
		}
		nanosleep(&tim, &tim2);
	}

	return 0;
}

/**
 * @brief BtSnoopTask::decode_file
 *      decode full snoop file header / packet record data
 * @param fileStream
 *      file
 * @param current_position
 *      current position of file (initial is 0 / cant be -1)
 * @return
 *      new position of file (to match with incoming changes)
 */
int BtSnoopTask::decode_streaming_file(ifstream *fileStream,int current_position) {

	switch(state){

		case FILE_HEADER:
		{
			char* file_header = new char[16];

			fileStream->read(file_header, 16);

			fileInfo=BtSnoopFileInfo(file_header);

			delete[] file_header;

			current_position = fileStream->tellg();

			packet_record_state = 0;

			state=PACKET_RECORD;
			
		}
		case PACKET_RECORD:
		{
			fileStream->seekg (0, fileStream->end);
			int length = fileStream->tellg();
			fileStream->seekg(current_position,ios::beg);

			while ((fileStream->tellg() != -1) && (length != fileStream->tellg())) {

				if ((packet_record_state == 1) || (packet_record_state == 0)){

					current_position = fileStream->tellg();

					if (packet_record_state == 0){

						#ifdef __ANDROID__
						//__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","[1] position : %d\n",fileStream->tellg());
						#else
						//cout << "[1] position : " << fileStream->tellg() << endl;
						#endif

						packet_header = new char[24];
						header_index = 0;

						packet_record_state = 1;
					}

					for (unsigned int i = header_index; i< 24;i++){

						current_position = fileStream->tellg();

						fileStream->seekg (0, fileStream->end);
						length = fileStream->tellg();
						fileStream->seekg(current_position,ios::beg);

						if ((fileStream->tellg() != -1) && (length != fileStream->tellg())){

							packet_header[header_index] = fileStream->get();
							header_index++;

							#ifdef __ANDROID__
							//__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","[2] position : %d\n",fileStream->tellg());
							#else
							//cout << "[2] position : " << fileStream->tellg() << endl;
							#endif
						}
						else{

							if (fileStream->tellg() != -1)
								current_position = fileStream->tellg();

							break;
						}
					}

					//cout << "header_index : " << header_index << " sur 24" << endl;
						
					if (header_index == 24){
						packet_record_state = 2;
					}

					#ifdef __ANDROID__
					//__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","[3] position : %d\n",fileStream->tellg());
					#else
					//cout << "[3] position : " << fileStream->tellg() << endl;
					#endif

				}

				if ((packet_record_state == 2) || (packet_record_state == 3))
				{

					if (packet_record_state == 2){

						#ifdef __ANDROID__
						//__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","[4] position : %d\n",fileStream->tellg());
						#else
						//cout << "[4] position : " << fileStream->tellg() << endl;
						#endif

						packet = BtSnoopPacket(packet_header);
						delete[] packet_header;

						packet_data = new char[packet.getincludedLength()];
						data_index = 0;

						packet_record_state = 3;
					}

					for (unsigned int i = data_index; i< packet.getincludedLength();i++){

						current_position = fileStream->tellg();

						fileStream->seekg (0, fileStream->end);
						length = fileStream->tellg();
						fileStream->seekg(current_position,ios::beg);

						if ((fileStream->tellg() != -1) && (length != fileStream->tellg())){

							packet_data[data_index] = fileStream->get();
							data_index++;

							#ifdef __ANDROID__
							//__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","[5] position : %d\n",fileStream->tellg());
							#else
							//cout << "[5] position : " << fileStream->tellg() << endl;
							#endif
						}
						else{

							if (fileStream->tellg() != -1)
								current_position = fileStream->tellg();
							break;
						}
					}

					if (fileStream->tellg() != -1)
								current_position = fileStream->tellg();

					//cout << "data index : " << data_index << " - packet length : " << packet.getincludedLength() << endl;

					if (data_index == packet.getincludedLength()){

						packet_record_state = 0;

						#ifdef __ANDROID__
						//__android_log_print(ANDROID_LOG_VERBOSE,"snoop decoder","[6] position : %d\n",fileStream->tellg());
						#else
						//cout << "[6] position : " << fileStream->tellg() << endl;
						#endif

						packet.decode_data(packet_data);

						delete[] packet_data;

						for (unsigned int i = 0; i  < snoopListenerList->size();i++){
							snoopListenerList->at(i)->onSnoopPacketReceived(fileInfo,packet);
						}
					}
				}
			}
		}
	}

	return current_position;
}

BtSnoopFileInfo BtSnoopTask::getFileInfo(){
	return fileInfo;
}

std::vector<BtSnoopPacket> BtSnoopTask::getPacketDataRecords(){
	return packetDataRecords;
}

/**
 * @brief BtSnoopTask::decode_file
 *      decode full snoop file header / packet record data
 * @param fileStream
 *      file
 * @param current_position
 *      current position of file (initial is 0 / cant be -1)
 * @return
 *      new position of file (to match with incoming changes)
 */
bool BtSnoopTask::decode_file() {

	packetDataRecords.clear();

	ifstream fileStream(file_path.c_str());

	if (fileStream.is_open()) {

		switch(state){

			case FILE_HEADER:
			{
				char* file_header = new char[16];

				fileStream.read(file_header, 16);

				fileInfo=BtSnoopFileInfo(file_header);

				delete[] file_header;

				if (fileStream.tellg()!=-1) {
					state=PACKET_RECORD;
				}
				else{
					return false;
				}
			}
			case PACKET_RECORD:
			{
				while (fileStream.tellg()!=-1){

					char * packet_header = new char[24];

					fileStream.read(packet_header, 24);

					if (fileStream.tellg()!=-1){

						BtSnoopPacket packet(packet_header);

						char * packet_data = new char[packet.getincludedLength()];

						fileStream.read(packet_data, packet.getincludedLength());

						packet.decode_data(packet_data);

						delete[] packet_data;

						if (snoopListenerList!=0){

							for (unsigned int i = 0; i  < snoopListenerList->size();i++){
								snoopListenerList->at(i)->onSnoopPacketReceived(fileInfo,packet);
							}
						}

						packetDataRecords.push_back(packet);
					}
					delete[] packet_header;
				}
			}
		}

		fileStream.close();
		return true;
	}
	return false;
}

