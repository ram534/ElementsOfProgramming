#include <stdio.h>
#include <stdlib.h>
#include <vector>

template<typename Traits>
// requires Traits is
//struct my_traits {
//	using handle_type = <<handle type>>;
//	constexpr static auto invalid_value = <<invalid value e.g. nullptr>>;
//	static void close(handle_type h) noexcept { <<close handle h here>>; }
//};
//using my_type = raii_with_invalid_value<my_traits>;
// NOTE: invalid_value needs to be equality comparable with the handle_type (could be a different type)
class raii_with_invalid_value {
	using handle_type = typename Traits::handle_type;

	handle_type h_;

public:
	explicit raii_with_invalid_value(handle_type h) noexcept : h_{ h }
	{
	}

	~raii_with_invalid_value()
	{
		if (is_valid())
			Traits::close(h_);
	}

	handle_type get() {
		return h_;
	}

	bool is_valid() {
		return h_ != Traits::invalid_value;
	}
};

struct char_buffer_traits {
	using handle_type = char *;
	constexpr static auto invalid_value = nullptr;
	static void close(handle_type h) noexcept { free(h); }
};
using char_buffer = raii_with_invalid_value<char_buffer_traits>;

struct file_traits {
	using handle_type = FILE *;
	constexpr static auto invalid_value = nullptr;
	static void close(handle_type h) noexcept { fclose(h); }
};
using file = raii_with_invalid_value<file_traits>;

bool copy_file(const char * src_file_name, const char * dst_file_name) {
	file src{ fopen(src_file_name, "rb") };
	if (!src.is_valid())
		return false;

	file dst{ fopen(dst_file_name, "wb") };
	if (!dst.is_valid())
	{
		return false;
	}

	const size_t buffer_size = 4;
	char_buffer buffer{ (char*)malloc(buffer_size) };
	if (!buffer.is_valid())
	{
		return false;
	}

	for (;;)
	{
		size_t read_count = fread(buffer.get(), 1, buffer_size, src.get());
		if (read_count != buffer_size) {
			if (ferror(src.get())) {
				return false;
			}
		}
		size_t wrote_count = fwrite(buffer.get(), 1, read_count, dst.get());
		if (wrote_count != read_count) {
			return false;
		}

		fputs(".", stdout);

		if (feof(src.get()))
		{
			break;
		}
	}
	return true;
}

int main()
{
	if (copy_file("src.txt", "dst.txt"))
	{
		fputs("\nSUCCESS\n", stdout);
	}
	else
	{
		fputs("\nFAILED\n", stdout);
	}
}