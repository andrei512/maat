if [ -f maat ]; then
	rm maat
fi

g++ -O2 -Wall -o maat maat.cpp
