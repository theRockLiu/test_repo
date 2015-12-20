/*
 * telnetserv.cpp
 *
 *  Created on: Jul 11, 2015
 *      Author: rock
 */

#include "telnetserv.h"

namespace qtp
{

#include <stdio.h>
#include <sys/types.h>
#ifdef WIN32
#include <winsock2.h>
#include <windows.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <signal.h>
#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// vim:sw=4 tw=120 et

#define CLITEST_PORT                8000
#define MODE_CONFIG_INT             10

#ifdef __GNUC__
# define UNUSED(d) d __attribute__ ((unused))
#else
# define UNUSED(d) d
#endif

unsigned int regular_count = 0;
unsigned int debug_regular = 0;

struct my_context
{
	int value;
	char* message;
};

#ifdef WIN32
typedef int socklen_t;

int winsock_init()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// Start up sockets
	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		// Tell the user that we could not find a usable WinSock DLL.
		return 0;
	}

	/*
	 * Confirm that the WinSock DLL supports 2.2
	 * Note that if the DLL supports versions greater than 2.2 in addition to
	 * 2.2, it will still return 2.2 in wVersion since that is the version we
	 * requested.
	 * */
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	{
		// Tell the user that we could not find a usable WinSock DLL.
		WSACleanup();
		return 0;
	}
	return 1;
}
#endif

int cmd_test(struct cli_def *cli_, const char *command, char *argv[], int argc)
{
	int i;
	cli_print(cli_, "called %s with \"%s\"", __FUNCTION__, command);
	cli_print(cli_, "%d arguments:", argc);
	for (i = 0; i < argc; i++)
		cli_print(cli_, "        %s", argv[i]);

	return CLI_OK;
}

int cmd_set(struct cli_def *cli_, UNUSED(const char *command), char *argv[],
		int argc)
{
	if (argc < 2 || strcmp(argv[0], "?") == 0)
	{
		cli_print(cli_, "Specify a variable to set");
		return CLI_OK;
	}

	if (strcmp(argv[1], "?") == 0)
	{
		cli_print(cli_, "Specify a value");
		return CLI_OK;
	}

	if (strcmp(argv[0], "regular_interval") == 0)
	{
		unsigned int sec = 0;
		if (!argv[1] && !&argv[1])
		{
			cli_print(cli_, "Specify a regular callback interval in seconds");
			return CLI_OK;
		}
		sscanf(argv[1], "%u", &sec);
		if (sec < 1)
		{
			cli_print(cli_, "Specify a regular callback interval in seconds");
			return CLI_OK;
		}
		cli_->timeout_tm.tv_sec = sec;
		cli_->timeout_tm.tv_usec = 0;
		cli_print(cli_, "Regular callback interval is now %d seconds", sec);
		return CLI_OK;
	}

	cli_print(cli_, "Setting \"%s\" to \"%s\"", argv[0], argv[1]);
	return CLI_OK;
}

int cmd_config_int(struct cli_def *cli_, UNUSED(const char *command),
		char *argv[], int argc)
{
	if (argc < 1)
	{
		cli_print(cli_, "Specify an interface to configure");
		return CLI_OK;
	}

	if (strcmp(argv[0], "?") == 0)
		cli_print(cli_, "  test0/0");

	else if (strcasecmp(argv[0], "test0/0") == 0)
		cli_set_configmode(cli_, MODE_CONFIG_INT, "test");
	else
		cli_print(cli_, "Unknown interface %s", argv[0]);

	return CLI_OK;
}

int cmd_config_int_exit(struct cli_def *cli_, UNUSED(const char *command),
UNUSED(char *argv[]), UNUSED(int argc))
{
	cli_set_configmode(cli_, MODE_CONFIG, NULL);
	return CLI_OK;
}

int cmd_show_regular(struct cli_def *cli_, UNUSED(const char *command),
		char *argv[], int argc)
{
	cli_print(cli_, "cli_regular() has run %u times", regular_count);
	return CLI_OK;
}

int cmd_debug_regular(struct cli_def *cli_, UNUSED(const char *command),
		char *argv[], int argc)
{
	debug_regular = !debug_regular;
	cli_print(cli_, "cli_regular() debugging is %s",
			debug_regular ? "enabled" : "disabled");
	return CLI_OK;
}

int cmd_context(struct cli_def *cli_, UNUSED(const char *command),
UNUSED(char *argv[]), UNUSED(int argc))
{
	struct my_context *myctx = (struct my_context *) cli_get_context(cli_);
	cli_print(cli_, "User context has a value of %d and message saying %s",
			myctx->value, myctx->message);
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

int regular_callback(struct cli_def *cli_)
{
	regular_count++;
	if (debug_regular)
	{
		cli_print(cli_, "Regular callback - %u times so far", regular_count);
		cli_reprompt(cli_);
	}
	return CLI_OK;
}

int check_enable(const char *password)
{
	return !strcasecmp(password, "topsecret");
}

int idle_timeout(struct cli_def *cli_)
{
	cli_print(cli_, "Custom idle timeout");
	return CLI_QUIT;
}

void pc(UNUSED(struct cli_def *cli_), const char *string)
{
	printf("%s\n", string);
}

int_fast32_t telnet_serv::run()
{
	struct cli_command *c;
	int x;
	struct sockaddr_in addr;
	int on = 1;

	// Prepare a small user context
	char mymessage[] = "I contain user data!";
	struct my_context myctx;
	myctx.value = 5;
	myctx.message = mymessage;

	cli_ = cli_init();
	cli_set_banner(cli_, "Smart Quant Platform");
	cli_set_hostname(cli_, "SQP");
	cli_telnet_protocol(cli_, 1);
	cli_regular(cli_, regular_callback);
	cli_regular_interval(cli_, 5); // Defaults to 1 second
	cli_set_idle_timeout_callback(cli_, 60, idle_timeout); // 60 second idle timeout
	cli_register_command(cli_, NULL, "test", cmd_test, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli_, NULL, "simple", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli_, NULL, "simon", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli_, NULL, "set", cmd_set, PRIVILEGE_PRIVILEGED,
	MODE_EXEC, NULL);

	c = cli_register_command(cli_, NULL, "show", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli_, c, "regular", cmd_show_regular,
	PRIVILEGE_UNPRIVILEGED, MODE_EXEC,
			"Show the how many times cli_regular has run");

	cli_register_command(cli_, c, "counters", cmd_test, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, "Show the counters that the system uses");

	cli_register_command(cli_, c, "junk", cmd_test, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli_, NULL, "interface", cmd_config_int,
	PRIVILEGE_PRIVILEGED, MODE_CONFIG, "Configure an interface");

	cli_register_command(cli_, NULL, "exit", cmd_config_int_exit,
	PRIVILEGE_PRIVILEGED, MODE_CONFIG_INT, "Exit from interface configuration");

	cli_register_command(cli_, NULL, "address", cmd_test, PRIVILEGE_PRIVILEGED,
	MODE_CONFIG_INT, "Set IP address");

	c = cli_register_command(cli_, NULL, "debug", NULL, PRIVILEGE_UNPRIVILEGED,
	MODE_EXEC, NULL);

	cli_register_command(cli_, c, "regular", cmd_debug_regular,
	PRIVILEGE_UNPRIVILEGED, MODE_EXEC,
			"Enable cli_regular() callback debugging");

	// Set user context and its command
	cli_set_context(cli_, (void*) &myctx);
	cli_register_command(cli_, NULL, "context", cmd_context,
	PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Test a user-specified context");

	cli_set_auth_callback(cli_, check_auth);
	cli_set_enable_callback(cli_, check_enable);
	// Test reading from a file
	{
		FILE *fh;

		if ((fh = fopen("clitest.txt", "r")))
		{
			// This sets a callback which just displays the cli_print() text to stdout
			cli_print_callback(cli_, pc);
			cli_file(cli_, fh, PRIVILEGE_UNPRIVILEGED, MODE_EXEC);
			cli_print_callback(cli_, NULL);
			fclose(fh);
		}
	}

	cli_loop(cli_, x);
	//
	cli_done(cli_);
}

telnet_serv::telnet_serv()
{
	// TODO Auto-generated constructor stub

}

telnet_serv::~telnet_serv()
{
	// TODO Auto-generated destructor stub
}

} /* namespace qtp */
