#include "base64.hpp"

#include <stdexcept>
#include <cassert>

namespace base64
{
	static std::uint8_t const table[64] = {
		'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P',
		'Q','R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f',
		'g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v',
		'w','x','y','z','0','1','2','3','4','5','6','7','8','9','+','/'
	}, padding = '=';

	static inline void encode_triplet(
		std::uint8_t const * read,
		std::uint8_t * write)
	{
		// 2-7.
		write[0] = table[read[0] >> 2];
		// 0-1, 4-7.
		write[1] = table[std::uint8_t((read[0] & 3) << 4) | (read[1]>>4)];
		// 0-3, 6-7.
		write[2] = table[((read[1] & 15) << 2) | (read[2]>>6)];
		// 0-5.
		write[3] = table[read[2] & 63];
	}

	static inline std::size_t encode_last_triplet(
		std::uint8_t const * read,
		std::uint8_t * write,
		std::size_t rest)
	{
		if(!rest)
			return 0;

		// 0-5.
		write[0] = table[read[0] >> 2];

		if(rest == 2)
		{
			// 6-7, 0-3.
			write[1] = table[std::uint8_t((read[0] & 3) << 4) | (read[1]>>4)];
			// 4-7.
			write[2] = table[(read[1] & 15) << 2];
		}
		else
		{
			// 6-7.
			write[1] = table[std::uint8_t((read[0] & 3) << 4) | (read[1]>>4)];
			// padding.
			write[2] = padding;
		}

		// padding.
		write[3] = padding;

		return 4;
	}

	void * encode(
		void const * input,
		std::size_t size,
		void * output)
	{
		assert(input != nullptr);
		assert(output != nullptr);

		std::size_t triplets = size / 3;
		std::uint8_t const * read = static_cast<std::uint8_t const*>(input);
		std::uint8_t * write = static_cast<std::uint8_t *>(output);

		while(triplets--)
		{
			encode_triplet(read, write);

			read += 3;
			write += 4;
		}

		return write + encode_last_triplet(read, write, size % 3);
	}

	void * encode_in_place(
		void * input,
		std::size_t size)
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

		void * const result = write + encode_last_triplet(read, write, rest);

		std::size_t last_triplets;

		// safely 
		if(triplets > 3)
		{
			last_triplets = 3;
			for(std::size_t count = triplets - 3; count--;)
			{
				read -= 3;
				write -= 4;
				encode_triplet(read, write);
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

			encode_triplet(triplet, write);
		}

		return result;
	}

	static bool initialised = false;
	static std::uint8_t reverse_table[256];

	static inline void decode_quartet(
		std::uint8_t const * read,
		std::uint8_t * write)
	{
		assert((read[0] | read[1] | read[2]) < 64);

		write[0] = (reverse_table[read[0]] << 2) | (reverse_table[read[1]] >> 4);
		write[1] = (reverse_table[read[1]] << 4) | (reverse_table[read[2]] >> 2);
		write[2] = (reverse_table[read[2]] << 6) | reverse_table[read[3]];
	}

	static inline std::size_t decode_last_quartet(
		std::uint8_t const * read,
		std::uint8_t * write)
	{
		assert((read[0] | read[1] | read[2]) < 64);

		write[0] = (reverse_table[read[0]] << 2) | (reverse_table[read[1]] >> 4);
		if(read[2] == padding)
		{
			write[1] = reverse_table[read[1]] << 4;
			return 1;
		} else
		{
			write[1] = (reverse_table[read[1]] << 4) | (reverse_table[read[2]] >> 2);
			if(read[3] == padding)
				write[2] = reverse_table[read[2]] << 6;
			else
				write[2] = (reverse_table[read[2]] << 6) | reverse_table[read[3]];

			return 2;
		}
	}

	void * decode(
		void const * input,
		std::size_t size,
		void * output)
	{
		assert(input != nullptr);
		assert(output != nullptr);

		precompute();

		if(size & 3)
			throw std::runtime_error("size is not a multiple of 4");

		std::size_t quartets = size >> 2;
		std::uint8_t const * read = static_cast<std::uint8_t const*>(input);
		std::uint8_t * write = static_cast<std::uint8_t *>(output);

		if(quartets)
			while(--quartets)
			{
				decode_quartet(read, write);
				read += 4;
				write += 3;
			}

		return write + decode_last_quartet(read, write);
	}

	void * decode_in_place(
		void * input,
		std::size_t size)
	{
		assert(input != nullptr);

		precompute();

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
			decode_quartet(read, triplet);
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
				decode_quartet(read, write);
				read += 4;
				write += 3;
			}
			return write + decode_last_quartet(read, write);
		} else
		{
			std::size_t writes = decode_last_quartet(read, triplet);
			void * const result = write + writes;

			for(std::size_t i = writes+1; i--;)
				write[i] = triplet[i];

			return result;
		}
	}

	void precompute()
	{
		if(initialised)
			return;

		for(std::size_t i = 0; i < sizeof(table); i++)
			reverse_table[table[i]] = i;

		initialised = true;
	}
}