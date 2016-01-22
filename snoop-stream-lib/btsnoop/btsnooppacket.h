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
	btsnooppacket.h

	Parse bt snoop packet record

	@author Bertrand Martel
	@version 0.1
*/

#ifndef BTSNOOPPACKET_H
#define BTSNOOPPACKET_H

#include "vector"
#include <inttypes.h>

class BtSnoopPacket
{

public:

	BtSnoopPacket();
	
	/**
	 * @brief BtSnoopPacket
	 *      build snoop packet
	 * @param data
	 *      data of size 24 (4 + 4 + 4 + 4 + 8)
	 */
	BtSnoopPacket(char * data);

	~BtSnoopPacket();

	/**
	 * @brief decode_data
	 *      decode packet data field
	 * @param data
	 */
	void decode_data(char * data);

	/**
	 * @brief getOriginalLength
	 *      get length of original packet (could be more than this packet's length)
	 * @return
	 */
	int getOriginalLength();

	/**
	 * @brief getincludedLength
	 *      get packet data field length
	 * @return
	 */
	int getincludedLength();

	/**
	 * @brief getCumulativeDrops
	 *      get number of packet lost between the first record and this record for this file
	 * @return
	 */
	int getCumulativeDrops();

	/**
	 * @brief getUnixTimestampMicroseconds
	 *      get unix timestamp for this packet record
	 * @return
	 */
	uint64_t getUnixTimestampMicroseconds();

	/**
	 * @brief is_packet_sent
	 *      define if packet record is sent
	 * @return
	 */
	bool is_packet_sent();

	/**
	 * @brief is_packet_received
	 *      define if packet record is received
	 * @return
	 */
	bool is_packet_received();

	/**
	 * @brief is_data
	 *      define if packet record is data record
	 * @return
	 */
	bool is_data();

	/**
	 * @brief is_command_event
	 *      define if packet record is command or event
	 * @return
	 */
	bool is_command_event();

	/**
	* @brief getPacketData
	*      retrieve packet data
	* @return
	*/
	std::vector<char> getPacketData();

	/**
	 * @brief printInfo
	 *      print info in debug mode
	 */
	void printInfo();

private:

	/**
	 * @brief original_length
	 *      length of original packet (could be more than this packet's length)
	 */
	int original_length;

	/**
	 * @brief included_length
	 *      packet data field length
	 */
	int included_length;

	/**
	 * @brief cumulative_drops
	 *      number of packet lost between the first record and this record for this file
	 */
	int cumulative_drops;

	/**
	 * @brief timestamp_seconds
	 *      unix timestamp for this packet record
	 */
	uint64_t timestamp_microseconds;

	/**
	 * @brief packet_data
	 *      packet data
	 */
	std::vector<char> packet_data;

	/**
	 * @brief packet_sent
	 *      define if packet record is sent
	 * @return
	 */
	bool packet_sent;

	/**
	 * @brief packet_received
	 *      define if packet record is received
	 * @return
	 */
	bool packet_received;

	/**
	 * @brief packet_type_data
	 *      define if packet record is data record
	 * @return
	 */
	bool packet_type_data;

	/**
	 * @brief packet_type_command_event
	 *      define if packet record is command or event
	 * @return
	 */
	bool packet_type_command_event;

};

#endif // BTSNOOPPACKET_H
