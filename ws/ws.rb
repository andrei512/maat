# create a log file and get references to stdin, stdout and stderr
# create a fork for the C++ code and serve it through a simple rack app

child_stdout_and_stderr = File.new("log/log.txt","a+")

@@r, @@w = IO.pipe

child = fork { 
	$stdin.reopen(@@r)
	$stdout.reopen(child_stdout_and_stderr)
	$stderr.reopen(child_stdout_and_stderr)
	exec "./run.sh loc.in"
}

# a little bit of sys magik from @alex :)
def query input	
	input.strip!
	if input.length > 0
		@@w.puts input
		response = `cat fifo_pipe`	
		response.split('***')
	else
		return []
	end
end

require 'rack'
require 'json'

class AutocompleteApp
  def call env
	request = Rack::Request.new env
	puts "ENV = #{JSON.pretty_generate(env)}\n\n"
	puts request.params
  	input = request.params['query'] || ""

  	
    [200, {"Content-Type" => "application/json"}, 
    	[
    		JSON.pretty_generate(query(input))
    	]
    ] 
  end
end



