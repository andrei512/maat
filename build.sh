if [ -f maat ]; then
	rm maat
fi

gcc -O2 -Wall -o maat maat.cpp
