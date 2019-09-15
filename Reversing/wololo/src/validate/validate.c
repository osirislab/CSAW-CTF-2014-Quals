#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define false 0
#define true 1

typedef struct 
{
	uint32_t magic;
	uint32_t version;
	uint16_t num_cols;
	uint16_t num_rows;
} header_t;

typedef struct
{
	uint8_t type;
	char name[16];
} col_t;

/*
 * Column types:
 *   * 0 = 8bit integer
 *   * 1 = 16bit integer
 *   * 2 = 32bit integer
 *   * 3 = 64bit integer
 *   * 4 = 8byte string
 *   * 5 = 16byte string
 *   * 6 = 32byte string
 *   * 7 = unix timestamp encoded as a 32bit integer
 *
 */

int col_size(const col_t *c)
{
	switch (c->type)
	{
		case 0:
			return sizeof(unsigned char);
		case 1:
			return sizeof(uint16_t);
		case 2:
			return sizeof(uint32_t);
		case 3:
			return sizeof(uint64_t);
		case 4:
			return 8 * sizeof(unsigned char);
		case 5:
			return 16 * sizeof(unsigned char);
		case 6:
			return 32 * sizeof(unsigned char);
		case 7:
			return sizeof(uint32_t);
	}

	return 0;
}

int validate_database(unsigned char *db, const uint32_t size)
{
	const header_t *h = (header_t *)db;
	size_t size_check = sizeof(header_t);

	if (size < size_check)
		return false;

	if (h->magic != 'OLOW')
		return false;

	if (h->version != 1)
		return false;

	if (h->num_rows < 4)
		return false;

	if (h->num_rows > 0x1000)
		return false;

	if (h->num_cols < 4)
		return false;

	if (h->num_cols > 0x10)
		return false;

	size_check += h->num_cols * sizeof(col_t);
	if (size < size_check)
		return false;

	size_t row_size = 0;
	unsigned char *cur = db + sizeof(header_t);
	for (unsigned int i = 0; i < h->num_cols; i++)
	{
		unsigned int sz = col_size((col_t *)cur);
		if (sz == 0)
			return false;

		row_size += sz;
		cur += sizeof(col_t);
	}

	size_check += row_size * h->num_rows;
	if (size < size_check)
		return false;

	return true;
}

int check_login(unsigned char *db, const uint32_t size)
{
	const header_t *h = (header_t *)db;

	col_t *cols = (col_t *)(db + sizeof(header_t));

	unsigned char *rows = db + sizeof(header_t);
	rows += h->num_cols * sizeof(col_t);

	size_t row_size = 0;
	unsigned char *cur = db + sizeof(header_t);
	for (unsigned int i = 0; i < h->num_cols; i++)
	{
		unsigned int sz = col_size((col_t *)cur);
		if (sz == 0)
			return false;

		row_size += sz;
		cur += sizeof(col_t);
	}

	cur = rows;
	const unsigned char *end = db + size;
	for (; cur < end; cur += row_size)
	{
		char *sub_cur = (char *)cur;

		int username = 0, password = 0, admin = 0, isawesome = 0;

		for (unsigned int i = 0; i < h->num_cols; i++)
		{

			if (strncmp(cols[i].name, "USERNAME", 8) == 0 && cols[i].type == 5)
			{
				if (strncmp(sub_cur, "captainfalcon", sizeof("captainfalcon")) == 0)
					username = true;
			}



			if (strncmp(cols[i].name, "PASSWORD", 8) == 0 && cols[i].type == 6)
			{
				if (strncmp(sub_cur, "fc03329505475dd4be51627cc7f0b1f1", 32) == 0)
					password = true;
			}
			


			if (strncmp(cols[i].name, "ADMIN", 5) == 0 && cols[i].type == 0)
			{
				uint8_t admin_check = *((uint8_t *)sub_cur);
				if (admin_check == 1)
					admin = true;
				printf("%u\n", admin);
			}



			if (strncmp(cols[i].name, "ISAWESOME", 9) == 0 && cols[i].type == 0)
			{
				uint8_t awesome_check = *((uint8_t *)sub_cur);
				isawesome = awesome_check == 1;
			}



			printf("%.16s %u\t%p\n", cols[i].name, col_size(&cols[i]), sub_cur);
			sub_cur += col_size(&cols[i]);
		}

		printf("username: %u\tpassword: %u\tadmin: %u\tisawesome: %u\n", username, password, admin, isawesome);

		if (username && password && admin && isawesome)
			return true;
	}


	return false;
}
