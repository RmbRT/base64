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
	/** Converts data to base64.
	@param[in] input:
		The data to convert.
	@param[in] size:
		The size of the input data in bytes.
	@param[out] output:
		The output data. Must contain `(4*size/3)+4` bytes.
	@return
		The pointer to the byte after the last written byte. */
	void * encode(
		void const * input,
		std::size_t size,
		void * output);

	/** Converts data to base64 in-place.
	@param[in] input:
		The data to convert. Must be (4*size/3)+4` bytes.
	@param[in] size:
		The input data byte count.
	@return
		The pointer to the byte after the last written byte. */
	void * encode_in_place(
		void * input,
		std::size_t size);


#ifndef BASE64_NO_GIMMICKS
	/** Converts data to a base64 string.
	@param[in] input:
		The data to convert.
	@param[in] size:
		The size of the input data in bytes.
	@return
		The base64 representation. */
	inline std::string encode(
		void const * input,
		std::size_t size);

	/** Converts data to a base64 string.
	@param[in] input:
		The data to convert.
	@return
		The base64 representation. */
	inline std::string encode(
		std::vector<std::uint8_t> const& input);

	/** Converts data to a base64 string.
	@param[in] input:
		The data to convert.
	@return
		The base64 representation. */
	inline std::string encode(
		std::string const& input);
#endif

	/** Converts base64 to data.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The byte count of the input data.
	@param[out] output:
		The data. Must contain `(
	@return
		The pointer to the byte after the last written byte. */
	void * decode(
		void const * input,
		std::size_t size,
		void * output);

	/** Converts base64 to data in-place.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The input data byte count.
	@return
		The pointer to the byte after the last written byte. */
	void * encode_in_place(
		void * input,
		std::size_t size);

#ifndef BASE64_NO_GIMMICKS
	/** Decodes data into a vector.
	@param[in] input:
		The base64 encoded data.
	@param[in] size:
		The byte count of the data. */
	inline std::vector<std::uint8_t> decode(
		void const * input,
		std::size_t size);

	/** Decodes the data into a vector.
	@param[in] input:
		The base64 encoded data. */
	inline std::vector<std::uint8_t> decode(
		std::string const& input);
#endif

	/** Precomputes lookup tables. */
	void precompute();
}

#ifndef BASE64_NO_GIMMICKS
#include "base64.inl"
#endif

#endif