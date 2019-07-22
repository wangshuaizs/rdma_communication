#include "common.h"

struct config_t client_config = {
	NULL,  /* dev_name */
	NULL,  /* server_name */
	19875, /* tcp_port */
	1,	 /* ib_port */
	-1, /* gid_idx */
	0   /* service level */
};

/*****************************************************************************
* Function: main
*
* Input
* argc number of items in argv
* argv command line parameters
*
* Output
* none
*
* Returns
* 0 on success, 1 on failure
*
* Description
* Main program code
******************************************************************************/
int main(int argc, char *argv[])
{
	struct resources res;
	int rc = 1;
	/* parse the command line parameters */
	while (1)
	{
		int c;
		static struct option long_options[] = {
			{.name = "port", .has_arg = 1, .val = 'p'},
			{.name = "ib-dev", .has_arg = 1, .val = 'd'},
			{.name = "ib-port", .has_arg = 1, .val = 'i'},
			{.name = "gid-idx", .has_arg = 1, .val = 'g'},
			{.name = "service-level", .has_arg = 1, .val = 's'},
			{.name = "help", .has_arg = 0, .val = 'h'},
			{.name = NULL, .has_arg = 0, .val = '\0'}
        };
		c = getopt_long(argc, argv, "p:d:i:g:s:h:", long_options, NULL);
		if (c == -1)
			break;
		switch (c)
		{
		case 'p':
			client_config.tcp_port = strtoul(optarg, NULL, 0);
			break;
		case 'd':
			client_config.dev_name = strdup(optarg);
			break;
		case 'i':
			client_config.ib_port = strtoul(optarg, NULL, 0);
			if (client_config.ib_port < 0)
			{
				usage(argv[0]);
				return 1;
			}
			break;
		case 'g':
			client_config.gid_idx = strtoul(optarg, NULL, 0);
			if (client_config.gid_idx < 0)
			{
				usage(argv[0]);
				return 1;
			}
			break;
		case 's':
			client_config.sl = strtoul(optarg, NULL, 0);
			if (client_config.sl < 0)
			{
				usage(argv[0]);
				return 1;
			}
			break;
		case 'h':
			usage(argv[0]);
			return 1;
		default:
			usage(argv[0]);
			return 1;
		}
	}
	/* parse the last parameter (if exists) as the server name */
	if (optind == argc - 1)
		client_config.server_name = argv[optind];
	/* print the used parameters for info*/
	print_config(client_config);
	/* init all of the resources, so cleanup will be easy */
	resources_init(&res);
	/* create resources before using them */
	if (resources_create(&res, client_config))
	{
		fprintf(stderr, "failed to create resources\n");
		goto main_exit;
	}
	/* connect the QPs */
	if (connect_qp(&res, client_config))
	{
		fprintf(stderr, "failed to connect QPs\n");
		goto main_exit;
	}
	printf("Connected.");
main_exit:
	if (resources_destroy(&res))
	{
		fprintf(stderr, "failed to destroy resources\n");
		rc = 1;
	}
	if (client_config.dev_name)
		free((char *)client_config.dev_name);
	fprintf(stdout, "\ntest result is %d\n", rc);
	return rc;
}