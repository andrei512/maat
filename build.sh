if [ -f maat ]; then
	rm maat
fi

ruby scripts/char_map.rb > char_map.h
ruby scripts/gen_keyboard_dist.rb > char_dist.h

g++ -O2 -Wall -o maat maat.cpp
