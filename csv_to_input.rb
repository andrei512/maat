require 'csv'

bad = 0
normal = 0
lines = []

open(ARGV[0]).each do |line|
	csv_line = []
  begin
		normal += 1
		csv_line = CSV.parse(line)

		lines << csv_line 
	rescue ArgumentError
		line.encode!('UTF-16', 'UTF-8', :invalid => :replace, :replace => '?')
		line.encode!('UTF-8', 'UTF-16')

		bad += 1
		csv_line = CSV.parse(line)

		lines << csv_line
	end
	puts csv_line
end

puts "#{normal} normal lines and #{bad} bad lines"