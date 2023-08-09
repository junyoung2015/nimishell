#!/usr/bin/expect

spawn ./minishell

set green "\033\[1;32;40m"
set red "\033\[1;31m"
# set yellow "\033\[1;33m"
set default "\033\[0m"
set count 1

set timeout 5

send "\r"
expect "" ;

##########
########## Testing cd builtin ##########
##########

send "pwd\r"
expect {
	"$env(HOME)/nimishell" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		# exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "cd includes\r"
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n"
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "pwd\r"
expect {
	"$env(HOME)/nimishell/includes" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		# exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "cd -\r"
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "pwd\r"
expect {
	"$env(HOME)/nimishell" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		# exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "cd\r"
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "pwd\r"
expect {
	"$env(HOME)" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		# exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "cd dir_does_not_exist\r"
expect {
	"minishell: cd: dir_does_not_exist: No such file or directory" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "export HOME_BACKUP=\$HOME\r"
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "unset HOME\r"
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

send "cd\r"
expect {
	"minishell: cd: HOME not set" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

##########
########## Executing commands with relative and absolute path ##########
##########

# Test wc
send "echo \"\" | wc\r"
expect {
	"1       0       1"  {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test ls
# send "ls includes\r"
# expect "builtin.h	xecutor.h	lexer.h	minishell.h	parser.	tokenizer.h	" ;

# Test executing command that does not exist 'cmd_nonexist'
send "cmd_nonexist\r"
# expect "minishell: cmd_nonexist: command not found" ;
expect {
	"minishell: cmd_nonexist: command not found" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test executing program that does not exist, in absolute path './program_nonexist'
send "./program_nonexist\r"
# expect "error: command not found: No such file or directory" ;
# expect "minishell: ./program_nonexist: No such file or directory" ;
# expect "minishell: program_nonexist: No such file or directory" ;
expect {
	"minishell: ./program_nonexist: No such file or directory" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

#========== Exit code for CMD_NOT_FOUND ==========#

# Test exit code for when coommand not found
send "echo \$?\r"
# expect "127" ;
expect {
	"127" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n" ;
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

##########
########## Input with wrong syntax ##########
##########

# Test 'ls > '
send "ls >\r"
# expect "minishell: syntax error near unexpected token `>`" ;
expect {
	"minishell: syntax error near unexpected token `>`" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

##########
########## Environment variables substitution ##########
##########

# Test 'echo $USER'
send "echo \$USER\r"
# expect "$env(USER)" ;
expect {
	"$env(USER)" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

#========== Exit code for normal exit ==========#
# Test exit code for normal exit
send "echo \$\?\r"
expect {
	"0" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test 'echo '$USER''
send "echo \'\$USER\'\r"
# expect "\$USER" ;
expect {
	"\$USER" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test 'echo "'$USER'"
send "echo \"\'\$USER\'\"\r"
# expect "'$env(USER)'" ;
expect {
	"$env(USER)" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

##########
########## Test for unset and export, using varaibles after unset and after export ##########
##########

#========== Normal Test ==========#
# Test 'unset USER'
send "unset USER\r"
# expect "" ;
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test 'echo $USER' after unset USER
send "echo \$USER\r"
# expect "\n" ;
expect {
	"\n" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test export
send "export test=ch\r"
# expect "" ;
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test env substitution for export
send "e\$test'o'\r"
# expect "\n" ;
expect {
	"\n" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test env substitution for export
send "e\$test'o' \$USER\r"
# expect "$env(USER)" ;
expect {
	"$env(USER)" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test env substitution for export
send "e\$test\"o\" \$USER\r"
# expect "$env(USER)" ;
expect {
	"$env(USER)" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test export with whitespace
send "export \"wspace=hi test\"\r"
# expect "" ;
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test env substitution for export with whitespace
send "e\$test'o' \$wspace\r"
expect "hi test" ;
expect {
	"hi test" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test executing non-builtin commands after unset PATH
send "unset PATH\r"
# expect "" ;
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test executing non-builtin commands after unset PATH
send "ls\r"
# exepct "minishell: ls: No such file or directory" ;
# expect "minishell: ls: command not found" ;
expect {
	"minishell: ls: command not found" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test executing non-builtin commands after export new PATH
send "export PATH=/bin:/usr/sbin:/usr/local/bin\r"
# expect "" ;
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test executing non-builtin commands after setting new PATH
send "ls includesabc\r"
expect {
	"ls: includesabc: No such file or directory" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		# exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# expect {
# 	"builtin.h\nexecutor.h\nlexer.h\nminishell.h\nparser.h\ntokenizer.h\n" {
# 		send_user "${green}Test passed${default}\n" ;
# 	}
# 	timeout {
# 		send_user "${red}Test ${count} failed${default}\n"
# 	}
# }

# Test executing non-builtin commands after export wrong PATH
send "export PATH=/tmp\r"
# expect "" ;
expect {
	"" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

# Test executing non-builtin commands after setting wrong PATH
send "ls includes\r"
# exepct "minishell: ls: No such file or directory" ;
# expect "minishell: ls: command not found" ;
expect {
	"minishell: ls: command not found" {
		send_user "${green}Test ${count} passed${default}\n" ;
	}
	timeout {
		send_user "${red}Test ${count} failed${default}\n"
		exit 1 ;
	}
}
set count [ expr \$count + 1 ] ;

#========== Error Test ==========#
# send "export A.B=C\r"
# expect "minishell: export: A.B=C: not a valid identifier"
# set count [ expr \$count + 1 ] ;

# send "export A=B B=C C=D\r"
# expect ""
# set count [ expr \$count + 1 ] ;

# send "echo \$A\$B\$C\r"
# expect "BCD"
# set count [ expr \$count + 1 ] ;

# send "export 123=A\r"
# expect "minishell: export: \`123=A': not a valid identifier"
# set count [ expr \$count + 1 ] ;

# Finish
send "exit\r"
expect eof
