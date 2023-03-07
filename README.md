# MINISHELL

###### Final grade : 99/100

## Description
Recoding personal bash-like shell in C


## Run it

#### Clone the project and navigate to the folder
```shell
> git clone https://github.com/Jerboulade/09_MINISHELL.git minishell
> cd minishell
```
#### Compile the code and run the executable
```shell
> make
> ./minishell
```
#### Note
Minishell uses external library called `readline`.<br>
In order the make the code compile, `readline` has to be installed on your computer. Also the `RLINCL` and `RLLIB` variable from the `Makefile` must be updated according to your own `readline` folder path ( lib and include ).


## Features
• Displays a prompt when waiting for a new command.

• Has a working history.

• Search and launch the right executable ( based on the PATH variable or using a relative or an absolute path ).

• Handles `’` (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.

• Handles ``"`` (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for ``$`` (dollar sign).

• Handles redirections : `<`, `>`, `<<`, `>>`

`<` redirects input

`>` redirects output

`<< [delimiter]` reads the input until a line containing the delimiter is seen

`>>` redirects output in append mode.

• Implements pipes `|`. The output of each command in the pipeline is
connected to the input of the next command via a pipe.

• Handles environment variables `$` followed by a sequence of characters, which should expand to their values.

• Handles `$?` which should expand to the exit status of the most recently execute foreground pipeline.

• Handles `ctrl-C`, `ctrl-D` and `ctrl-\` signals which should behave like in bash.

`ctrl-C` displays a new prompt on a new line.

`ctrl-D` exits the shell.

`ctrl-\` does nothing.

• Implements the following builtins:

`echo` with option -n

`cd` with only a relative or absolute path

`pwd` with no options

`export` with no options

`unset` with no options

`env` with no options or arguments

`exit` with no options
