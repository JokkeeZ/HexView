#include <stdio.h>
#include <stdint.h>

#define READ_SIZE 24
#define SEGMENT_SIZE 12

void print_bytes_as_ascii(uint8_t *buffer, size_t bytes_readed)
{
	for (int i = 0; i < bytes_readed; ++i) {
		if (buffer[i] >= '!' && buffer[i] <= '~') {
			printf("%c", buffer[i]);
		}
		else {
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
		}
		else {
			// We have already readed all the available bytes,
			// so lets add whitespace so ascii is always on the same position.
			printf("  ");
		}

		if ((((i + 1) % (SEGMENT_SIZE / 2)) == 0) && (i != READ_SIZE - 1)) {
			printf(" ");
		}

		if (i == (READ_SIZE - 1)) {
			printf("  ");
		}
	}
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
		printf("Expected file size >= %d bytes, received only %d.\n", READ_SIZE, file_length);
		fclose(file);
		return 1;
	}

	long offset = 0L;

	while (offset < file_length) {
		uint8_t buffer[READ_SIZE];
		size_t read_count = fread(buffer, 1, READ_SIZE, file);

		printf("0x%06X  ", offset);

		print_hex_segments(buffer, read_count);
		print_bytes_as_ascii(buffer, read_count);

		offset += READ_SIZE;
	}

	fclose(file);
	return 0;
}
