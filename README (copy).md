minishell> echo $PATH
… # (видно нормальный PATH)
minishell> ls
# работает

minishell> unset PATH
minishell> ls
minishell: ls: command not found
minishell> /bin/ls
# работает

minishell> export PATH=/home/asalniko/path_test/dir1:/home/asalniko/path_test/dir2
minishell> whichdir
dir1
minishell> export PATH=/home/asalniko/path_test/dir2:/home/asalniko/path_test/dir1
minishell> whichdir
dir2

