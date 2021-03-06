


#include "ozone.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>



void test_ozread_open()
{
	OZRead or;
	int ret;
	ret = ozread_open(&or, "/home/liheyuan/workspace_cpp_yq/kesdb/testd_db/");
	printf("test_ozread_open() %d\n", ret);
}

void test_ozread_get()
{
	OZRead or = {NULL, 0, NULL};
	if (ozread_open(&or, "/tmp/test_db/"))
	{
		printf("open db fail\n");
	}
	else
	{
		//Param
		OZRead_Get get = {NULL, NULL};
		int ret;
		get._key = "1";
		//Get
		ret = ozread_get(&or, &get);
		if (!ret)
		{
			printf("ozread_get() succ\n");
			if(get._value)
			{
				printf("%s\n", get._value);
			}
		}
		else
		{
			printf("ozread_get() fail %d\n", ret);
		}
		//Remember to free
		ozread_get_free(&get);
	}

	//Close
	ozread_close(&or);
}

void test_ozread_gets()
{
	OZRead or = {NULL, 0, NULL};
	const char* fn = "/tmp/test_db";

	if (ozread_open(&or, fn))
	{
		printf("open db fail\n");
	}
	else
	{
		OZRead_Gets gets;
		int ret, i;
		struct timeval start, end;
		int n = 10;
		char buf[1024];

		printf("open db succ\n");

		// init gets param
		scanf("%d", &n);
		ozread_gets_init(&gets, n);

		// Get n keys
		for (i = 0; i < n; i++)
		{
			if(scanf("%s", buf)!=1)
			{
				printf("Read key error\n");
			}
			else
			{
				gets._keys[i] = (char*)malloc(sizeof(char)*(strlen(buf)+1));
				strcpy(gets._keys[i], buf);
			}
		}

		gettimeofday(&start, NULL);
		ret = ozread_gets(&or, &gets);
		if (!ret)
		{
			printf("ozread_gets() succ\n");
			for (i = 0; i < gets._nkeys; i++)
			{
				printf("%s\n", gets._keys[i]);
				if(gets._values[i])
				{
					printf("length: %d\n", strlen(gets._values[i]));
					printf("%s\n", gets._values[i]);
				}
				else
				{
					printf("not found \n");
				}
			}
		}
		else
		{
			printf("ozread_gets() fail %d\n", ret);
		}

		gettimeofday(&end, NULL);
		printf("time %lf(s)\n", ((end.tv_sec * 1000000 + end.tv_usec)
				- (start.tv_sec * 1000000 + start.tv_usec)) / (float) (1000000));

		// free keys 
		for(i=0; i<n; i++)
		{
			free(gets._keys[i]);
		}
		// free others
		ozread_gets_free(&gets);
	}

	//Close
	ozread_close(&or);
}

void test_ozwrite_put()
{
	OZWrite ow;
	char key[OZ_KEY_BUF_SIZE];
	char value[4096];
	int i, j;

	if (ozwrite_open(&ow, "/tmp/test_db"))
	{
		printf("open db fail\n");
	}
	else
	{
		printf("open db succ\n");
		srandom(time(NULL));
		for (i = 0; i < 300000; i++)
		{
			long k = random() % 10000000;
			snprintf(key, OZ_KEY_BUF_SIZE, "%ld", k);
			value[0] = '\0';
			for(j = 0; j < 50; j++)
			{
				strcat(value, " ");
				strcat(value, key);
			}
			if(ozwrite_put(&ow, key, value, strlen(value)))
			{
				printf("write %s fail.\n", key);
			}
		}
	}

	//Close
	ozwrite_close(&ow);
}

void test_oztrav_open()
{
	OZTrav ot;
	int ret;
	ret = oztrav_open(&ot, "/tmp/test_db");
	oztrav_close(&ot);
	printf("test_oztrav_open() %d\n", ret);
}

void test_oztrav_next()
{
	OZTrav ot;
	int ret;
	ret = oztrav_open(&ot, "/tmp/test_db");
	if(ret)
	{
		printf("oztrav_open fail.\n");
	}
	else
	{
		// Init cursor
		OZTrav_Cursor cur;
		oztrav_cursor_init(&cur);
		// Traverse
		while(!oztrav_next(&ot, &cur))
		{
			printf("key: %s\n", cur._key);	
			printf("value: %s\n", cur._value);	
		}
		// Free cursor
		oztrav_cursor_free(&cur);
	}
	oztrav_close(&ot);
}

int main()
{
	//test_ozread_open();
	//test_ozread_get();
	//test_ozread_gets();
	//test_ozwrite_put();
	//test_oztrav_open();
	test_oztrav_next();

	//sleep(10);
	return 0;

}

