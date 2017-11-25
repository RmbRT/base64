namespace base64
{
	std::string encode(
		void const * input,
		std::size_t size)
	{
		std::string result;
		std::size_t triplets = size / 3;
		std::size_t last_triplet = size % 3 ? 4:0;

		result.resize(4 * triplets + last_triplet);
		char const * end = static_cast<char *>(encode(input, size, &result[0]));
		result.resize(end-result.data());
		return std::move(result);
	}

	std::string encode(
		std::vector<std::uint8_t> const& input)
	{
		return encode(input.data(), input.size());
	}

	std::string encode(
		std::string const& input)
	{
		return encode(input.data(), input.size());
	}

	std::vector<std::uint8_t> decode(
		void const * input,
		std::size_t size)
	{
		std::vector<std::uint8_t> result;
		result.resize(3 * (size >> 2));
		std::uint8_t * end = static_cast<std::uint8_t *>(decode(input, size, result.data()));
		result.resize(end - result.data());
		return std::move(result);
	}

	std::vector<std::uint8_t> decode(
		std::string const& input)
	{
		return decode(input.data(), input.size());
	}
}