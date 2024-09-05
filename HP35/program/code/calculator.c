#include "ANSI.h"
#include "calculator.h"
#include "stack.h"
#include "util.h"

#include <limits.h>
#include <string.h>

#define INPUT_BUFFER_CHARACTER_LIMIT 64
char input_buffer[INPUT_BUFFER_CHARACTER_LIMIT];

bool running = true;

stack_dynamic *stack;

bool read_line(char *str, unsigned size)
{
	unsigned gets = 0;
	while (strchr(str, '\n') == NULL)
	{
		fgets(str, size + 1, stdin);
		gets++;
	}

	bool overflowed = gets > 1;
	if (overflowed)
	{
		strcpy(str, "");
		return false;
	}

	// discarding linebreaks
	str[strcspn(str, "\n\r")] = '\0';
	return true;
}

void display_stack()
{
	printf("Stack: ");
	print_array_of_ints(stack->array, stack->top);

	if (stack->top > 0 && stack->array[stack->top - 1] == 42)
	{
		printf("If only we knew what the question was...\n");
	}
}

void handle_command(char *command)
{
	if (strcmp(command, "") == 0) return;

	if (strcmp(command, "help") == 0)
	{
		printf(
			ANSI_BOLD "LIST OF COMMANDS\n" ANSI_OFF
			"\thelp       - Display helpful information.\n"
			"\texit       - Exit the calculator.\n"
			"\tstack      - Print the stack.\n"
			"\tclear      - Empty the stack.\n"
			"\t<integer>  - Push an integer to the stack. [%i, %i]\n"
			"\t<operator> - Perform an operation. Operators to choose from include: +, -, *, /, %%\n"
			ANSI_BOLD "TIPS\n" ANSI_OFF
			"\tCommands can be chained together. As an example, try running: 4 2 3 * 4 + 4 * + 2 -\n"
		, INT_MIN, INT_MAX);
	}
	else if (strcmp(command, "exit") == 0)
	{
		running = false;
	}
	else if (strcmp(command, "stack") == 0)
	{
		display_stack();
	}
	else if (strcmp(command, "clear") == 0)
	{
		stack_dynamic_clear(stack);
		display_stack();
	}
	else if (strcmp(command, "+") == 0)
	{
		if (stack->top >= 2)
		{
			int b = stack_dynamic_pop(stack).value;
			int a = stack_dynamic_pop(stack).value;
			int result = a + b;
			printf("%i + %i = %i\n", a, b, result);
			stack_dynamic_push(stack, result);
			display_stack();
		}
		else
		{
			printf("Addition failed. Too few arguments.\n");
		}
	}
	else if (strcmp(command, "-") == 0)
	{
		if (stack->top >= 2)
		{
			int b = stack_dynamic_pop(stack).value;
			int a = stack_dynamic_pop(stack).value;
			int result = a - b;
			printf("%i - %i = %i\n", a, b, result);
			stack_dynamic_push(stack, result);
			display_stack();
		}
		else
		{
			printf("Subtraction failed. Too few arguments.\n");
		}
	}
	else if (strcmp(command, "*") == 0)
	{
		if (stack->top >= 2)
		{
			int b = stack_dynamic_pop(stack).value;
			int a = stack_dynamic_pop(stack).value;
			int result = a * b;
			printf("%i * %i = %i\n", a, b, result);
			stack_dynamic_push(stack, result);
			display_stack();
		}
		else
		{
			printf("Multiplication failed. Too few arguments.\n");
		}
	}
	else if (strcmp(command, "/") == 0)
	{
		if (stack->top >= 2)
		{
			int b = stack_dynamic_pop(stack).value;
			int a = stack_dynamic_pop(stack).value;
			if (b == 0)
			{
				printf("The calculator exploded.\n");
				running = false;
			}
			else
			{
				int result = a / b;
				printf("%i / %i = %i\n", a, b, result);
				stack_dynamic_push(stack, result);
				display_stack();
			}
		}
		else
		{
			printf("Division failed. Too few arguments.\n");
		}
	}
	else if (strcmp(command, "%") == 0)
	{
		if (stack->top >= 2)
		{
			int b = stack_dynamic_pop(stack).value;
			int a = stack_dynamic_pop(stack).value;
			if (b == 0)
			{
				printf("The calculator exploded.\n");
				running = false;
			}
			else
			{
				int result = a % b;
				printf("%i %% %i = %i\n", a, b, result);
				stack_dynamic_push(stack, result);
				display_stack();
			}
		}
		else
		{
			printf("Modulos failed. Too few arguments.\n");
		}
	}
	else
	{
		if (str_is_int(command))
		{
			int integer = atoi(command);
			stack_dynamic_push(stack, integer);
			display_stack();
		}
		else
		{
			printf("%s: Command not found.\n", command);
			printf("Type \"help\" if you do not know what to do.\n");
		}
	}
}

void handle_input_buffer()
{
	char *command_buffer = (char *)malloc(INPUT_BUFFER_CHARACTER_LIMIT * sizeof(char));
	unsigned command_buffer_size = 0;

	char *c = input_buffer;
	while (*c != '\0')
	{
		if (*c == ' ')
		{
			command_buffer[command_buffer_size] = '\0';
			handle_command(command_buffer);
			command_buffer_size = 0;
		}
		else
		{
			command_buffer[command_buffer_size++] = *c;
		}
		c++;
	}
	if (command_buffer_size > 0)
	{
		command_buffer[command_buffer_size] = '\0';
		handle_command(command_buffer);
		command_buffer_size = 0;
	}

	free(command_buffer);
}

void calculator()
{
	printf(
		ANSI_BOLD
		"╭─────────────────────────╮\n"
		"│ HP-35 Pocket Calculator │\n"
		"╰─────────────────────────╯\n"
		ANSI_OFF
	);

	stack = stack_dynamic_construct();

	while (running)
	{
		printf("> ");
		if (read_line(input_buffer, INPUT_BUFFER_CHARACTER_LIMIT))
		{
			handle_input_buffer();
		}
		else
		{
			printf("Input overflow.\n");
		}
	}

	stack_dynamic_destruct(stack);
}
