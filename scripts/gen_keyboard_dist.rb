
@qwerty_keyboard = [
	"qwertyuiop".split(''),
	"asdfghjkl".split(''), 
	"zxcvbnm".split(''), 
]

def idx_for letter
	for i in 0..2 do 
		for j in 0..@qwerty_keyboard[i].count-1
			if @qwerty_keyboard[i][j] == letter
				return i, j
			end
		end
	end
end

dist = []
for a_letter in 'a'..'z' do
	current_dist = []
	for b_letter in 'a'..'z' do
		ia, ja = idx_for a_letter
		ib, jb = idx_for b_letter

		d = "#{((ia - ib) ** 2 + (ja - jb) ** 2)}"
		while d.length < 2
			d = " " + d
		end

		current_dist << d
	end
	dist << current_dist
end

puts <<-EOF
int char_dist[26][26] = {
#{dist.map { |letter_dist|
		"\t{ " + letter_dist.join(", ") + " },\n"
}.join}};

inline int cost_for(char a, char b) {
	if (a < 'a' || a > 'z') {
		return 0;
	}

	if (b < 'a' || b > 'z') {
		return 0;
	}
	
	a -= 'a';
	b -= 'a';

	return char_dist[(int)a][(int)b];
}
EOF



