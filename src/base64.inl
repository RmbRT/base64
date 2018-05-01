namespace base64
{
	void * encode(
		void const * input,
		std::size_t size,
		void * output)
	{
		return encode(
			input,
			size,
			output,
			Configuration::default_configuration);
	}

	void * encode_in_place(
		void * input,
		std::size_t size)
	{
		return encode_in_place(
			input,
			size,
			Configuration::default_configuration);
	}

#ifndef BASE64_NO_GIMMICKS

	std::string encode(
		void const * input,
		std::size_t size,
		Configuration const& configuration)
	{
		std::string result;
		std::size_t triplets = size / 3;
		std::size_t last_triplet = size % 3 ? 4:0;

		result.resize(4 * triplets + last_triplet);
		char const * end = static_cast<char *>(encode(
			input,
			size,
			&result[0],
			configuration));

		result.resize(end-result.data());
		return std::move(result);
	}

	std::string encode(
		void const * input,
		std::size_t size)
	{
		return encode(input, size, Configuration::default_configuration);
	}

	std::string encode(
		std::vector<std::uint8_t> const& input,
		Configuration const& configuration)
	{
		return encode(input.data(), input.size(), configuration);
	}

	std::string encode(
		std::vector<std::uint8_t> const& input)
	{
		return encode(input.data(), input.size(), Configuration::default_configuration);
	}

	std::string encode(
		std::string const& input,
		Configuration const& configuration)
	{
		return encode(input.data(), input.size(), configuration);
	}

	std::string encode(
		std::string const& input)
	{
		return encode(input.data(), input.size(), Configuration::default_configuration);
	}

#endif

	void * decode(
		void const * input,
		std::size_t size,
		void * output)
	{
		return decode(input, size, output, Configuration::default_configuration);
	}

	void * decode_in_place(
		void * input,
		std::size_t size)
	{
		return decode_in_place(input, size, Configuration::default_configuration);
	}


#ifndef BASE64_NO_GIMMICKS

	std::vector<std::uint8_t> decode(
		void const * input,
		std::size_t size,
		Configuration const& configuration)
	{
		std::vector<std::uint8_t> result;
		result.resize(3 * (size >> 2));
		std::uint8_t * end = static_cast<std::uint8_t *>(decode(
			input,
			size,
			result.data(),
			configuration));

		result.resize(end - result.data());
		return std::move(result);
	}

	std::vector<std::uint8_t> decode(
		void const * input,
		std::size_t size)
	{
		return decode(input, size, Configuration::default_configuration);
	}

	std::vector<std::uint8_t> decode(
		std::string const& input,
		Configuration const& configuration)
	{
		return decode(input.data(), input.size(), configuration);
	}

	std::vector<std::uint8_t> decode(
		std::string const& input)
	{
		return decode(input.data(), input.size(), Configuration::default_configuration);
	}

#endif
}