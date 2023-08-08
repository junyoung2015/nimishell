#!/usr/bin/expect

spawn ./minishell

set green "\033\[1;32;40m"
set red "\033\[1;31m"
# set yellow "\033\[1;33m"
set default "\033\[0m"

set timeout 5

##### Executing commands with relative and absolute path #####

# Test wc
send "echo \"\" | wc\r"
expect {
    "1       0       1"  { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" ; }
}

# Test ls
# send "ls includes\r"
# expect "builtin.h	xecutor.h	lexer.h	minishell.h	parser.	tokenizer.h	" ;

# Test executing command that does not exist 'cmd_nonexist'
send "cmd_nonexist\r"
# expect "minishell: cmd_nonexist: command not found" ;
expect {
    "minishell: cmd_nonexist: command not found" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" ; }
}

# Test executing program that does not exist, in absolute path './program_nonexist'
send "./program_nonexist\r"
# expect "error: command not found: No such file or directory" ;
# expect "minishell: program_nonexist: No such file or directory" ;
expect {
    "error: command not found: No such file or directory" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" ; }
}

#======#### Exit code for CMD_NOT_FOUND #####======

# Test exit code for when coommand not found
send "echo \$?\r"
# expect "127" ;
expect {
    "127" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" ; }
}

##### Input with wrong syntax #####

# Test 'ls > '
send "ls >\r"
# expect "minishell: syntax error near unexpected token `>`" ;
expect {
    "minishell: syntax error near unexpected token `>`" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

###### Environment variables substitution #####

# Test 'echo $USER'
send "echo \$USER\r"
# expect "$env(USER)" ;
expect {
    "$env(USER)" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

#======#### Exit code for normal exit #####======
# Test exit code for normal exit
send "echo \$\?\r"
# expect "0" ;
expect {
    "0" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test 'echo '$USER''
send "echo \'\$USER\'\r"
# expect "\$USER" ;
expect {
    "\$USER" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test 'echo "'$USER'"
send "echo \"\'\$USER\'\"\r"
# expect "'$env(USER)'" ;
expect {
    "$env(USER)" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

##### Test for unset and export, using varaibles after unset and after export #####

# Test 'unset USER'
send "unset USER\r"
# expect "" ;
expect {
    "" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test 'echo $USER' after unset USER
send "echo \$USER\r"
# expect "\n" ;
expect {
    "\n" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test export
send "export test=ch\r"
# expect "" ;
expect {
    "" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test env substitution for export
send "e\$test'o'\r"
# expect "\n" ;
expect {
    "\n" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test env substitution for export
send "e\$test'o' \$USER\r"
# expect "$env(USER)" ;
expect {
    "$env(USER)" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test env substitution for export
send "e\$test\"o\" \$USER\r"
# expect "$env(USER)" ;
expect {
    "$env(USER)" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test export with whitespace
send "export \"wspace=hi test\"\r"
# expect "" ;
expect {
    "" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test env substitution for export with whitespace
send "e\$test'o' \$wspace\r"
expect "hi test" ;
expect {
    "hi test" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test executing non-builtin commands after unset PATH
send "unset PATH\r"
# expect "" ;
expect {
    "" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test executing non-builtin commands after unset PATH
send "ls\r"
# exepct "minishell: ls: No such file or directory" ;
# expect "minishell: ls: command not found" ;
expect {
    "minishell: ls: command not found" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test executing non-builtin commands after export new PATH
send "export PATH=/bin:/usr/sbin:/usr/local/bin\r"
# expect "" ;
expect {
    "" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test executing non-builtin commands after setting new PATH
send "ls includes\r"
# expect "builtin.h   executor.h  lexer.h     minishell.h parser.h    tokenizer.h" ;
expect {
    "builtin.h   executor.h  lexer.h     minishell.h parser.h    tokenizer.h" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test executing non-builtin commands after export wrong PATH
send "export PATH=/tmp\r"
# expect "" ;
expect {
    "" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Test executing non-builtin commands after setting wrong PATH
send "ls includes\r"
# exepct "minishell: ls: No such file or directory" ;
# expect "minishell: ls: command not found" ;
expect {
    "minishell: ls: command not found" { send_user "${green}Test passed${default}\n" ; }
    timeout { send_user "${red}Test failed${default}\n" }
}

# Finish
send "exit\r"
expect eof
