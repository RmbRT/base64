#include "base64.hpp"

#include <stdexcept>
#include <cassert>

namespace base64
{
	Configuration::Configuration(
		char const * alphabet,
		char padding)
	{
		assert(alphabet);

		this->padding = padding;

		// set all unused fields to invalid.
		for(std::size_t i = 256; i--;)
			reverse_table[i] = 255;

		for(std::size_t i = 0; i < 64; i++)
		{
			table[i] = alphabet[i];
			reverse_table[table[i]] = i;
			assert(alphabet[i]);
		}
	}

	Configuration::Configuration(
		char c63,
		char c64,
		char padding)
	{
		this->padding = padding;

		// set all unused fields to invalid.
		for(std::size_t i = 256; i--;)
			reverse_table[i] = 255;

		static char const * alphabet =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789";

		for(std::size_t i = 0; i < 62; i++)
		{
			table[i] = alphabet[i];
			reverse_table[table[i]] = i;
			assert(alphabet[i]);
		}

		table[62] = c63;
		reverse_table[std::uint8_t(c63)] = 62;
		assert(c63);

		table[63] = c64;
		reverse_table[std::uint8_t(c64)] = 63;
		assert(c64);
	}


	Configuration const Configuration::default_configuration {
		'+',
		'/',
		'='
	};

	static inline void encode_triplet(
		std::uint8_t const * read,
		std::uint8_t * write,
		Configuration const& c)
	{
		// 2-7.
		write[0] = c.table[read[0] >> 2];
		// 0-1, 4-7.
		write[1] = c.table[std::uint8_t((read[0] & 3) << 4) | (read[1]>>4)];
		// 0-3, 6-7.
		write[2] = c.table[((read[1] & 15) << 2) | (read[2]>>6)];
		// 0-5.
		write[3] = c.table[read[2] & 63];
	}

	static inline std::size_t encode_last_triplet(
		std::uint8_t const * read,
		std::uint8_t * write,
		std::size_t rest,
		Configuration const& c)
	{
		if(!rest)
			return 0;

		// 0-5.
		write[0] = c.table[read[0] >> 2];

		if(rest == 2)
		{
			// 6-7, 0-3.
			write[1] = c.table[std::uint8_t((read[0] & 3) << 4) | (read[1]>>4)];
			// 4-7.
			write[2] = c.table[(read[1] & 15) << 2];
		}
		else
		{
			// 6-7.
			write[1] = c.table[std::uint8_t((read[0] & 3) << 4)];
			// padding.
			write[2] = c.padding;
		}

		// padding.
		write[3] = c.padding;

		return 4;
	}

	void * encode(
		void const * input,
		std::size_t size,
		void * output,
		Configuration const& c)
	{
		assert(input != nullptr);
		assert(output != nullptr);

		std::size_t triplets = size / 3;
		std::uint8_t const * read = static_cast<std::uint8_t const*>(input);
		std::uint8_t * write = static_cast<std::uint8_t *>(output);

		while(triplets--)
		{
			encode_triplet(read, write, c);

			read += 3;
			write += 4;
		}

		return write + encode_last_triplet(read, write, size % 3, c);
	}

	void * encode_in_place(
		void * input,
		std::size_t size,
		Configuration const& c)
	{
		assert(input != nullptr);

		std::size_t triplets = size / 3;
		std::size_t rest = size % 3;
		std::uint8_t const * read = static_cast<std::uint8_t const*>(input) + triplets*3;
		std::uint8_t * write = static_cast<std::uint8_t *>(input) + triplets*4;

		// the last triplet.
		std::uint8_t triplet[3];
		for(std::size_t i = 0; i < rest; i++)
			triplet[i] = read[i];

		void * const result = write + encode_last_triplet(read, write, rest, c);

		std::size_t last_triplets;

		// safely 
		if(triplets > 3)
		{
			last_triplets = 3;
			for(std::size_t count = triplets - 3; count--;)
			{
				read -= 3;
				write -= 4;
				encode_triplet(read, write, c);
			}
		} else
			last_triplets = triplets ? triplets : 0;

		// the last triplets have to be done with temp storing.
		for(std::size_t i = last_triplets; i--;)
		{
			read -= 3;
			write -= 4;

			triplet[0] = read[0];
			triplet[1] = read[1];
			triplet[2] = read[2];

			encode_triplet(triplet, write, c);
		}

		return result;
	}

	static inline void decode_quartet(
		std::uint8_t const * read,
		std::uint8_t * write,
		Configuration const& c)
	{
		assert(~c.reverse_table[read[0]]);
		assert(~c.reverse_table[read[1]]);
		assert(~c.reverse_table[read[2]]);
		assert(~c.reverse_table[read[3]]);

		write[0] = (c.reverse_table[read[0]] << 2) | (c.reverse_table[read[1]] >> 4);
		write[1] = (c.reverse_table[read[1]] << 4) | (c.reverse_table[read[2]] >> 2);
		write[2] = (c.reverse_table[read[2]] << 6) | c.reverse_table[read[3]];
	}

	static inline std::size_t decode_last_quartet(
		std::uint8_t const * read,
		std::uint8_t * write,
		Configuration const& c)
	{
		assert(~c.reverse_table[read[0]]);
		assert(~c.reverse_table[read[1]]);
		assert(~c.reverse_table[read[2]]);
		assert(~c.reverse_table[read[3]]);

		write[0] = (c.reverse_table[read[0]] << 2) | (c.reverse_table[read[1]] >> 4);
		if(read[2] == c.padding)
		{
			write[1] = c.reverse_table[read[1]] << 4;
			return 1;
		} else
		{
			write[1] = (c.reverse_table[read[1]] << 4) | (c.reverse_table[read[2]] >> 2);
			if(read[3] == c.padding)
			{
				write[2] = c.reverse_table[read[2]] << 6;
				return 2;
			}
			else
				write[2] = (c.reverse_table[read[2]] << 6) | c.reverse_table[read[3]];

			return 3;
		}
	}

	void * decode(
		void const * input,
		std::size_t size,
		void * output,
		Configuration const& c)
	{
		assert(input != nullptr);
		assert(output != nullptr);


		if(size & 3)
			throw std::runtime_error("size is not a multiple of 4");

		std::size_t quartets = size >> 2;
		std::uint8_t const * read = static_cast<std::uint8_t const*>(input);
		std::uint8_t * write = static_cast<std::uint8_t *>(output);

		if(quartets)
			while(--quartets)
			{
				decode_quartet(read, write, c);
				read += 4;
				write += 3;
			}

		return write + decode_last_quartet(read, write, c);
	}

	void * decode_in_place(
		void * input,
		std::size_t size,
		Configuration const& c)
	{
		assert(input != nullptr);

		if(size & 3)
			throw std::runtime_error("size is not a multiple of 4");

		std::size_t quartets = size >> 2;
		std::uint8_t const * read = static_cast<std::uint8_t const*>(input);
		std::uint8_t * write = static_cast<std::uint8_t *>(input);

		std::size_t lower_quartets = quartets > 3
			? 3
			: quartets
				? quartets - 1
				: 0;
		std::uint8_t triplet[3];
		for(std::size_t i = lower_quartets; i--;)
		{
			decode_quartet(read, triplet, c);
			write[0] = triplet[0];
			write[1] = triplet[1];
			write[2] = triplet[2];

			read += 4;
			write += 3;
		}

		if(quartets > 3)
		{
			for(std::size_t i = quartets - 4; i--;)
			{
				decode_quartet(read, write, c);
				read += 4;
				write += 3;
			}
			return write + decode_last_quartet(read, write, c);
		} else
		{
			std::size_t writes = decode_last_quartet(read, triplet, c);
			void * const result = write + writes;

			for(std::size_t i = writes+1; i--;)
				write[i] = triplet[i];

			return result;
		}
	}
}