transport_site:main.cpp XUdp.o UdpThread.o XTcp.o TcpThread.o global.o
	arm-linux-gnueabi-g++ -o transport_site main.cpp XUdp.o UdpThread.o XTcp.o TcpThread.o global.o -std=c++11 -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	#g++ -o transport_site main.cpp XUdp.o UdpThread.o XTcp.o TcpThread.o global.o -std=c++11 -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
XUdp.o:XUdp.cpp XUdp.h
	arm-linux-gnueabi-g++ -c XUdp.cpp -std=c++11 -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	#g++ -c XUdp.cpp -std=c++11 -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
UdpThread.o:UdpThread.cpp UdpThread.h
	arm-linux-gnueabi-g++ -c UdpThread.cpp -std=c++11       -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	#g++ -c UdpThread.cpp -std=c++11       -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
XTcp.o:XTcp.cpp XTcp.h
	arm-linux-gnueabi-g++ -c XTcp.cpp -std=c++11   -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	#g++ -c XTcp.cpp -std=c++11   -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
TcpThread.o:TcpThread.cpp TcpThread.h
	arm-linux-gnueabi-g++ -c TcpThread.cpp -std=c++11 -pthread   -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	#g++ -c TcpThread.cpp -std=c++11 -pthread   -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
global.o:global.cpp global.h
	arm-linux-gnueabi-g++ -c global.cpp -std=c++11 -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive
	#g++ -c global.cpp -std=c++11 -pthread  -static -Wl,--whole-archive -lpthread -Wl,--no-whole-archive


