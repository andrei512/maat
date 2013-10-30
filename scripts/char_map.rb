# map a-z and A-Z to 0..25 and space to 26

map = Array.new(256) { -1 }

def next_free map
	m = map.uniq
	for i in 26..255 do
		unless m.include? i 
		 	return i
		end 
	end	
end

def add_character c, map
	map[c.ord] = next_free map
end

# letters
for c in 'a'..'z' do 
	map[c.ord] = c.ord - 'a'.ord
end

for c in 'A'..'Z' do 
	map[c.ord] = c.ord - 'A'.ord
end

# numbers
for i in '0'..'9' do
	add_character i, map
end

# special characters
add_character ' ', map
add_character '-', map
add_character '.', map
add_character ',', map
add_character '&', map
add_character '/', map
add_character '(', map
add_character ')', map
add_character '#', map
add_character '\'', map
add_character '\\', map

# deltition marker 
DELETITION_MARKER = "@"
add_character DELETITION_MARKER, map

puts "const char DELETITION_MARKER = '#{DELETITION_MARKER}';"
puts "const int SIGMA = #{map.uniq.count - 1};"
puts "int char_map[256] = {#{map.join(", ")}};"
