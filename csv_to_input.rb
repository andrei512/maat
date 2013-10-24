require 'csv'

to_convert = open(ARGV[0]).read.encode!('UTF-16', 'UTF-8', :invalid => :replace, :replace => '').encode!('UTF-8', 'UTF-16')


idx = 0

lines = CSV.parse(to_convert)

File.open(ARGV[1], "w") do |file|  
	lines.each do |row|
		unless idx == 0			
			file.write(row.join("\n") + "\n")			
		else 			
			file.write("#{lines.count - 1}\n")			
		end		
		idx += 1 
	end
end


# bad = 0
# normal = 0
# lines = []

# open(ARGV[0]).each do |line|
# 	csv_line = []
#   begin
# 		normal += 1
# 		csv_line = CSV.parse(line)

# 		lines << csv_line 
# 	rescue ArgumentError
# 		line.encode!('UTF-16', 'UTF-8', :invalid => :replace, :replace => '?')
# 		line.encode!('UTF-8', 'UTF-16')

# 		bad += 1
# 		csv_line = CSV.parse(line)

# 		lines << csv_line
# 	end
# end

# # puts "#{normal} normal lines and #{bad} bad lines"

# File.open(ARGV[1], "w") do |file|  
# 	lines.each do |line|
# 		file.write(line)
# 	end
# end