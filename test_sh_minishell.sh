#!/usr/bin/expect

spawn ./minishell

# set green "\033\[0;32;40m"
# set red "\033\[1;31m"
# set default "\033\[0m"

set timeout 5

# Test 'cmd_nonexist'
send "cmd_nonexist\r"
expect "minishell: cmd_nonexist: command not found" ;
# expect {
#     "minishell: cmd_nonexist: command not found" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test exit code for when coommand not found
send "echo \$?\r"
expect "127" ;

# Test './program_nonexist'
send "./program_nonexist\r"
expect "minishell: program_nonexist: No such file or directory" ;

# Test 'ls > '
send "ls >\r"
expect "minishell: syntax error near unexpected token `>`" ;
# expect {
#     "minishell: syntax error near unexpected token `>`" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test 'echo $USER'
send "echo \$USER\r"
expect "$env(USER)" ;
# expect {
#     "$env(USER)" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test exit code for normal exit
send "echo \$\?\r"
expect "0" ;

# Test 'echo '$USER''
send "echo \'\$USER\'\r"
expect "\$USER" ;
# expect {
#     "\$USER" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test 'echo "'$USER'"
send "echo \"\'\$USER\'\"\r"
expect "'$env(USER)'" ;
# expect {
#     "$env(USER)" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test 'unset USER'
send "unset USER\r"
expect "" ;

# Test 'echo $USER' after unset USER
send "echo \$USER\r"
expect "\n" ;

# Test export
send "export test=ch\r"
expect "" ;

# Test env substitution for export
send "e\$test'o' \$USER\r"
expect "$env(USER)" ;

# Test env substitution for export
send "e\$test\"o\" \$USER\r"
expect "$env(USER)" ;

# Test export with whitespace
send "export \"wspace=hi test\"\r"
expect "" ;

# Test env substitution for export with whitespace
send "e\$test'o' \$wspace\r"
expect "hi test" ;

# Finish
send "exit\r"
expect eof
