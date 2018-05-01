#ifndef __base64_hpp_defined
#define __base64_hpp_defined

#include <cstddef>
#include <cinttypes>

/** @def BASE64_NO_GIMMICKS
	Define this to prevent adding additional functions.
	This includes multiple STL headers. */
#ifndef BASE64_NO_GIMMICKS
#include <string>
#include <vector>
#endif

namespace base64
{
	/** A configuration of the algorithm with a custom alphabet. */
	struct Configuration
	{
		/** Creates a configuration with the requested alphabet and padding character.
		@param[in] alphabet:
			The configuration's alphabet. Must be 64 characters long.
		@param[in] padding:
			The configuration's padding character. */
		Configuration(
			char const * alphabet,
			char padding);

		/** The default alphabet. */
		static Configuration const default_configuration;

		/** The lookup table used for encoding. */
		std::uint8_t table[64];
		/** The padding character. */
		std::uint8_t padding;
		/** Used for fast decoding. */
		std::uint8_t reverse_table[256];
	};

	/** Converts data to custom base64.
	@param[in] input:
		The data to convert.
	@param[in] size:
		The size of the input data in bytes.
	@param[out] output:
		The output data. Must contain `(4*size/3)+4` bytes.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The pointer to the byte after the last written byte. */
	void * encode(
		void const * input,
		std::size_t size,
		void * output,
		Configuration const& configuration);

	/** Converts data to standard base64.
	@param[in] input:
		The data to convert.
	@param[in] size:
		The size of the input data in bytes.
	@param[out] output:
		The output data. Must contain `(4*size/3)+4` bytes.
	@return
		The pointer to the byte after the last written byte. */
	inline void * encode(
		void const * input,
		std::size_t size,
		void * output);

	/** Converts data to custom base64 in-place.
	@param[in] input:
		The data to convert. Must be (4*size/3)+4` bytes.
	@param[in] size:
		The input data byte count.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The pointer to the byte after the last written byte. */
	void * encode_in_place(
		void * input,
		std::size_t size,
		Configuration const& configuration);

	/** Converts data to standard base64 in-place.
	@param[in] input:
		The data to convert. Must be (4*size/3)+4` bytes.
	@param[in] size:
		The input data byte count.
	@return
		The pointer to the byte after the last written byte. */
	inline void * encode_in_place(
		void * input,
		std::size_t size);


#ifndef BASE64_NO_GIMMICKS
	/** Converts data to a custom base64 string.
	@param[in] input:
		The data to convert.
	@param[in] size:
		The size of the input data in bytes.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The base64 representation. */
	inline std::string encode(
		void const * input,
		std::size_t size,
		Configuration const& configuration);

	/** Converts data to a standard base64 string.
	@param[in] input:
		The data to convert.
	@param[in] size:
		The size of the input data in bytes.
	@return
		The base64 representation. */
	inline std::string encode(
		void const * input,
		std::size_t size);

	/** Converts data to a custom base64 string.
	@param[in] input:
		The data to convert.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The base64 representation. */
	inline std::string encode(
		std::vector<std::uint8_t> const& input,
		Configuration const& config);

	/** Converts data to a standard base64 string.
	@param[in] input:
		The data to convert.
	@return
		The base64 representation. */
	inline std::string encode(
		std::vector<std::uint8_t> const& input);

	/** Converts data to a custom base64 string.
	@param[in] input:
		The data to convert.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The base64 representation. */
	inline std::string encode(
		std::string const& input,
		Configuration const& config);

	/** Converts data to a standard base64 string.
	@param[in] input:
		The data to convert.
	@return
		The base64 representation. */
	inline std::string encode(
		std::string const& input);
#endif

	/** Converts custom base64 to data.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The byte count of the input data.
	@param[out] output:
		The data. Must contain `(3*size/4)` bytes.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The pointer to the byte after the last written byte. */
	void * decode(
		void const * input,
		std::size_t size,
		void * output,
		Configuration const& configuration);

	/** Converts standard base64 to data.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The byte count of the input data.
	@param[out] output:
		The data. Must contain `(3*size/4)` bytes.
	@return
		The pointer to the byte after the last written byte. */
	inline void * decode(
		void const * input,
		std::size_t size,
		void * output);

	/** Converts custom base64 to data in-place.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The input data byte count.
	@param[in] configuration:
		The custom base64 configuration.
	@return
		The pointer to the byte after the last written byte. */
	void * decode_in_place(
		void * input,
		std::size_t size,
		Configuration const& configuration);

	/** Converts standard base64 to data in-place.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The input data byte count.
	@return
		The pointer to the byte after the last written byte. */
	inline void * decode_in_place(
		void * input,
		std::size_t size);

#ifndef BASE64_NO_GIMMICKS
	/** Decodes custom base64 into a vector.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The byte count of the data.
	@param[in] configuration:
		The custom base64 configuration. */
	inline std::vector<std::uint8_t> decode(
		void const * input,
		std::size_t size,
		Configuration const& configuration);

	/** Decodes standard base64 into a vector.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The byte count of the data. */
	inline std::vector<std::uint8_t> decode(
		void const * input,
		std::size_t size);

	/** Decodes custom base64 into a vector.
	@param[in] input:
		The base64 encoded data.
	@param[in] configuration:
		The custom base64 configuration. */
	inline std::vector<std::uint8_t> decode(
		std::string const& input,
		Configuration const& configuration);

	/** Decodes the standard base64 into a vector.
	@param[in] input:
		The base64 encoded data. */
	inline std::vector<std::uint8_t> decode(
		std::string const& input);
#endif
}

#ifndef BASE64_NO_GIMMICKS
#include "base64.inl"
#endif

#endif