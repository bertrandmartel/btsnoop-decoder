# C++ Bluetooth Snoop File streaming decoder #

[![Build Status](https://travis-ci.org/akinaru/btsnoop-decoder.svg?branch=master)](https://travis-ci.org/akinaru/btsnoop-decoder)

Small library to decode Bluetooth Snoop file used to store radio packet records.

* streaming enabled : incoming packet data can be decoded over the fly for the same snoop file
* non-blocking or blocking process (thread task running) 

Note : this library doesnt decode HCI Bluetooth data, only snoop-like format

<hr/>

##Build

```
cd snoop-stream-lib
make clean
make
```

Library release is under `build-btsnoop-X.X` directory.


##Decode non-dynamic Bt snoop file

To decode one single bt snoop file with no streaming support, use BtSnoopTask with method :

``bool BtSnoopTask::decode_file() ``

Exemple :

```
#include "btsnoop/btsnooptask.h"

..........
..........


BtSnoopTask decoder("/path/to/your/file");

bool success = decoder.decode_file();

if (success){
	//success
}
else{
	//failure (bad reading / file not found)
}
```

##Decode dynamic Bt snoop file

* To decode in streaming mode a bt snoop file, use ``BtSnoopParser`` :

```
#include "btsnoop/btsnoopparser.h"

..........
..........

BtSnoopParser parser;
```

* Add a listner to monitor incoming packet data record with ``void BtSnoopParser::addSnoopListener(IBtSnoopListener* listener)``:

```
BtSnoopMonitor monitor;

parser.addSnoopListener(&monitor);
```

* `BtSnoopMonitor` is a class inheriting IBtSnoopListener interface

```
class BtSnoopMonitor : public IBtSnoopListener
{

public:

	BtSnoopMonitor();

	~BtSnoopMonitor();

	/**
	 * @brief onSnoopPacketReceived
	 *      called when a new packet record has been received
	 * @param fileInfo
	 *      file info object
	 * @param packet
	 *      snoop packet record object
	 */
	void onSnoopPacketReceived(BtSnoopFileInfo fileInfo,BtSnoopPacket packet);

};
```

* launch non blocking decoding task with :

``bool BtSnoopParser::decode_streaming_file(std::string file_path)``

Exemple : 

```
#include "btsnoop/btsnoopparser.h"

..........
..........

BtSnoopParser parser;

BtSnoopMonitor monitor;

parser.addSnoopListener(&monitor);

bool success = parser.decode_streaming_file("/path/to/your/file");

if (success){
	//success
}
else{
	//failure (bad reading / file not found)
}
```

* You can block file monitoring process with ``void BtSnoopParser::join();`` method

##Datamodel description


* ``BtSnoopTask`` description :

| method     | type        |  description
|--------------|---------|-----|------------------------|
| ``getFileInfo()`` | ``BtSnoopFileInfo`` |  retrieve file information      |
| ``getPacketDataRecords()`` | ``std::vector<BtSnoopPacket>`` |  retrieve list of packet record      |

* ``BtSnoopFileInfo`` description :

| method     | type        |  description
|--------------|---------|-----|------------------------|
| ``getIdentificationNumber()`` | ``std::string`` |  get identification number      |
| ``getVersionNumber()`` | ``int`` |  get snoop version number      |
| ``getDatalinkNumber()`` | ``datalink_type`` |  get datalink enum      |
| ``getDatalinkStr()`` | ``std::string`` |  get datalink name string      |

* ``BtSnoopPacket`` description :

| method     | type        |  description
|--------------|---------|-----|------------------------|
| ``getOriginalLength()`` | ``int`` |  get length of original packet      |
| ``getincludedLength()`` | ``int`` |  get packet data field length     |
| ``getCumulativeDrops()`` | ``int`` |  get number of packet lost between the first record and this record for this file     |
| ``getUnixTimestampMicroseconds()`` | ``uint64_t`` |  get unix timestamp for this packet record     |
| ``is_packet_sent()`` | ``bool`` |  define if packet record is sent    |
| ``is_packet_received()`` | ``bool`` |  define if packet record is received      |
| ``is_data()`` | ``bool`` |  define if packet record is data record      |
| ``is_command_event()`` | ``bool`` |  define if packet record is command or event    |
| ``getPacketData()`` | ``std::vector<char>`` |  get packet data records    |

##Android integration

An Android Makefile is provided for easy Android integration. Simply add the git repository as a submodule in your `jni` directory :

```
git submodule add https://github.com/akinaru/btsnoop-decoder.git <yourproject/src/main/jni/btsnoop-decoder>
```

In you `Application.mk` add gnustl support :

```
APP_STL := gnustl_static 
```

If you dont use Android NDK rc10 change `Android.mk` std include with your own path in you android-ndk source :

```
LOCAL_C_INCLUDES := $NDK/sources/cxx-stl/gnu-libstdc++/4.8/include
```

##Examples

[From test project in main.cpp](https://github.com/akinaru/btsnoop-decoder/blob/master/snoop-stream-test/main.cpp)


##Memory checking

``
valgrind --tool=memcheck --leak-check=full ./snoop-test
``

##Specifications

snoop format V2 : https://tools.ietf.org/html/rfc1761
