#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>
#include <sys/stat.h>
#include <assert.h>

typedef struct _hdr {
	uint8_t magic[8];
	uint32_t version;
	uint32_t num_files;
} hdr;

typedef struct _entry {
	uint32_t magic;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
	uint8_t filename[32];
} entry;

char *compress(const char *input, unsigned int *out_size);
char *decompress(const char *in, const unsigned int input_size, unsigned int *decompressed_size);

unsigned int min(unsigned int one, unsigned int two)
{
	if (one < two)
		return one;
	return two;
}

void create_archive(int argc, char **argv) 
{
	const char *output_file = argv[argc - 1];
	FILE *outfp = fopen(output_file, "wb");

	const unsigned int num_files = argc - 3;

	hdr outhdr;
	memset(&outhdr, 0, sizeof(hdr));

	memcpy(&outhdr.magic, "CSAWlz\x00\x00", sizeof(outhdr.magic));
	outhdr.version = 1;
	outhdr.num_files = num_files;

	fwrite(&outhdr, 1, sizeof(hdr), outfp);

	for (unsigned int i = 0; i < num_files; i++)
	{
		const char *input_file = argv[i + 2];

		entry file;
		memset(&file, 0, sizeof(entry));
		file.magic = 0x65654141;
		memcpy(&file.filename, input_file, min(32, strlen(input_file)));

		struct stat st;
		if (stat(input_file, &st) < 0)
			err(2, "Stat failed");

		file.uncompressed_size = st.st_size;

		char *compressed_file = compress(input_file, &file.compressed_size);

		fwrite(&file, 1, sizeof(entry), outfp);
		fwrite(compressed_file, file.compressed_size, 1, outfp);

	}

	fclose(outfp);
}

void decompress_archive(char **argv) 
{
	const char *input = argv[2];

	struct stat st;
	if (stat(input, &st) < 0)
		err(2, "Stat failed");

	char *in = (char *)malloc(st.st_size);
	FILE *fp = fopen(input, "rb");
	if (!fp)
		err(3, "Couldn't open file");

	fread(in, 1, st.st_size, fp);
	fclose(fp);

	unsigned int ii = 0;

	hdr *h = (hdr *)in;
	unsigned int num_files = h->num_files;
	ii += sizeof(hdr);
	
	for (unsigned int i = 0; i < num_files; i++) {
		entry *file = (entry *)&in[ii];
		ii += sizeof(entry);

		printf("magic: %x, compressed: %u, uncompressed: %u, name: %s\n", file->magic, file->compressed_size, file->uncompressed_size, file->filename);

		if (file->magic != 0x65654141)
			errx(2, "Magic doesn't match");

		
		fp = fopen((char *)file->filename, "wb");

		unsigned int out_size = 0;
		char *out = decompress(&in[ii], file->compressed_size, &out_size);
		fwrite(out, out_size, 1, fp);

		fclose(fp);
		ii += file->compressed_size;

	}

}

int main(int argc, char **argv)
{

	if (argc < 3)
		errx(1, "Usage: %s d input_file\n\t%s c input_file [input_files...] output_file\n", argv[0], argv[0]);

	if (strncmp(argv[1], "d", 2) == 0) {
		// decompress the file
		decompress_archive(argv);
	} else {
		// compress the files
		create_archive(argc, argv);
	}

	return 0;
}
