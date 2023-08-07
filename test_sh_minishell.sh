#!/usr/bin/expect

spawn ./minishell

set green "\033\[0;32;40m"
set red "\033\[1;31m"
set default "\033\[0m"

set timeout 5

# Test 'cmd_nonexist'
send "cmd_nonexist\r"
expect "minishell: cmd_nonexist: command not found" ;
# expect {
#     "minishell: cmd_nonexist: command not found" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test './program_nonexist'
#send "./program_nonexist\r"
#expect "minishell: program_nonexist: No such file or directory" ;

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

# Finish
send "exit\r"
expect eof
