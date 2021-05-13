#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define READ_SIZE 24
#define SEGMENT_SIZE 4

void print_bytes_as_ascii(uint8_t *buffer, size_t bytes_readed)
{
	for (int i = 0; i < bytes_readed; ++i) {
		if (buffer[i] >= '!' && buffer[i] <= '~') {
			printf("%c", buffer[i]);
		} else {
			printf(".");
		}
	}

	printf("\n");
}

void print_hex_segments(uint8_t *buffer, size_t bytes_readed)
{
	for (int i = 0; i < READ_SIZE; ++i) {
		if (i <= bytes_readed) {
			printf("%02X", buffer[i]);
		} else {
			printf("  ");
		}

		if ((i + 1) % SEGMENT_SIZE == 0
			&& i != (READ_SIZE - 1)) {
			printf(" ");
		}
	}
}

int get_offset_width(long file_length)
{
	long quotient = file_length / 16;
	int width = 1;

	while (quotient != 0) {
		quotient /= 16;
		++width;
	}

	return width;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: HexView \"PATH-TO-FILE\"\n");
		return 1;
	}

	FILE *file = fopen(argv[1], "rb");

	if (file == NULL) {
		printf("File '%s' doesn't exist.\n", argv[1]);
		fclose(file);
		return 1;
	}

	fseek(file, 0, SEEK_END);

	long file_length = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (file_length < READ_SIZE) {
		printf("Expected file size >= %d bytes, received only %d.\n",
			READ_SIZE, file_length);
		fclose(file);
		return 1;
	}

	char *format = (char *)malloc(9 * sizeof(char));
	sprintf(format, "0x%%0%ix  ", get_offset_width(file_length));

	while (ftell(file) < file_length) {
		uint8_t buffer[READ_SIZE];

		printf(format, ftell(file));

		size_t read_count = fread(buffer, 1, READ_SIZE, file);

		print_hex_segments(buffer, read_count);
		printf("  ");
		print_bytes_as_ascii(buffer, read_count);
	}

	free(format);
	fclose(file);
	return 0;
}
