# base64

This project is a static, extremely lightweight C++ library that offers basic base64 encoding and decoding. It supports in-place encoding and decoding as well as helper functions that allow easier use. Custom alphabets can be used. Helper functions and STL dependencies can be removed by defining `BASE64_NO_GIMMICKS` before including the library. The comments are doxygen-compatible.

## Contents

These are the most important functions in the library:

* `base64::encode()` for encoding data into base64 format. There is an overloaded version that converts the data into an `std::string`.
* `base64::decode()` for decoding data. There is an overloaded version that converts the data into an `std::vector<std::uint8_t>`.
* `base64::Configuration` allows to create custom alphabets to pass to the algorithms.

## License

This library is licensed under the GNU General Public License, version 3 or newer. A copy can be found in the file `LICENSE`.

## Installation

Make sure `cmake` is installed. Navigate into the `base64` directory and execute:
	
	cmake .

Now, makefiles (or project files) have been generated.

## Future features

Some features are planned to be implemented in the future.

* OpenMP or some other parallelisation support.
* Optional linebreaks in encoded text.
* Optional checksums at the end of lines in encoded text.