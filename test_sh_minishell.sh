#!/usr/bin/expect

spawn ./minishell

# set green "\033\[0;32;40m"
# set red "\033\[1;31m"
# set default "\033\[0m"

set timeout 5

##### Executing commands with relative and absolute path #####

# Test ls
send "ls includes\r"
expect "builtin.h   executor.h  lexer.h     minishell.h parser.h    tokenizer.h" ;

# Test executing command that does not exist 'cmd_nonexist'
send "cmd_nonexist\r"
expect "minishell: cmd_nonexist: command not found" ;
# expect {
#     "minishell: cmd_nonexist: command not found" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

# Test executing program that does not exist, in absolute path './program_nonexist'
send "./program_nonexist\r"
expect "error: command not found: No such file or directory" ;
# expect "minishell: program_nonexist: No such file or directory" ;

#======#### Exit code for CMD_NOT_FOUND #####======

# Test exit code for when coommand not found
send "echo \$?\r"
expect "127" ;

##### Input with wrong syntax #####

# Test 'ls > '
send "ls >\r"
expect "minishell: syntax error near unexpected token `>`" ;
# expect {
#     "minishell: syntax error near unexpected token `>`" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

###### Environment variables substitution #####

# Test 'echo $USER'
send "echo \$USER\r"
expect "$env(USER)" ;
# expect {
#     "$env(USER)" { send_user "${green}Test passed${default}\n" ; sleep 1 }
#     timeout { send_user "${red}Test failed${default}" }
# }

#======#### Exit code for normal exit #####======
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

##### Test for unset and export, using varaibles after unset and after export #####

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
send "e\$test'o'\r"
expect "\n" ;

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

# Test executing non-builtin commands after unset PATH
# send "unset PATH\r"
# expect "" ;

# Test executing non-builtin commands after unset PATH
# send "ls\r"
# exepct "minishell: ls: No such file or directory" ;

# Test executing non-builtin commands after export new PATH
# send "export PATH=/bin:/usr/sbin:/usr/local/bin\r"
# expect "" ;

# Test executing non-builtin commands after setting new PATH
# send "ls includes\r"
# expect "builtin.h   executor.h  lexer.h     minishell.h parser.h    tokenizer.h" ;

# Test executing non-builtin commands after export wrong PATH
# send "export PATH=/tmp\r"
# expect "" ;

# Test executing non-builtin commands after setting wrong PATH
# send "ls includes\r"
# exepct "minishell: ls: No such file or directory" ;

# Finish
send "exit\r"
expect eof
