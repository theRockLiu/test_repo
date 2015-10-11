/*
 * engine.cpp
 *
 *  Created on: Sep 16, 2015
 *      Author: rock
 */

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include <libcli.h>

#include "shared.h"

#ifdef __GNUC__
# define UNUSED(d) d __attribute__ ((unused))
#else
# define UNUSED(d) d
#endif

#define CLI_PORT                8000

namespace
{

int32_t regular_callback(struct cli_def *cli)
{

	SHARED_OBJ().exec();
//	regular_count++;
//	if (debug_regular)
//	{
//		cli_print(cli, "Regular callback - %u times so far", regular_count);
//		cli_reprompt(cli);
//	}
	return CLI_OK;
}

int32_t idle_timeout(struct cli_def *cli)
{
	cli_print(cli, "Custom idle timeout");
	return CLI_QUIT;
}

int cmd_test(struct cli_def *cli, const char *command, char *argv[], int argc)
{
    int i;
    //cli_print(cli, "called %s with \"%s\"", __FUNCTION__, command);
    cli_print(cli, "%d arguments:", argc);
    for (i = 0; i < argc; i++)
        cli_print(cli, "        %s", argv[i]);

    return CLI_OK;
}

int cmd_set(struct cli_def *cli, UNUSED(const char *command), char *argv[],
    int argc)
{
    if (argc < 2 || strcmp(argv[0], "?") == 0)
    {
        cli_print(cli, "Specify a variable to set");
        return CLI_OK;
    }

    if (strcmp(argv[1], "?") == 0)
    {
        cli_print(cli, "Specify a value");
        return CLI_OK;
    }

    if (strcmp(argv[0], "regular_interval") == 0)
    {
        unsigned int sec = 0;
        if (!argv[1] && !&argv[1])
        {
            cli_print(cli, "Specify a regular callback interval in seconds");
            return CLI_OK;
        }
        sscanf(argv[1], "%u", &sec);
        if (sec < 1)
        {
            cli_print(cli, "Specify a regular callback interval in seconds");
            return CLI_OK;
        }
        cli->timeout_tm.tv_sec = sec;
        cli->timeout_tm.tv_usec = 0;
        cli_print(cli, "Regular callback interval is now %d seconds", sec);
        return CLI_OK;
    }

    cli_print(cli, "Setting \"%s\" to \"%s\"", argv[0], argv[1]);
    return CLI_OK;
}

int check_auth(const char *username, const char *password)
{
    if (strcasecmp(username, "fred") != 0)
        return CLI_ERROR;
    if (strcasecmp(password, "nerk") != 0)
        return CLI_ERROR;
    return CLI_OK;
}

int check_enable(const char *password)
{
    return !strcasecmp(password, "topsecret");
}

void pc(UNUSED(struct cli_def *cli), const char *string)
{
    printf("%s\n", string);
}

}

using namespace atp;


engine::engine()
{
	// TODO Auto-generated constructor stub

}

engine::~engine()
{
	// TODO Auto-generated destructor stub
}

int_fast32_t engine::run(const string_t& cf)
{
	if (SHARED_OBJ().init(cf) < 0)
	{
		return -1;
	}



	struct cli_command *c;
	struct cli_def *cli;
	int s, x;
	struct sockaddr_in addr;
	int on = 1;

	// Prepare a small user context
	//char mymessage[] = "I contain user data!";
	//struct my_context myctx;
	//myctx.value = 5;
	//myctx.message = mymessage;

	cli = cli_init();
	cli_set_banner(cli, "cli environment");
	cli_set_hostname(cli, "router");
	cli_telnet_protocol(cli, 1);
	cli_regular(cli, regular_callback);
	cli_regular_interval(cli, 5); // Defaults to 1 second
	cli_set_idle_timeout_callback(cli, 60, idle_timeout); // 60 second idle timeout
	cli_register_command(cli, NULL, "test", cmd_test, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli, NULL, "simple", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli, NULL, "simon", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli, NULL, "set", cmd_set, PRIVILEGE_PRIVILEGED,
	MODE_EXEC, NULL);

	c = cli_register_command(cli, NULL, "show", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

//	cli_register_command(cli, c, "regular", cmd_show_regular,
//	PRIVILEGE_UNPRIVILEGED, MODE_EXEC,
//			"Show the how many times cli_regular has run");

	cli_register_command(cli, c, "counters", cmd_test, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, "Show the counters that the system uses");

	cli_register_command(cli, c, "junk", cmd_test, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

//	cli_register_command(cli, NULL, "interface", cmd_config_int,
//	PRIVILEGE_PRIVILEGED, MODE_CONFIG, "Configure an interface");
//
//	cli_register_command(cli, NULL, "exit", cmd_config_int_exit,
//	PRIVILEGE_PRIVILEGED, MODE_CONFIG_INT, "Exit from interface configuration");
//
//	cli_register_command(cli, NULL, "address", cmd_test, PRIVILEGE_PRIVILEGED,
//			MODE_CONFIG_INT, "Set IP address");

//	c = cli_register_command(cli, NULL, "debug", NULL, PRIVILEGE_UNPRIVILEGED,
//	MODE_EXEC, NULL);

//	cli_register_command(cli, c, "regular", cmd_debug_regular,
//	PRIVILEGE_UNPRIVILEGED, MODE_EXEC,
//			"Enable cli_regular() callback debugging");

	// Set user context and its command
	//cli_set_context(cli, (void*) &myctx);
	//cli_register_command(cli, NULL, "context", cmd_context, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Test a user-specified context");

	cli_set_auth_callback(cli, check_auth);
	cli_set_enable_callback(cli, check_enable);
	// Test reading from a file
	{
		FILE *fh;

		if ((fh = fopen("clitest.txt", "r")))
		{
			// This sets a callback which just displays the cli_print() text to stdout
			cli_print_callback(cli, pc);
			cli_file(cli, fh, PRIVILEGE_UNPRIVILEGED, MODE_EXEC);
			cli_print_callback(cli, NULL);
			fclose(fh);
		}
	}

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return 1;
	}
	setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(CLI_PORT);
	if (bind(s, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return 1;
	}

	if (listen(s, 50) < 0)
	{
		perror("listen");
		return 1;
	}

	printf("Listening on port %d\n", CLI_PORT);
	while ((x = accept(s, NULL, 0)))
	{
		cli_loop(cli, x);
		close(x);
	}

	cli_done(cli);

	return RET_SUC;
}

//} /* namespace qtp_fw */
